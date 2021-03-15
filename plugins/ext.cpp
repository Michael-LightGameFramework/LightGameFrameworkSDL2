#include "../item.h"
#include <iostream>


class ext : public item
{
	public:
	ext();
	void preloop();
};



ext::ext()
{

}

void ext::preloop()
{
	std::cout << "Exit Plugin preloop was called.\n";
}

extern "C" item * create()
{

	std::cout << "Plugin Created.\n";
	return new ext;
}

extern "C" void destroy(item * done)
{
	
	std::cout << "Plugin Destruction.\n";
	delete(done);
	done = NULL;
}
