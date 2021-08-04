all: main board players dice cards

main:
	g++ ../item.cpp src/main.cpp -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_net -ldl

template:
	g++ -rdynamic -fPIC src/template.cpp ../item.cpp -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_net -shared -o plugins/template.so



board:
	g++ -rdynamic -fPIC src/board.cpp ../item.cpp -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_net -shared -o plugins/board.so

cards:
	g++ -rdynamic -fPIC src/cards.cpp ../item.cpp -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_net -shared -o plugins/cards.so

players:
	g++ -rdynamic -fPIC src/players.cpp ../item.cpp -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_net -shared -o plugins/players.so

dice:
	g++ -rdynamic -fPIC src/dice.cpp ../item.cpp -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_net -shared -o plugins/dice.so




