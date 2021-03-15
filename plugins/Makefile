all: main ext

main:
	g++ ../item.cpp main.cpp -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_net -ldl
ext: 
	g++ -rdynamic -fPIC ext.cpp ../item.cpp -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_net -shared -o ext.so
