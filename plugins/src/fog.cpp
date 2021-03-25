#include "../../item.h"
#include <iostream>
#include <cstdlib>


class wave : public item
{
	public:
	wave();
	void preloop();
	void update(int tix);
	bool ready();

	private:
	bool loaded;
};



wave::wave()
{
	loaded = false;
}

void wave::preloop()
{
	srand(ID);
	setSize(3000, 800);
	setPos(800, 0);
	std::cout << "Plugin preloop was called.\n";
	if(!(loadImage("res/fog.png")))
	{
		std::cout << "Plugin Image load FAILED!!!!!!!!!!!.\n";
	}
	loaded = true;
}

void wave::update(int ticks)
{
	if(pos.x < -3000)
	{
		setPos(800, 0);
	}
	if(!(ticks % 2))
	{
		move(-4, 0);
	}
}

bool wave::ready()
{
	return loaded;
}

extern "C" item * create()
{

	return new wave;
}

extern "C" void destroy(item * done)
{
	
	delete(done);
	done = NULL;
}
