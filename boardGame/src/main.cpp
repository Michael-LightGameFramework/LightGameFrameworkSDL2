#include "../../item.h"

#include <cstdlib>
#include <ctime>
#include <iostream>
#include <vector>
#include <fstream>
#include <string>

typedef item * fnPrototype(); // fnPrototype ==> item * fnPrototype();
typedef void fnDestroy(item *); // fnDestroy ==> void fnDestroy(item *);

class group
{
	public:
	void preloop();
	bool ready();
	void draw();
	void destroy();
	void update(int tick);
	void handleEvent(SDL_Event * ev);
	void cleanup();


	std::vector <item*> pack;

};

void group::preloop()
{
	for(int i = 0; i < pack.size(); i ++)
	{
		if(pack[i])
		{
			pack[i]->preloop();
		}
	}

}

void group::draw()
{
	for(int i = 0; i < pack.size(); i ++)
	{
		if(pack[i])
		{
			pack[i]->draw();
		}
	}
}

void group::update(int tick)
{
	for(int i = 0; i < pack.size(); i ++)
	{
		if(pack[i])
		{
			pack[i]->update(tick);
		}
	}
}

void group::handleEvent(SDL_Event * ev)
{
	for(int i = 0; i < pack.size(); i ++)
	{
		if(pack[i])
		{
			pack[i]->handleEvent(ev);
		}
	}
}
void group::cleanup()
{
	for(int i = 0; i < pack.size(); i ++)
	{
		if(pack[i])
		{
			pack[i]->cleanup();
		}
	}
}

bool group::ready()
{
	int count = 0;
	for(int i = 0; i < pack.size(); i ++)
	{
		if(pack[i])
		{
			if(!(pack[i]->ready()))
			{
				i --;
				count ++;
				if(count > 400)
				{
					std::cout << "Lib " << i << "failed to prepare, recommend closing\n";
					return 0;
				}
			}

		}
		count ++;
	}
	return 1;
}

int main()
{
// Window Creation and setup shaping
	srand(time(NULL));
	itemInit();

	SDL_Renderer * screen = NULL;
	SDL_Window * wind = NULL;
	wind = SDL_CreateShapedWindow("Pluggy", 100, 100, 800, 800, SDL_WINDOW_SHOWN);
	screen = SDL_CreateRenderer(wind, -1, 0);

	SDL_Surface * pluggy = IMG_Load("plug800.png");
	SDL_SetWindowShape(wind, pluggy, NULL);

	item bkg(screen, "valley.jpg");

// Get list of plugins

	group plugins;
	std::vector <fnPrototype *> create;
	std::vector <fnDestroy *> destroy;
	std::vector <void *> libs;
	std::ifstream ifile("List.txt");
	int count = 0;
	while(ifile.good())
	{
		std::string temp;
		getline(ifile, temp);
		if(!(temp.empty()) && temp[0] != '#')
		{
			libs.push_back(NULL);
			libs[count] = SDL_LoadObject(temp.c_str());
			if(libs[count] == NULL)
			{
				count --;
				std::cout << "Library " << temp << ": Failed to load library:\n";
				std::cout << SDL_GetError() << '\n';
			}
			else 
			{
				std::cout << temp << " Success load Lib!!!\n";
				create.push_back(NULL);
				destroy.push_back(NULL);
				create[count] = (fnPrototype *) SDL_LoadFunction(libs[count], "create");
				destroy[count] = (fnDestroy *) SDL_LoadFunction(libs[count], "destroy");
	
	
				if(create[count] == NULL)
				{
					std::cout << temp << " But failed to load function...\n";
				}
				else
				{
					plugins.pack.push_back(create[count]());	
					plugins.pack[count]->setRenderer(screen);
				}
			}
			count ++;
		}
	}
	plugins.preloop();
	bool run = true;
	if(!(plugins.ready()))
	{
		//run = false;
	}

	while(run)
	{
		int startLoop = SDL_GetTicks();
		SDL_Event ev;
		while(SDL_PollEvent(&ev))
		{
			switch(ev.type)
			{
				case SDL_KEYUP:
				       if(ev.key.keysym.sym == SDLK_w)
				       {
						run = false;
				       }	       
				       break;
				case SDL_QUIT:
					run = false;
				       break;
			}
			plugins.handleEvent(&ev);
		}
		SDL_RenderClear(screen);
		bkg.draw();

		plugins.draw();
		SDL_RenderPresent(screen);
		plugins.update(startLoop);

		SDL_Delay(40);


	}
		
	plugins.cleanup();

	for(int i = 0; i < libs.size(); i ++)
	{
		SDL_UnloadObject(libs[i]);
	}

	SDL_DestroyWindow(wind);
	itemQuit();
}
