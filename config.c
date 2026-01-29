#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "config.h"
#include "thoth.h"
#include "json.h"

static void readColor( JSON_Value *val,Thoth_Config *cfg, int index){
	int x = 0;
	if(!val->children ||  !val->children->string) return;
	sscanf(val->children->string,"%x", &x);	
	#ifndef SDL_COMPILE
	#ifdef  WINDOWS_COMPILE
		cfg->colors[index].r = (int)x << 16;
		cfg->colors[index].g = (int)(x << 8) & 0xFF;
		cfg->colors[index].b = x & 0xFF;
	#else
		cfg->colors[index].r = (x>>16)*1000/255.0f;
		cfg->colors[index].g = ((x>>8)&0xFF)*1000/255.0f;
		cfg->colors[index].b = (x&0xFF)*1000/255.0f;
	#endif
	#else
		cfg->colors[index].r = (x>>16)/255.0f;
		cfg->colors[index].g = ((x>>8)&0xFF)/255.0f;
		cfg->colors[index].b = (x&0xFF)/255.0f;
	#endif
}

	
static void ReadCommand(JSON_Value *val, unsigned int *command){
	*command = 0;
	val = val->children;
	if(!val) return;
	do{
		if(val->type == JSON_STRING) {
			if(strcmp(val->string, "CTRL") == 0) *command |= THOTH_CTRL_KEY;
			else if(strcmp(val->string, "SHIFT") == 0) *command |= THOTH_SHIFT_KEY;
			else if(strcmp(val->string, "ALT") == 0) *command |= THOTH_ALT_KEY;
			else if(strcmp(val->string, "ARROW_RIGHT") == 0) *command |= THOTH_ARROW_RIGHT;
			else if(strcmp(val->string, "ARROW_UP") == 0) *command |= THOTH_ARROW_UP;
			else if(strcmp(val->string, "ARROW_DOWN") == 0) *command |= THOTH_ARROW_DOWN;
			else if(strcmp(val->string, "ARROW_LEFT") == 0) *command |= THOTH_ARROW_LEFT;
			else if(strlen(val->string) == 1) *command |= val->string[0];
		}
		val = val->next;
	}while(val);
}
	
static void ConfigRead(JSON_Value *val, Thoth_Config *cfg){

	do{

		if(val->type == JSON_ARRAY){
			if(val->key){
				if(strcmp(val->key, "MakeCMD") == 0){
					if( val->children && val->children->string) 
					   sprintf(cfg->makecmd, "%s",val->children->string);
				} else if(strcmp(val->key, "ExpandSelectionWords_BACKWARD") == 0)
					ReadCommand(val, &cfg->keybinds[THOTH_ExpandSelectionWords_BACK]);
				else if(strcmp(val->key, "ExpandSelectionWords_FORWARD") == 0)
					ReadCommand(val, &cfg->keybinds[THOTH_ExpandSelectionWords_FORWARD]);
				else if(strcmp(val->key, "SelectAll") == 0)
					ReadCommand(val, &cfg->keybinds[THOTH_SelectAll]);
				else if(strcmp(val->key, "COLOR_CYAN") == 0)
				    readColor(val, cfg,  THOTH_COLOR_CYAN);
				else if(strcmp(val->key, "COLOR_RED") == 0)
				    readColor(val, cfg,  THOTH_COLOR_RED);
				else if(strcmp(val->key, "COLOR_YELLOW") == 0)
				    readColor(val, cfg,  THOTH_COLOR_YELLOW);
				else if(strcmp(val->key, "COLOR_BLUE") == 0)
				    readColor(val, cfg,  THOTH_COLOR_BLUE);
				else if(strcmp(val->key, "COLOR_GREEN") == 0)
				    readColor(val, cfg,  THOTH_COLOR_GREEN);
				else if(strcmp(val->key, "COLOR_MAGENTA") == 0)
				    readColor(val, cfg,  THOTH_COLOR_MAGENTA);
				else if(strcmp(val->key, "COLOR_WHITE") == 0)
				    readColor(val, cfg,  THOTH_COLOR_WHITE);
				else if(strcmp(val->key, "COLOR_BLACK") == 0)
				    readColor(val, cfg,  THOTH_COLOR_BLACK);
				else if(strcmp(val->key, "COLOR_GREY") == 0)
				    readColor(val, cfg,  THOTH_COLOR_GREY);
				else if(strcmp(val->key, "COLOR_BG") == 0)
				    readColor(val, cfg,  THOTH_COLOR_BG);
				else if(strcmp(val->key, "MoveLinesText_UP") == 0)
					ReadCommand(val, &cfg->keybinds[THOTH_MoveLinesText_UP]);
				else if(strcmp(val->key, "MoveLinesText_DOWN") == 0)
					ReadCommand(val, &cfg->keybinds[THOTH_MoveLinesText_DOWN]);
				else if(strcmp(val->key, "OpenFileBrowser") == 0)
					ReadCommand(val, &cfg->keybinds[THOTH_OpenFileBrowser]);
				else if(strcmp(val->key, "NewFile") == 0)
					ReadCommand(val, &cfg->keybinds[THOTH_NewFile]);
				else if(strcmp(val->key, "CloseFile") == 0)
					ReadCommand(val, &cfg->keybinds[THOTH_CloseFile]);
				else if(strcmp(val->key, "SwitchFile") == 0)
					ReadCommand(val, &cfg->keybinds[THOTH_SwitchFile]);
				else if(strcmp(val->key, "SaveAsFile") == 0)
					ReadCommand(val, &cfg->keybinds[THOTH_SaveAsFile]);
				else if(strcmp(val->key, "SaveFile") == 0)
					ReadCommand(val, &cfg->keybinds[THOTH_SaveFile]);
				else if(strcmp(val->key, "ToggleComment") == 0)
					ReadCommand(val, &cfg->keybinds[THOTH_ToggleComment]);
				else if(strcmp(val->key, "ToggleCommentMulti") == 0)
					ReadCommand(val, &cfg->keybinds[THOTH_ToggleCommentMulti]);
				else if(strcmp(val->key, "MoveBrackets") == 0)
					ReadCommand(val, &cfg->keybinds[THOTH_MoveBrackets]);
				else if(strcmp(val->key, "SelectBrackets") == 0)
					ReadCommand(val, &cfg->keybinds[THOTH_SelectBrackets]);
				else if(strcmp(val->key, "GotoLine") == 0)
					ReadCommand(val, &cfg->keybinds[THOTH_GotoLine]);
				else if(strcmp(val->key, "FindTextInsensitive") == 0)
					ReadCommand(val, &cfg->keybinds[THOTH_FindTextInsensitive]);
				else if(strcmp(val->key, "FindTextZim") == 0)
					ReadCommand(val, &cfg->keybinds[THOTH_FindTextZim]);
				else if(strcmp(val->key, "EventCtrlEnter") == 0)
					ReadCommand(val, &cfg->keybinds[THOTH_EventCtrlEnter]);
				else if(strcmp(val->key, "SelectNextWord") == 0)
					ReadCommand(val, &cfg->keybinds[THOTH_SelectNextWord]);
				else if(strcmp(val->key, "AddCursorCommand_UP") == 0)
					ReadCommand(val, &cfg->keybinds[THOTH_AddCursorCommand_UP]);
				else if(strcmp(val->key, "AddCursorCommand_DOWN") == 0)
					ReadCommand(val, &cfg->keybinds[THOTH_AddCursorCommand_DOWN]);
				else if(strcmp(val->key, "ExpandSelectionLines") == 0)
					ReadCommand(val, &cfg->keybinds[THOTH_ExpandSelectionLines]);
				else if(strcmp(val->key, "DeleteLine") == 0)
					ReadCommand(val, &cfg->keybinds[THOTH_DeleteLine]);
				else if(strcmp(val->key, "MoveByChars_BACKWARD") == 0)
					ReadCommand(val, &cfg->keybinds[THOTH_MoveByChars_BACK]);
				else if(strcmp(val->key, "MoveByChars_FORWARD") == 0)
					ReadCommand(val, &cfg->keybinds[THOTH_MoveByChars_FORWARD]);
				else if(strcmp(val->key, "MoveLines_UP") == 0)
					ReadCommand(val, &cfg->keybinds[THOTH_MoveLines_UP]);
				else if(strcmp(val->key, "MoveLines_DOWN") == 0)
					ReadCommand(val, &cfg->keybinds[THOTH_MoveLines_DOWN]);
				else if(strcmp(val->key, "MoveByWords_BACKWARD") == 0)
					ReadCommand(val, &cfg->keybinds[THOTH_MoveByWords_BACK]);
				else if(strcmp(val->key, "MoveByWords_FORWARD") == 0)
					ReadCommand(val, &cfg->keybinds[THOTH_MoveByWords_FORWARD]);
				else if(strcmp(val->key, "IndentLine_FORWARD") == 0)
					ReadCommand(val, &cfg->keybinds[THOTH_IndentLine_FORWARD]);
				else if(strcmp(val->key, "IndentLine_BACKWARD") == 0)
					ReadCommand(val, &cfg->keybinds[THOTH_IndentLine_BACK]);
				else if(strcmp(val->key, "ScrollScreen_UP") == 0)
					ReadCommand(val, &cfg->keybinds[THOTH_ScrollScreen_UP]);
				else if(strcmp(val->key, "ScrollScreen_DOWN") == 0)
					ReadCommand(val, &cfg->keybinds[THOTH_ScrollScreen_DOWN]);
				else if(strcmp(val->key, "Undo") == 0)
					ReadCommand(val, &cfg->keybinds[THOTH_Undo]);
				else if(strcmp(val->key, "Redo") == 0)
					ReadCommand(val, &cfg->keybinds[THOTH_Redo]);
				else if(strcmp(val->key, "Cut") == 0)
					ReadCommand(val, &cfg->keybinds[THOTH_Cut]);
				else if(strcmp(val->key, "Copy") == 0)
					ReadCommand(val, &cfg->keybinds[THOTH_Copy]);
				else if(strcmp(val->key, "Paste") == 0)
					ReadCommand(val, &cfg->keybinds[THOTH_Paste]);
			}
			else if(val->children)
				ConfigRead(val->children, cfg);
		}
		val = val->next;
	} while(val);
}

void Thoth_Config_Read(Thoth_Config *cfg){
	memset(cfg, 0, sizeof(Thoth_Config));
	struct {
		unsigned int r;
		unsigned int g;
		unsigned int b;
	} defaultColors[] = { 
		{ 0x8e, 0xc0 ,0x7c},
		{ 0xfb, 0x49 ,0x34},
		{ 0xfa, 0xbd ,0x2f},
		{ 0x83, 0xa5 ,0x98},
		{ 0xb8, 0xbb ,0x26},
		{ 0xd3, 0x86 ,0x9b},
		{ 0xeb, 0xdb ,0xb2},
		{ 0x28, 0x28 ,0x28},
		{ 0x92, 0x83 ,0x74},
		{ 0x28, 0x28 ,0x28 },
	};
	
	int k;
	for(k = 0; k < THOTH_NUM_COLORS; k++){
	#ifdef WINDOWS_COMPILE
		cfg->colors[k].r = (int)defaultColors[k].r << 16;
		cfg->colors[k].g = (int)defaultColors[k].g << 8;
		cfg->colors[k].b = defaultColors[k].b & 0xFF;
	#else
		cfg->colors[k].r = defaultColors[k].r*1000/255;
		cfg->colors[k].g = defaultColors[k].g*1000/255;
		cfg->colors[k].b = defaultColors[k].b*1000/255;
	#endif
	}

	#ifdef LINUX_COMPILE
	strcpy(cfg->makecmd, "make");
	#endif
	#ifdef WINDOWS_COMPILE
	strcpy(cfg->makecmd, "mingw32-make");
	#endif

	cfg->keybinds[THOTH_MoveLinesText_UP] = THOTH_CTRL_KEY|THOTH_SHIFT_KEY|THOTH_ARROW_UP;
	cfg->keybinds[THOTH_MoveLinesText_DOWN] = THOTH_CTRL_KEY|THOTH_SHIFT_KEY|THOTH_ARROW_DOWN;
	cfg->keybinds[THOTH_OpenFileBrowser] = THOTH_CTRL_KEY|THOTH_SHIFT_KEY|'o';
	cfg->keybinds[THOTH_OpenFileZim] = THOTH_CTRL_KEY|'o';
	cfg->keybinds[THOTH_Help] = THOTH_CTRL_KEY|THOTH_SHIFT_KEY|'m';
	cfg->keybinds[THOTH_NewFile] = THOTH_CTRL_KEY|'n';
	cfg->keybinds[THOTH_CloseFile] = THOTH_CTRL_KEY|'w';
	cfg->keybinds[THOTH_SwitchFile] = THOTH_CTRL_KEY|'p';
	cfg->keybinds[THOTH_SaveAsFile] = THOTH_CTRL_KEY|THOTH_SHIFT_KEY|'s';
	cfg->keybinds[THOTH_SaveFile] = THOTH_CTRL_KEY|'s';
	cfg->keybinds[THOTH_ToggleComment] = THOTH_CTRL_KEY|'/';
	cfg->keybinds[THOTH_ToggleCommentMulti] = THOTH_CTRL_KEY|THOTH_SHIFT_KEY|'/';
	cfg->keybinds[THOTH_MoveBrackets] = THOTH_CTRL_KEY|'m';
	cfg->keybinds[THOTH_SelectBrackets] = THOTH_CTRL_KEY|THOTH_SHIFT_KEY|'j';
	cfg->keybinds[THOTH_GotoLine] = THOTH_CTRL_KEY|'g';
	cfg->keybinds[THOTH_FindTextInsensitive] = THOTH_CTRL_KEY|'f';
	cfg->keybinds[THOTH_FindTextZim] = THOTH_CTRL_KEY|THOTH_SHIFT_KEY|'f';
	cfg->keybinds[THOTH_EventCtrlEnter] = THOTH_ENTER_KEY|THOTH_CTRL_KEY;
	cfg->keybinds[THOTH_SelectNextWord] = 	'd'|THOTH_CTRL_KEY;
	cfg->keybinds[THOTH_AddCursorCommand_UP] = THOTH_ARROW_UP|THOTH_CTRL_KEY;
	cfg->keybinds[THOTH_AddCursorCommand_DOWN] = THOTH_ARROW_DOWN|THOTH_CTRL_KEY;
	cfg->keybinds[THOTH_ExpandSelectionLines] = 	'l'|THOTH_SHIFT_KEY|THOTH_CTRL_KEY;
	cfg->keybinds[THOTH_DeleteLine] = 'k'|THOTH_SHIFT_KEY|THOTH_CTRL_KEY;
	cfg->keybinds[THOTH_MoveByChars_BACK] = 	'h'|THOTH_CTRL_KEY;
	cfg->keybinds[THOTH_MoveByChars_FORWARD] = 	'l'|THOTH_CTRL_KEY;
	cfg->keybinds[THOTH_MoveLines_UP] = 	'j'|THOTH_CTRL_KEY;
	cfg->keybinds[THOTH_MoveLines_DOWN] = 	'k'|THOTH_CTRL_KEY;
	cfg->keybinds[THOTH_MoveByWords_BACK] = 'h'|THOTH_ALT_KEY|THOTH_CTRL_KEY;
	cfg->keybinds[THOTH_MoveByWords_FORWARD] = 'l'|THOTH_ALT_KEY|THOTH_CTRL_KEY;
	cfg->keybinds[THOTH_IndentLine_FORWARD] = ']'|THOTH_CTRL_KEY;
	cfg->keybinds[THOTH_IndentLine_BACK] = '['|THOTH_CTRL_KEY;
	cfg->keybinds[THOTH_ExpandSelectionWords_BACK] = 'l'|THOTH_ALT_KEY|THOTH_SHIFT_KEY|THOTH_CTRL_KEY;
	cfg->keybinds[THOTH_ExpandSelectionWords_FORWARD] = 'h'|THOTH_ALT_KEY|THOTH_SHIFT_KEY|THOTH_CTRL_KEY;
	cfg->keybinds[THOTH_ScrollScreen_UP] = THOTH_ARROW_UP|THOTH_SHIFT_KEY;
	cfg->keybinds[THOTH_ScrollScreen_DOWN] = THOTH_ARROW_DOWN|THOTH_SHIFT_KEY;
	cfg->keybinds[THOTH_SelectAll] = THOTH_CTRL_KEY|'a';
	cfg->keybinds[THOTH_Undo] = 	'z'|THOTH_CTRL_KEY;
	cfg->keybinds[THOTH_Redo] = 	'y'|THOTH_CTRL_KEY;
	cfg->keybinds[THOTH_Cut] = 	'x'|THOTH_CTRL_KEY;
	cfg->keybinds[THOTH_Copy] = 	'c'|THOTH_CTRL_KEY;
	cfg->keybinds[THOTH_Paste] ='v'|THOTH_CTRL_KEY;

	memcpy(&cfg->colorPairs[THOTH_COLOR_SIDE_NUMBERS], (int[]){ THOTH_COLOR_WHITE, THOTH_COLOR_BLACK }, sizeof(int)*2);
	memcpy(&cfg->colorPairs[THOTH_COLOR_NORMAL], (int[]){ THOTH_COLOR_WHITE, THOTH_COLOR_BLACK }, sizeof(int)*2);
	memcpy(&cfg->colorPairs[THOTH_COLOR_KEYWORD], (int[]){ THOTH_COLOR_CYAN, THOTH_COLOR_BLACK }, sizeof(int)*2);
	memcpy(&cfg->colorPairs[THOTH_COLOR_COMMENT], (int[]){ THOTH_COLOR_GREY, THOTH_COLOR_BLACK }, sizeof(int)*2);
	memcpy(&cfg->colorPairs[THOTH_COLOR_TOKEN], (int[]){ THOTH_COLOR_GREEN, THOTH_COLOR_BLACK }, sizeof(int)*2);
	memcpy(&cfg->colorPairs[THOTH_COLOR_NUM], (int[]){ THOTH_COLOR_RED, THOTH_COLOR_BLACK }, sizeof(int)*2);
	memcpy(&cfg->colorPairs[THOTH_COLOR_FUNCTION], (int[]){ THOTH_COLOR_YELLOW, THOTH_COLOR_BLACK }, sizeof(int)*2);
	memcpy(&cfg->colorPairs[THOTH_COLOR_STRING], (int[]){ THOTH_COLOR_MAGENTA, THOTH_COLOR_BLACK }, sizeof(int)*2);
	memcpy(&cfg->colorPairs[THOTH_COLOR_SELECTED], (int[]){ THOTH_COLOR_BLACK ,THOTH_COLOR_GREEN }, sizeof(int)*2);
	memcpy(&cfg->colorPairs[THOTH_COLOR_SELECTED_DIRECTORY], (int[]){ THOTH_COLOR_RED ,THOTH_COLOR_CYAN }, sizeof(int)*2);
	memcpy(&cfg->colorPairs[THOTH_COLOR_UNSELECTED_DIRECTORY], (int[]){ THOTH_COLOR_RED ,THOTH_COLOR_WHITE }, sizeof(int)*2);
	memcpy(&cfg->colorPairs[THOTH_COLOR_AUTO_COMPLETE], (int[]){ THOTH_COLOR_BLACK, THOTH_COLOR_WHITE }, sizeof(int)*2);
	memcpy(&cfg->colorPairs[THOTH_COLOR_LOG_UNSELECTED], (int[]){ THOTH_COLOR_BLACK, THOTH_COLOR_WHITE }, sizeof(int)*2);
	memcpy(&cfg->colorPairs[THOTH_COLOR_CURSOR], (int[]){ THOTH_COLOR_BLACK ,THOTH_COLOR_MAGENTA }, sizeof(int)*2);
	memcpy(&cfg->colorPairs[THOTH_COLOR_FIND], (int[]){ THOTH_COLOR_BLACK ,THOTH_COLOR_WHITE }, sizeof(int)*2);
	memcpy(&cfg->colorPairs[THOTH_COLOR_LINE_NUM], (int[]){ THOTH_COLOR_WHITE ,THOTH_COLOR_BLACK }, sizeof(int)*2);
	memcpy(&cfg->colorPairs[THOTH_COLOR_LINENUM_CURSOR], (int[]){ THOTH_COLOR_GREEN ,THOTH_COLOR_BLACK }, sizeof(int)*2);
	memcpy(&cfg->colorPairs[THOTH_TE_COLOR_BLACK], (int[]){ THOTH_COLOR_BLACK ,THOTH_COLOR_WHITE }, sizeof(int)*2);
	memcpy(&cfg->colorPairs[THOTH_TE_COLOR_WHITE], (int[]){ THOTH_COLOR_WHITE ,THOTH_COLOR_BLACK }, sizeof(int)*2);
	memcpy(&cfg->colorPairs[THOTH_TE_COLOR_CYAN], (int[]){ THOTH_COLOR_CYAN ,THOTH_COLOR_BLACK }, sizeof(int)*2);
	memcpy(&cfg->colorPairs[THOTH_TE_COLOR_RED], (int[]){ THOTH_COLOR_RED ,THOTH_COLOR_BLACK }, sizeof(int)*2);
	memcpy(&cfg->colorPairs[THOTH_TE_COLOR_YELLOW], (int[]){ THOTH_COLOR_YELLOW ,THOTH_COLOR_BLACK }, sizeof(int)*2);
	memcpy(&cfg->colorPairs[THOTH_TE_COLOR_BLUE], (int[]){ THOTH_COLOR_BLUE ,THOTH_COLOR_BLACK }, sizeof(int)*2);
	memcpy(&cfg->colorPairs[THOTH_TE_COLOR_GREEN], (int[]){ THOTH_COLOR_GREEN ,THOTH_COLOR_BLACK }, sizeof(int)*2);
	memcpy(&cfg->colorPairs[THOTH_TE_COLOR_MAGENTA], (int[]){ THOTH_COLOR_MAGENTA ,THOTH_COLOR_BLACK }, sizeof(int)*2);

	cfg->tabs = DEFAULT_TAB_WIDTH;

	FILE *fp = fopen(THOTH_CONFIG_FILE,"rb");
	void *stack = malloc(2048<<1);
	void *stackEnd = stack + (2048<<1);
	JSON_Value *top;
	fseek(fp,0,SEEK_END);
	int len = ftell(fp);

	rewind(fp);
	char *memory = malloc(len+1);
	fread(memory,1,len,fp);
		
	JSON_Parse(&top,memory,len,stack,stackEnd,16);

	ConfigRead(top, cfg);	

	free(stack);

}
