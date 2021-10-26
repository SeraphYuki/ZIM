#include <GL/glew.h>
#include <stdio.h>
#include "graphics.h"
#include "utils.h"
#include "log.h"
#include "window.h"

#define FONT_PATH "resources/font.png"

#define STRDEF(x) STR(x)
#define STR(x) #x

#define CLEAR_COLOR 0,0,0,0

#define POS_ATTRIB "pos"
#define UV_ATTRIB "uv"
#define COLORFG_ATTRIB "colorFG"
#define COLORBG_ATTRIB "colorBG"

#define FONT_SIZE_BITS 4
#define FONT_SIZE_MASK ((1 << FONT_SIZE_BITS)-1)
#define FONTSIZE 9


#define TEXTHEIGHT 60
#define TEXTWIDTH 106
#define MAX_TEXT_CHARS (int)(TEXTHEIGHT*TEXTWIDTH)
#define RENDER_VRAM_SIZE MAX_TEXT_CHARS*6// idk most text characters on screen possible

#define TAB_SPACING 4
#define SS_CHAR_SIZE 0.0625f


typedef struct {
    float r;
    float g;
    float b;
} RGBColor;

static RGBColor ncursesColors[NUM_COLORS] = {
    {0,1,1},
    {1,0,0},
    {1,1,0},
    {0,0,1},
    {0,1,0},
    {1,0,1},
    {1,1,1},
    {0,0,0},
    {0.05,0.05,0.05},
    {0,0,0},
};

static u16          ncursespos[MAX_TEXT_CHARS*6][2];
static float        ncursesuv[MAX_TEXT_CHARS*6][2];
static u8           ncursesfgs[MAX_TEXT_CHARS*6];
static u8           ncursesbgs[MAX_TEXT_CHARS*6];
static u8           currentNCursesPair = 0;
static u8          ncursesAttrPairs[MAX_COLOR_PAIRS][2] = {
    {COLOR_YELLOW,COLOR_BLACK},
    {COLOR_WHITE,COLOR_BLACK},
    {COLOR_WHITE,COLOR_BLACK},
    {COLOR_WHITE,COLOR_BLACK},
    {COLOR_WHITE,COLOR_BLACK},
    {COLOR_WHITE,COLOR_BLACK},
    {COLOR_WHITE,COLOR_BLACK},
    {COLOR_WHITE,COLOR_BLACK},
    {COLOR_WHITE,COLOR_BLACK},
    {COLOR_WHITE,COLOR_BLACK},
    {COLOR_WHITE,COLOR_BLACK},
};
static struct { u32 w; u32 h; } viewport;
static const u8     RectTriangleVerts[] = {0,0,1,0,1,1,1,1,0,1,0,0};
static u8           fontSize = FONTSIZE;
static u32           stringOffset = 0;
// shaders
static Shader_t     shaders[NUM_SHADERS];
// textured/texturelesss vao/vbos
static u32          vao_g;
static u32          posVbo_g;
static u32          uvVbo_g;
// ncurses vao/vbos
static u32          ncursesVao_g;
static u32          ncursesPosVbo_g;
static u32          ncursesUvVbo_g;
static u32          ncursesColorFGVbo_g;
static u32          ncursesColorBGVbo_g;

// quad vao/vbo
static u32          quadVao_g;
static u32          quadVbo_g;
// framebuffer
static u32          fb_g;
static u32          fbTexture_g;
// textures
static Image_t     font_g;

static void Compile(Shader_t *shader, const char *vSource, const char *fSource){

    shader->program = glCreateProgram();
    shader->fShader = glCreateShader(GL_FRAGMENT_SHADER);
    shader->vShader = glCreateShader(GL_VERTEX_SHADER);
    
    GLint status = GL_TRUE;

    char buffer[512];

    glShaderSource(shader->fShader, 1, (const GLchar **)&fSource, NULL);
    glCompileShader(shader->fShader);
    glGetShaderiv(shader->fShader, GL_COMPILE_STATUS, &status);

    if(status != GL_TRUE){
        glGetShaderInfoLog(shader->fShader, 512, NULL, buffer);
        LOG(LOG_RED, "FSHADER: %s", buffer);
        return;
    }

    glAttachShader(shader->program, shader->fShader);

    glShaderSource(shader->vShader, 1, (const GLchar **)&vSource, NULL);
    glCompileShader(shader->vShader);
    glGetShaderiv(shader->vShader, GL_COMPILE_STATUS, &status);

    if(status != GL_TRUE){
        glGetShaderInfoLog(shader->vShader, 512, NULL, buffer);
        LOG(LOG_RED, "VSHADER: %s", buffer);
        return;
    }

    glBindAttribLocation(shader->program, POS_LOC, POS_ATTRIB);
    glBindAttribLocation(shader->program, UV_LOC, UV_ATTRIB);
    glBindAttribLocation(shader->program, COLORFG_LOC, COLORFG_ATTRIB);
    glBindAttribLocation(shader->program, COLORBG_LOC, COLORBG_ATTRIB);

    glAttachShader(shader->program, shader->vShader);
    glLinkProgram(shader->program);

    glUseProgram(shader->program);

    shader->uniColorLoc = glGetUniformLocation(shader->program, "uniformColor");
    shader->invViewportLoc = glGetUniformLocation(shader->program, "invViewport");
    shader->ncursesColorsLoc = glGetUniformLocation(shader->program, "colors");
    glUniform3fv(shader->ncursesColorsLoc, NUM_COLORS-1, (float *)&ncursesColors[0].r);
    glUniform2f(shader->invViewportLoc,1.0f/viewport.w, 1.0f/viewport.h);
}

static const char *VS_Source = "#version 120\n"
STR(
attribute vec2 pos;
attribute vec2 uv;
varying vec2 TexCoord;
uniform vec2 invViewport;
)


STR(

void main(){

    TexCoord = uv;
    gl_Position = vec4(((pos * invViewport) * 2) - 1, -1, 1);
}
);

static const char *VSNCurses_Source = "#version 120\n"
STR(
attribute vec2 pos;
attribute vec2 uv;
attribute float colorFG;
attribute float colorBG;
varying vec2 TexCoord;
varying float ColorOutFG;
varying float ColorOutBG;
uniform vec2 invViewport;
)


STR(

void main(){

    ColorOutFG = colorFG;
    ColorOutBG = colorBG;
    TexCoord = uv;
    gl_Position = vec4(((pos * invViewport) * 2) - 1, -1, 1);
}
);

static const char *FSNCurses_Source = "#version 120\n"
STR(
varying vec2 TexCoord;
varying float ColorOutFG;
varying float ColorOutBG;
uniform sampler2D tex;
uniform vec3 colors[16];

void main(){

    vec4 color = texture2D(tex, TexCoord);

    if(color.a < 0.5){
        color = vec4(colors[int(ColorOutBG)],1);
    }else 
    {
        color = color * vec4(colors[int(ColorOutFG)],1);
    }

    gl_FragColor = color;
}
);

static const char *FS_Source = "#version 120\n"
STR(
varying vec2 TexCoord;
uniform sampler2D tex;
uniform vec4 uniformColor = vec4(1,1,1,1);

void main(){

    vec4 color = texture2D(tex, TexCoord);
    if(color.a < 0.5) discard;

    gl_FragColor = color * vec4(uniformColor.rgb*uniformColor.a, 1);
}
);

static const char *VS_Quad_Source = "#version 120\n"
STR(
attribute vec2 pos;
varying vec2 TexCoord;
)

STR(

void main(){
    TexCoord = pos;
    vec2 screenSpace = vec2((pos.x * 2) - 1,((1-pos.y) * 2) - 1);
    gl_Position = vec4(screenSpace.x, screenSpace.y, -1, 1);
}
);

static const char *FS_Quad_Source = "#version 120\n"
STR(
varying vec2 TexCoord;
uniform sampler2D tex;

void main(){

    vec3 color = texture2D(tex, TexCoord).rgb;

    gl_FragColor = vec4(color, 1);
}
);

static const char *VS_Textureless_Source = "#version 120\n"
STR(
attribute vec2 pos;
uniform vec2 invViewport;
)

STR(

void main(){
    gl_Position = vec4(((pos * invViewport) * 2) - 1, -1, 1);
}
);

static const char *FS_Textureless_Source = "#version 120\n"
STR(
uniform vec4 uniformColor = vec4(1,1,1,1);

void main(){

    gl_FragColor = vec4(uniformColor.rgb*uniformColor.a, 1);
}
);

static void CreateFrameBuffer(void){

    glGenFramebuffers(1,&fb_g);
    glBindFramebuffer(GL_FRAMEBUFFER, fb_g);

    glGenTextures(1, &fbTexture_g);
    glBindTexture(GL_TEXTURE_2D, fbTexture_g);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, viewport.w, viewport.h, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fbTexture_g, 0);

    glDrawBuffer(GL_COLOR_ATTACHMENT0);

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE){
        glDeleteFramebuffers(1, &fb_g);
        glDeleteTextures(1, &fbTexture_g);
        LOG(LOG_RED, "Error creating framebuffer.");
    }

    glViewport(0, 0, viewport.w, viewport.h);

    glClearColor(CLEAR_COLOR);
    glClear(GL_COLOR_BUFFER_BIT);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Graphics_Init(void){

    viewport.w = WINDOW_INIT_WIDTH;
    viewport.h = WINDOW_INIT_HEIGHT;

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_POLYGON_SMOOTH);

    // textures

    Utils_LoadImage(&font_g, FONT_PATH);

    // compile shaders

    Compile(&shaders[TEXTURED_SHADER], VS_Source, FS_Source);
    Compile(&shaders[QUAD_SHADER], VS_Quad_Source, FS_Quad_Source);
    Compile(&shaders[NCURSES_SHADER], VSNCurses_Source, FSNCurses_Source);
    Compile(&shaders[TEXTURELESS_SHADER], VS_Textureless_Source, FS_Textureless_Source);

    // quad vao

    glGenVertexArrays(1, &quadVao_g);
    glBindVertexArray(quadVao_g);

    glGenBuffers(1, &quadVbo_g);
    glBindBuffer(GL_ARRAY_BUFFER, quadVbo_g);
    glBufferData(GL_ARRAY_BUFFER, sizeof(RectTriangleVerts), RectTriangleVerts, GL_STATIC_DRAW);
    glEnableVertexAttribArray(POS_LOC);
    glVertexAttribPointer(POS_LOC, 2, GL_UNSIGNED_BYTE, GL_FALSE, 0, 0);

    // ncurses vao

    glGenVertexArrays(1, &ncursesVao_g);
    glBindVertexArray(ncursesVao_g);

    glGenBuffers(1, &ncursesPosVbo_g);
    glBindBuffer(GL_ARRAY_BUFFER, ncursesPosVbo_g);
    glBufferData(GL_ARRAY_BUFFER, sizeof(u16)*2*MAX_TEXT_CHARS*6, NULL, GL_STATIC_DRAW);
    glEnableVertexAttribArray(POS_LOC);
    glVertexAttribPointer(POS_LOC, 2, GL_SHORT, GL_FALSE, 0, 0);

    glGenBuffers(1, &ncursesColorBGVbo_g);
    glBindBuffer(GL_ARRAY_BUFFER, ncursesColorBGVbo_g);
    glBufferData(GL_ARRAY_BUFFER, sizeof(u8)*MAX_TEXT_CHARS*6, NULL, GL_STATIC_DRAW);
    glEnableVertexAttribArray(COLORFG_LOC);
    glVertexAttribPointer(COLORFG_LOC, 1, GL_UNSIGNED_BYTE, GL_FALSE, 0, 0);

    glGenBuffers(1, &ncursesColorFGVbo_g);
    glBindBuffer(GL_ARRAY_BUFFER, ncursesColorFGVbo_g);
    glBufferData(GL_ARRAY_BUFFER, sizeof(u8)*MAX_TEXT_CHARS*6, NULL, GL_STATIC_DRAW);
    glEnableVertexAttribArray(COLORBG_LOC);
    glVertexAttribPointer(COLORBG_LOC, 1, GL_UNSIGNED_BYTE, GL_FALSE, 0, 0);

    glGenBuffers(1, &ncursesUvVbo_g);
    glBindBuffer(GL_ARRAY_BUFFER, ncursesUvVbo_g);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*2*MAX_TEXT_CHARS*6, NULL, GL_STATIC_DRAW);
    glEnableVertexAttribArray(UV_LOC);
    glVertexAttribPointer(UV_LOC, 2, GL_FLOAT, GL_FALSE, 0, 0);


    // textured/textureless vao/vbos

    glGenVertexArrays(1, &vao_g);
    glBindVertexArray(vao_g);

    glGenBuffers(1, &posVbo_g);
    glBindBuffer(GL_ARRAY_BUFFER, posVbo_g);
    glBufferData(GL_ARRAY_BUFFER, sizeof(u16)*2*RENDER_VRAM_SIZE, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(POS_LOC);
    glVertexAttribPointer(POS_LOC, 2, GL_SHORT, GL_FALSE, 0, 0);

    glGenBuffers(1, &uvVbo_g);
    glBindBuffer(GL_ARRAY_BUFFER, uvVbo_g);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*2*RENDER_VRAM_SIZE, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(UV_LOC);
    glVertexAttribPointer(UV_LOC, 2, GL_FLOAT, GL_FALSE, 0, 0);

    // init framebuffer
    
    CreateFrameBuffer();

    glBindFramebuffer(GL_FRAMEBUFFER, fb_g);
}

static void DeleteShader(Shader_t *shader){
    glDeleteProgram(shader->program);
    glDeleteShader(shader->fShader);
    glDeleteShader(shader->vShader);
}

void Graphics_Close(void){

    glDeleteFramebuffers(1, &fb_g);
    glDeleteTextures(1, &fbTexture_g);
    
    DeleteShader(&shaders[TEXTURED_SHADER]);
    DeleteShader(&shaders[TEXTURELESS_SHADER]);
    DeleteShader(&shaders[NCURSES_SHADER]);

    glDeleteVertexArrays(1, &quadVao_g);
    glDeleteBuffers(1, &quadVbo_g);

    glDeleteVertexArrays(1, &vao_g);
    glDeleteBuffers(1, &uvVbo_g);
    glDeleteBuffers(1, &posVbo_g);

    glDeleteVertexArrays(1, &ncursesPosVbo_g);
    glDeleteBuffers(1, &ncursesUvVbo_g);
    glDeleteBuffers(1, &ncursesColorFGVbo_g);
    glDeleteBuffers(1, &ncursesColorBGVbo_g);

    glDeleteTextures(1, &font_g.texture);
}

void Graphics_Resize(int w, int h){
    glBindTexture(GL_TEXTURE_2D, fbTexture_g);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

}

void Graphics_Render(void){

    // render

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glBindVertexArray(quadVao_g);


    glViewport(0, 0, viewport.w, viewport.h);

    glUseProgram(shaders[QUAD_SHADER].program);
    glUniform2f(shaders[QUAD_SHADER].invViewportLoc,1.0f, 1.0f);

    glCullFace(GL_FRONT);

    glBindTexture(GL_TEXTURE_2D, fbTexture_g);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    // // end
}

void Graphics_Clear(void){
    glBindFramebuffer(GL_FRAMEBUFFER, fb_g);
    glViewport(0, 0, viewport.w, viewport.h);

    glClearColor(CLEAR_COLOR);
    glClear(GL_COLOR_BUFFER_BIT);
    
    glCullFace(GL_BACK);
}

static int ValidateXY(u32 *x, u32 *y, u32 fontSize, u32 startX, u32 vSpacing, u32 maxWidth){
    // bottom width is the smaller one.
    if(*x+fontSize >= maxWidth - fontSize){

        *y += (fontSize + vSpacing);
        *x = startX;

        // top and bottom height are the same.
        if(*y+fontSize >= viewport.h)
            return -1;
    }

    return 1;
}

void Graphics_RenderString(char *str, u32 x, u32 y, u8 r, u8 g, u8 b){

    glUseProgram(shaders[TEXTURED_SHADER].program);
    glUniform2f(shaders[TEXTURED_SHADER].invViewportLoc, 1.0f/viewport.w, 1.0f/viewport.h); 
    glUniform4f(shaders[TEXTURED_SHADER].uniColorLoc, r / 255.0f, g  / 255.0f, b  / 255.0f, 1); 


    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, font_g.texture);

    glBindVertexArray(vao_g);

    u32 offset = 0;

    u16 pos[2];
    float uv[2];

    u32 k;

    float fontSize = font_g.width / 16;
    int startX = x;
    int vSpacing = 0;
    int hSpacing = 0;
    int maxWidth = 300;

    char p;
    while((p = *str++) && offset < MAX_TEXT_CHARS*6){


        float tX = (p % 16) / 16.0f;
        float tY = (1 - SS_CHAR_SIZE) - (floorf(p / 16.0f) / 16.0f);

        p -= ' ';

        for(k = 0; k < 12; k+=2){

        if(p == '\n'){

            y += (fontSize + vSpacing);
            
            x = startX;

            // top and bottom height are the same.
            if(y+fontSize >= viewport.h)
                break;

            continue;
        
        } else if(p == '\t' ){
            
            x += (fontSize + hSpacing) * TAB_SPACING;
            
            if(ValidateXY(&x, &y, fontSize, startX, vSpacing, maxWidth) < 0) break;

            continue;

        } else if(p < 32){

            break;
        
        } else if(p == 32){

            x += (fontSize + hSpacing);

            if(ValidateXY(&x, &y, fontSize, startX, vSpacing, maxWidth) < 0) break;

            continue;
        }

            uv[0] = ((RectTriangleVerts[k] * SS_CHAR_SIZE) + tX);
            uv[1] = 1 - ((RectTriangleVerts[k+1] * SS_CHAR_SIZE) + tY);
            pos[0] = (RectTriangleVerts[k] * fontSize) + x;
            pos[1] = ((1-RectTriangleVerts[k+1]) * fontSize) + y;

            // pos[0] = ((s16)RectTriangleVerts[k] << 3) + x;
            // pos[1] = ((s16)RectTriangleVerts[k+1] << 3) + y;
            // uv[0] = (float)((RectTriangleVerts[k] + (p & FONT_SIZE_MASK)) << 3) * font_g.invW;
            // uv[1] = (float)((RectTriangleVerts[k+1] + (p >> FONT_SIZE_BITS)) << 3) * font_g.invH;
        
            glBindBuffer(GL_ARRAY_BUFFER, posVbo_g);
            glBufferSubData(GL_ARRAY_BUFFER, offset*sizeof(pos), sizeof(pos), pos);
            glBindBuffer(GL_ARRAY_BUFFER, uvVbo_g);
            glBufferSubData(GL_ARRAY_BUFFER, offset*sizeof(uv), sizeof(uv), uv);
            ++offset;
        }
        
        x += fontSize;
    }



    glDrawArrays(GL_TRIANGLES, 0, offset);
}


void Graphics_RenderSprite(float x, float y, u16 w, u16 h, u16 tx, u16 ty, u16 tw, u16 th, Image_t tex){

    glBindVertexArray(vao_g);

    u32 offset = 0;
    u16 pos[2];

    float uv[2];

    u32 k;
    for(k = 0; k < 12; k+=2){

        pos[0] = x + ((s16)RectTriangleVerts[k] * w);
        pos[1] = y + ((s16)RectTriangleVerts[k+1] * h);

        uv[0] = ((float)(RectTriangleVerts[k] * tw) + tx) * tex.invW;
        uv[1] = ((float)(RectTriangleVerts[k+1] * th) + ty) * tex.invH;

        glBindBuffer(GL_ARRAY_BUFFER, posVbo_g);
        glBufferSubData(GL_ARRAY_BUFFER, offset*sizeof(pos), sizeof(pos), pos);
        glBindBuffer(GL_ARRAY_BUFFER, uvVbo_g);
        glBufferSubData(GL_ARRAY_BUFFER, offset*sizeof(uv), sizeof(uv), uv);
    
        ++offset;
    }


    glUseProgram(shaders[TEXTURED_SHADER].program);
    glUniform2f(shaders[TEXTURED_SHADER].invViewportLoc, 1.0f/viewport.w, 1.0f/viewport.h); 

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex.texture);

    glBindVertexArray(vao_g);
    glBindBuffer(GL_ARRAY_BUFFER, posVbo_g);

    glDrawArrays(GL_TRIANGLES, 0, 6);
}


void Graphics_RenderRect(float x, float y, u16 w, u16 h, u8 r, u8 g, u8 b, u8 a){

    glBindFramebuffer(GL_FRAMEBUFFER, fb_g);
    glBindVertexArray(vao_g);

    u32 offset = 0;
    u16 pos[2];

    u32 k;
    for(k = 0; k < 12; k+=2){
        pos[0] = x + ((s16)RectTriangleVerts[k] * w);
        pos[1] = y + ((s16)RectTriangleVerts[k+1] * h);

        glBindBuffer(GL_ARRAY_BUFFER, posVbo_g);
        glBufferSubData(GL_ARRAY_BUFFER, offset*sizeof(pos), sizeof(pos), pos);
        // glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(RectTriangleVerts), RectTriangleVerts);
    
        ++offset;
    }


    glUseProgram(shaders[TEXTURELESS_SHADER].program);
    glUniform4f(shaders[TEXTURELESS_SHADER].uniColorLoc, r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f); 
    glUniform2f(shaders[TEXTURELESS_SHADER].invViewportLoc, 1.0f/viewport.w, 1.0f/viewport.h); 

    glBindVertexArray(vao_g);
    glBindBuffer(GL_ARRAY_BUFFER, posVbo_g);

    glDisableVertexAttribArray(UV_LOC);

    glDrawArrays(GL_TRIANGLES, 0, 6);

    glEnableVertexAttribArray(UV_LOC);

    glUniform4f(shaders[TEXTURELESS_SHADER].uniColorLoc, 1, 1, 1, 1); 
}

void Graphics_RenderRectLines(float x, float y, u16 w, u16 h, u8 r, u8 g, u8 b, u8 a){

    glBindVertexArray(vao_g);

    u16 buffer[2*4];

    buffer[0] = x;
    buffer[1] = y;
    buffer[2] = x + w;
    buffer[3] = y;
    buffer[4] = x + w;
    buffer[5] = y + h;
    buffer[6] = x;
    buffer[7] = y + h;

    glBindBuffer(GL_ARRAY_BUFFER, posVbo_g);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(buffer), buffer);


    glUseProgram(shaders[TEXTURELESS_SHADER].program);
    glUniform4f(shaders[TEXTURELESS_SHADER].uniColorLoc, r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f); 
    glUniform2f(shaders[TEXTURELESS_SHADER].invViewportLoc, 1.0f/viewport.w, 1.0f/viewport.h); 

    glBindVertexArray(vao_g);
    glBindBuffer(GL_ARRAY_BUFFER, posVbo_g);

    glDisableVertexAttribArray(UV_LOC);

    glDrawArrays(GL_LINE_STRIP, 0, 4);

    glEnableVertexAttribArray(UV_LOC);

    glUniform4f(shaders[TEXTURELESS_SHADER].uniColorLoc, 1, 1, 1, 1); 
}

void Graphics_UseShader(int shader){
    glUseProgram(shaders[shader].program);
}


u32 Graphics_TextCollumns(){
   return TEXTHEIGHT; 
}
u32 Graphics_TextRows(){
   return TEXTWIDTH;
}

void Graphics_SetFontSize(u8 fs){
    fontSize = fs;
}

void Graphics_mvprintw(u32 x, u32 y, char *str, int strLen){


    u32 k;

    // float fontSize = font_g.width / 16;
    x *= fontSize;
    y *= fontSize;

    char p;


    int j;

    for(j = 0; j < strLen && stringOffset < MAX_TEXT_CHARS*6; j++){

        p = str[j];

        float tX = (p % 16) / 16.0f;
        float tY = (1 - SS_CHAR_SIZE) - (floorf(p / 16.0f) / 16.0f);

        for(k = 0; k < 12; k+=2){


                ncursesuv[stringOffset][0] = ((RectTriangleVerts[k] * SS_CHAR_SIZE) + tX);
                ncursesuv[stringOffset][1] = 1 - ((RectTriangleVerts[k+1] * SS_CHAR_SIZE) + tY);
                ncursespos[stringOffset][0] = (RectTriangleVerts[k] * fontSize) + x;
                ncursespos[stringOffset][1] = ((1-RectTriangleVerts[k+1]) * fontSize) + y;
                ncursesfgs[stringOffset] = ncursesAttrPairs[currentNCursesPair][0];
                ncursesbgs[stringOffset] = ncursesAttrPairs[currentNCursesPair][1];
                ++stringOffset;
        }

        
        x += fontSize;
    }

}

void Graphics_RenderNCurses(){

    glUseProgram(shaders[NCURSES_SHADER].program);
    glUniform2f(shaders[NCURSES_SHADER].invViewportLoc, 1.0f/viewport.w, 1.0f/viewport.h); 


    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, font_g.texture);

    glBindVertexArray(ncursesVao_g);

    // use instancing.
    glCullFace(GL_FRONT);

    glBindBuffer(GL_ARRAY_BUFFER, ncursesPosVbo_g);
    glBufferData(GL_ARRAY_BUFFER, stringOffset*2*sizeof(u16), &ncursespos[0][0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, ncursesUvVbo_g);
    glBufferData(GL_ARRAY_BUFFER, stringOffset*2*sizeof(float), &ncursesuv[0][0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, ncursesColorFGVbo_g);
    glBufferData(GL_ARRAY_BUFFER, stringOffset, &ncursesfgs[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, ncursesColorBGVbo_g);
    glBufferData(GL_ARRAY_BUFFER, stringOffset, &ncursesbgs[0], GL_STATIC_DRAW);

    glDrawArrays(GL_TRIANGLES, 0, stringOffset);

    stringOffset = 0;
}

void Graphics_attron(u32 attr){
    currentNCursesPair = attr;
}

void Graphics_attroff(u32 attr){
}

void Graphics_init_pair(u8 pair, u8 bg, u8 fg){
    ncursesAttrPairs[pair][0] = fg-1;
    ncursesAttrPairs[pair][1] = bg-1;
}