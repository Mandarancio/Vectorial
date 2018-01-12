IDIR =/usr/include/SDL
CC=g++
CFLAGS=-I$(IDIR)

LIBS=-lm -lSDL -lSDL_gfx


vectorial: src/*.cpp
	$(CC) -o vectorial src/*.cpp src/graphics/*.cpp src/nanosvg/*.cpp src/vectorSVG/*.cpp $(CFLAGS) $(LIBS)