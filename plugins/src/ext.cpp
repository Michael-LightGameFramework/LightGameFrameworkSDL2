#include "../../item.h"
#include <iostream>


class ext : public item
{
	public:
	ext();
	void preloop();
	void draw();
};



ext::ext()
{

}

void ext::preloop()
{
}

void ext::draw()
{
	SDL_Event ender;
	ender.type = SDL_QUIT;
	ender.quit.timestamp = 0;
	SDL_PushEvent(&ender);
}

extern "C" item * create()
{

	return new ext;
}

extern "C" void destroy(item * done)
{
	
	delete(done);
	done = NULL;
}
