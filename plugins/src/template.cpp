#include "../../item.h"
#include <iostream>


class wave : public animation
{
	public:
	wave();
	void preloop();
	void update(int tix);
};



wave::wave()
{
}

void wave::preloop()
{
	setSize(900, 100);
	setPos(0, 600);
	setFPS(20);
	std::cout << "Plugin preloop was called.\n";
	if(!(loadAnimation("res/waves/outfile", "00000", ".png")))
	{
		std::cout << "Plugin Image load FAILED!!!!!!!!!!!.\n";
	}
}

void wave::update(int ticks)
{
	animation::update(ticks);
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
