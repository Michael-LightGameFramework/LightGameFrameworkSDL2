#ifndef HEXBOARDWRAPPER
#define HEXBOARDWRAPPER
#include <item.h>

class hexBoard : public board
{
	public:
	hexBoard(SDL_Renderer * screen);
	~hexBoard();
	void init();
	void snapToGrid(item * tile);
	void setBoardSize(int w, int h);
	void moveCursorTo(int x, int y);
	void stamp();
	void update(int tick);
	void handleEvent(SDL_Event * ev);
	void draw();

	public:
	item * cursor;
	item * hovered;
	int srcIndex;
	int layerIndex;
	tilemap tileSrc;
	std::vector<group*> layers;
	bool snapping;
	bool stamping;
	int gridW;
	int gridH;

};


#endif


