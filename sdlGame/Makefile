LIBS = -lSDL2 -lSDL2_image -lSDL2_mixer -I./
PHONEY = help
PREFIX = $(shell ./paths)
GAMENAME = "game"

plugins: $(GAMENAME) lvl1.so overworld.so platformer.so
all: clean cleanInstalled plugins

install: paths
	@echo 'Installing $(GAMENAME) to ~/bin'
	@echo ' other files and resources will be added to $(PREFIX)'
	cp -ruv * `./paths`
	cp -u $(GAMENAME) ~/bin/

paths:
	g++ -O0 installPaths.cpp item.cpp -lSDL2 -lSDL2_image -lSDL2_mixer -o paths -I./
	

$(GAMENAME):
	g++ -O0 main.cpp game.cpp item.cpp -lSDL2 -lSDL2_image -lSDL2_mixer -o $(GAMENAME) -I./
	
lvl1.so:
	g++ -O0 -rdynamic -fpic -shared -o lvl1.so item.cpp level1_arrival.cpp $(LIBS)
overworld.so:
	g++ -O0 -rdynamic -fpic -shared -o overworld.so item.cpp overworld.cpp $(LIBS)
platformer.so:
	g++ -O0 -rdynamic -fpic -shared -o platformer.so item.cpp platformer.cpp $(LIBS)

test:
	g++ -O0 -rdynamic -fpic -shared -o test.so landTest.cpp item.cpp platformer.cpp $(LIBS)
sticks:
	g++ -O0 -rdynamic -fpic -shared -o test.so stickman.cpp item.cpp platformer.cpp $(LIBS)
hexboard:
	g++ -O0 -rdynamic -fpic -shared -o test.so hexBoard.cpp item.cpp platformer.cpp $(LIBS)

clean:
	@rm -f a.out game paths platformer.so overworld.so lvl1.so test.so
cleanInstalled:
	@rm -f $(PREFIX)a.out $(PREFIX)game $(PREFIX)paths $(PREFIX)platformer.so $(PREFIX)overworld $(PREFIS)lvl1.so $(PREFIX)test.so

help:
	@echo 'Remove all compiled files: '
	@echo '  make clean           - removes all build files'
	@echo '  make cleanInstalled           - removes the installed build files'
	@echo 'Building the Game:     '
	@echo '  make                 - run a basic make, only rebuild if files are not pressent'
	@echo '  make plugins         - same as calling make'
	@echo '  make all             - removes all built files, then rebuilds everything'
	@echo 'Installation:'
	@echo '  make install         - installs to ~/bin and $(PREFIX) '
	@echo 'Playing the game: '
	@echo '    after installing, just call $(GAMENAME)'
	@echo 'Plugins: '
	@echo '    Plugins can be added by adding their path to the (packages) file, however'
	@echo '    Plugins added to packages will not run untill the name of that plugin is requested '
	@echo '    by an exiting board. Usually from the overworld board, though chaining is possible.'
	@echo '  Test Plugins: You can ask for a level to load immediately by adding it to the '
	@echo '    packages file and set the board variable (name) to text string (test)'

PHONEY += test
PHONEY += hexboard
PHONEY += clean
PHONEY += install


PHONEY: $(PHONEY)

