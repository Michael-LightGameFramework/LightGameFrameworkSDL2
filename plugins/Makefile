all: main ext waves template dual ducky

main:
	g++ ../item.cpp src/main.cpp -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_net -ldl

ext: 
	g++ -rdynamic -fPIC src/ext.cpp ../item.cpp -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_net -shared -o plugins/ext.so

waves:
	g++ -rdynamic -fPIC src/waves.cpp ../item.cpp -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_net -shared -o plugins/waves.so

template:
	g++ -rdynamic -fPIC src/template.cpp ../item.cpp -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_net -shared -o plugins/template.so

dual:
	g++ -rdynamic -fPIC src/dual.cpp ../item.cpp -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_net -shared -o plugins/dual.so

fog:
	g++ -rdynamic -fPIC src/fog.cpp ../item.cpp -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_net -shared -o plugins/fog.so

ducky:
	g++ -rdynamic -fPIC src/ducky.cpp ../item.cpp -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_net -shared -o plugins/ducky.so


