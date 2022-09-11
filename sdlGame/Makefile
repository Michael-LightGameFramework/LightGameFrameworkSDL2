LIBS = -lSDL2 -lSDL2_image -lSDL2_mixer -I./
plugins: a.out start.so overworld.so platformer.so
a.out:
	g++ -O0 main.cpp game.cpp item.cpp -lSDL2 -lSDL2_image -lSDL2_mixer -I./
	
start.so:
	g++ -O0 -rdynamic -fpic -shared -o start.so item.cpp level1_arrival.cpp $(LIBS)
overworld.so:
	g++ -O0 -rdynamic -fpic -shared -o overworld.so item.cpp overworld.cpp $(LIBS)
platformer.so:
	g++ -O0 -rdynamic -fpic -shared -o platformer.so item.cpp platformer.cpp $(LIBS)

clean:
	rm a.out platformer.so overworld.so start.so
