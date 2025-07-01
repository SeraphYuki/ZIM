# wish i knew make, uncomment for library compile comment everything else
# EXECUTABLE=libthoth.a
# CC=gcc
# EXECUTABLE=libthoth.a
# CFLAGS = -g -Wall -lm -DLIBRARY_COMPILE -DLINUX_COMPILE  $(shell sdl2-config --cflags) $(shell pkg-config --cflags freetype2)
# 
# SOURCES=main.c text_editor.c window.c graphics.c log.c freetype.c file_browser.c config.c
# OBJECTS=$(SOURCES:.c=.o)
# 
# 
# all: $(SOURCES) $(EXECUTABLE)
# 
# $(EXECUTABLE): $(OBJECTS) 
# 	ar -rc $@ $(OBJECTS)
# 
# .c.o:
# 	$(CC) -c $(CFLAGS) $< -o $@
# 
# clean:
# 	rm *.o

# # linuxa
# CC=gcc
# EXECUTABLE=zim
# CFLAGS = -g -Wall -lm -DLINUX_COMPILE  $(shell sdl2-config --cflags) $(shell pkg-config --cflags freetype2) -DLINUX_INSTALL

# FREETYPELIBS = $(shell pkg-config --libs freetype2)
# GLEWLIBS = $(shell pkg-config --static --libs glew)
# SDLLIBS = $(shell pkg-config --libs sdl2)
# LDLIBS = -lm -lutil -static-libgcc $(GLEWLIBS) $(SDLLIBS) $(FREETYPELIBS)
# add -pg for gdb


# CC=mingw32-gcc
# EXECUTABLE=zim.exe

# FREETYPELIBS = -lfreetype
# GLEWLIBS = -lglew32 -lopengl32 -mwindows
# SDLLIBS = -lsdl2main -lsdl2
# LDLIBS = -lm -lmingw32 $(GLEWLIBS) $(SDLLIBS) $(FREETYPELIBS) 
# add -pg for gdb

# windows
 CC=mingw32-gcc
 EXECUTABLE=zim.exe
 CFLAGS = -DWINDOWS_COMPILE \
-IC:/MinGW/include/freetype2/ -IC:/MinGW/include/

 LDLIBS = -m32 -lmingw32 -lSDL3 -mwindows  -lm -lkernel32 -luser32 -lgdi32 -lwinmm -limm32 -lole32 -loleaut32 -lversion -luuid -ladvapi32 -lsetupapi -lshell32 -ldinput8 \
   -lglew32 -lglu32 -lopengl32 \
   -lfreetype -lz -lpng

# LIBS = -Wl,-Bdynamic -lmingw32 -lSDL2main -lSDL2 -lopengl32 -lglew32 -Wl,-Bstatic -lpng16 -lz -lm -Llib/freetype/lib/win_cb/ -lfreetype


SOURCES=main.c \
text_editor.c window.c graphics.c log.c freetype.c file_browser.c config.c
OBJECTS=$(SOURCES:.c=.o)


all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(OBJECTS) $(LDLIBS) -o $@

# wihndows unused right now
# all: createResourcesO $(SOURCES) $(EXECUTABLE)

# $(EXECUTABLE): $(OBJECTS) icon.o
# 	$(CC) $(OBJECTS) icon.o $(LDLIBS) -o $@

# createResourcesO: icon.rc
# 	windres icon.rc -o icon.o

.c.o:
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	rm *.o
