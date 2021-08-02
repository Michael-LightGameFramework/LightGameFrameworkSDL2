#include "../../item.h"
#include <iostream>


class wav : public animation
{
	public:
	wav();
	void preloop();
	void update(int tix);
};



wav::wav()
{
}

void wav::preloop()
{
	setSize(900, 30);
	setPos(0, 570);
	setFPS(10);
	std::cout << "Plugin preloop was called.\n";
	if(loadAnimation("res/waves/outfile", "00000", ".png"))
	{
		std::cout << "Image load success!!!!!!!!!!!.\n";
	}
	else
	{
		std::cout << "Image load FAILED!!!!!!!!!!!.\n";

	}
}

void wav::update(int ticks)
{
	animation::update(ticks);
}

extern "C" item * create()
{

	std::cout << "Plugin Created.\n";
	return new wav;
}

extern "C" void destroy(item * done)
{
	
	std::cout << "Plugin Destruction.\n";
	delete(done);
	done = NULL;
}
