CC=gcc

CFLAGS = -g -Wall -lm  $(shell pkg-config --cflags sdl2) $(shell pkg-config --cflags freetype2)
 


# GTKLIBS = $(shell pkg-config --libs gtk+-3.0)
FREETYPELIBS = $(shell pkg-config --libs freetype2)
GLEWLIBS = $(shell pkg-config --static --libs glew)
SDLLIBS = $(shell pkg-config --libs sdl2)
PNGLIBS = $(shell pkg-config --libs libpng)
# NCURSELIBS = $(shell pkg-config --static --libs ncurses)
# XLIBS       = $(shell pkg-config --libs x11)
# XLIBS       = $(shell pkg-config --libs glew)
# XLIBS      ?= $(shell pkg-config --libs libva-x11)
# XLIBS      ?= $(shell pkg-config --libs x11-xcb)

LDLIBS =-lm -pg -static-libgcc $(GLEWLIBS) $(SDLLIBS) $(PNGLIBS) $(FREETYPELIBS)

SOURCES=main.c text_editor.c window.c graphics.c log.c utils.c freetype.c

OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=thoth

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(OBJECTS) $(LDLIBS) -o $@

.c.o:
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	rm *.o