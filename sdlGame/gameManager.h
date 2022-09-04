#ifndef GAMEMANAGERFOROOP
#define GAMEMANAGERFOROOP

#include <item.h>

class game : public item
{
	public:
	game(SDL_Window * win);
	void changeBoard(board * next);
	void handleEvent(SDL_Event * ev);
	void update(int tick);


	public:
	unordered_map<std::string, board *> levels;
	board * currentLevel;

}


#endif
