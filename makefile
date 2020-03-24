CC=g++
CFLAGS=-c -Wall
SDLFLAGS=-lSDLmain -lSDL -lSDL_image -lSDL_ttf -lSDL_gfx
WXFLAGS=`wx-config --libs` `wx-config --cxxflags`
XMLFLAGS=`xml2-config --cflags --libs`
OLDFLAGS=-lxml2 -lwsock32
SRC=src/

all: hello

hello: main.o map_editor.o helpful_functions.o game.o timer.o rect.o block.o spikes.o map_point.o sdl_functions.o player.o zone.o iomap.o map.o mini_map.o button.o wx_app.o widgets.o
	$(CC) main.o map_editor.o helpful_functions.o game.o timer.o rect.o block.o spikes.o map_point.o sdl_functions.o player.o zone.o iomap.o map.o mini_map.o button.o wx_app.o widgets.o $(SDLFLAGS) $(XMLFLAGS) $(WXFLAGS) -o hello

main.o: $(SRC)main.cpp
	$(CC) $(SRC)main.cpp $(CFLAGS)

map_editor.o: $(SRC)map_editor.cpp $(SRC)map_editor.h
	$(CC) $(SRC)map_editor.cpp $(CFLAGS) $(SDLFLAGS)

helpful_functions.o: $(SRC)helpful_functions.cpp $(SRC)helpful_functions.h
	$(CC) $(SRC)helpful_functions.cpp $(CFLAGS)

button.o: $(SRC)button.cpp $(SRC)button.h
	$(CC) $(SRC)button.cpp $(CFLAGS) $(SDLFLAGS)

game.o: $(SRC)game.cpp $(SRC)game.h
	$(CC) $(SRC)game.cpp $(CFLAGS) $(SDLFLAGS)

timer.o: $(SRC)timer.cpp $(SRC)timer.h
	$(CC) $(SRC)timer.cpp $(CFLAGS)

rect.o: $(SRC)rect.cpp $(SRC)rect.h
	$(CC) $(SRC)rect.cpp $(CFLAGS)

block.o: $(SRC)block.cpp $(SRC)block.h
	$(CC) $(SRC)block.cpp $(CFLAGS) $(SDLFLAGS)

spikes.o: $(SRC)spikes.cpp $(SRC)spikes.h
	$(CC) $(SRC)spikes.cpp $(CFLAGS) $(SDLFLAGS)

map_point.o: $(SRC)map_point.cpp $(SRC)map_point.h
	$(CC) $(SRC)map_point.cpp $(CFLAGS)

sdl_functions.o: $(SRC)sdl_functions.cpp $(SRC)sdl_functions.h
	$(CC) $(SRC)sdl_functions.cpp $(CFLAGS) $(SDLFLAGS)

player.o: $(SRC)player.cpp $(SRC)player.h
	$(CC) $(SRC)player.cpp $(CFLAGS) $(SDLFLAGS)

zone.o: $(SRC)zone.cpp $(SRC)zone.h
	$(CC) $(SRC)zone.cpp $(CFLAGS) $(SDLFLAGS)

iomap.o: $(SRC)iomap.cpp $(SRC)iomap.h
	$(CC) $(SRC)iomap.cpp $(CFLAGS) $(XMLFLAGS)

mini_map.o: $(SRC)mini_map.cpp $(SRC)mini_map.h
	$(CC) $(SRC)mini_map.cpp $(CFLAGS) $(SDLFLAGS)

map.o: $(SRC)map.cpp $(SRC)map.h
	$(CC) $(SRC)map.cpp $(CFLAGS) $(SDLFLAGS)

wx_app.o: $(SRC)wx_app.cpp $(SRC)wx_app.h
	$(CC) $(SRC)wx_app.cpp $(CFLAGS) $(WXFLAGS)

widgets.o: $(SRC)widgets.cpp $(SRC)widgets.h
	$(CC) $(SRC)widgets.cpp $(CFLAGS) $(WXFLAGS)

clean: 
	rm -rf *.o hello
