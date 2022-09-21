#ifndef TESTITEMSWRAPPER
#define TESTITEMSWRAPPER
#include <item.h>
#include <platformer.h>


class landTest : public board
{

	public:
	landTest();
	landTest(SDL_Renderer * screen);
	~landTest();
	void init();
	void update(int tick);
	void draw();
	public:
	item * thing;
	item * moon;

};


#endif
