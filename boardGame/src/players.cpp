#include "../../item.h"
#include <iostream>


class players : public animation
{
	public:
	players();
	void preloop();
	void update(int tix);
};



players::players()
{
}

void players::preloop()
{
	setSize(900, 100);
	setPos(0, 600);
	setFPS(20);
	std::cout << "Plugin preloop was called.\n";
	if(!(loadAnimation("res/playerss/outfile", "00000", ".png")))
	{
		std::cout << "Plugin Image load FAILED!!!!!!!!!!!.\n";
	}
}

void players::update(int ticks)
{
	animation::update(ticks);
}

extern "C" item * create()
{

	return new players;
}

extern "C" void destroy(item * done)
{
	
	delete(done);
	done = NULL;
}
