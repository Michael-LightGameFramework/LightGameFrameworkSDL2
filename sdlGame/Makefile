LIBS = -lSDL2 -lSDL2_image -lSDL2_mixer -I./
all:
	g++ -march=haswell -Ofast main.cpp game.cpp item.cpp -lSDL2 -lSDL2_image -lSDL2_mixer -I./
	
plugins: start overworld all
start:
	g++ -rdynamic -fpic -shared -o start.so item.cpp level1_arrival.cpp $(LIBS)
overworld:
	g++ -rdynamic -fpic -shared -o overworld.so item.cpp overworld.cpp $(LIBS)
