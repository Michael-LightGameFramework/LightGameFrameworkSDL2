#include "../item.h"

#include <cstdlib>
#include <ctime>
#include <iostream>

typedef item * fnPrototype(); // fnPrototype ==> item * fnPrototype();
typedef void fnDestroy(item *); // fnDestroy ==> void fnDestroy(item *);

int main()
{
	srand(time(NULL));
	itemInit();

	SDL_Renderer * screen = NULL;
	SDL_Window * wind = NULL;
	wind = SDL_CreateShapedWindow("Pluggy", 100, 100, 800, 800, SDL_WINDOW_SHOWN);
	screen = SDL_CreateRenderer(wind, -1, 0);

	SDL_Surface * pluggy = IMG_Load("plug800.png");
	SDL_SetWindowShape(wind, pluggy, NULL);

	item bkg(screen, "valley.jpg");

	// load
	
	void * lib = NULL;
	lib = SDL_LoadObject("./ext.so");
	if(lib == NULL)
	{
		std::cout << "Failed to load library:\n";
		std::cout << SDL_GetError() << '\n';
		itemQuit();
		exit(0);
	}
	else 
	{
		std::cout << "Success load Lib!!!\n";
	}
	fnPrototype * ext = NULL;
	fnDestroy * extDest = NULL;
	ext = (fnPrototype *) SDL_LoadFunction(lib, "create");
	extDest = (fnDestroy *) SDL_LoadFunction(lib, "destroy");


	if(ext == NULL)
	{
		std::cout << "But failed to load function...\n";
	}
	item * bobby = ext();
	bobby->preloop();

	bool run = true;
	while(run)
	{
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
			}
		}
		SDL_RenderClear(screen);
		bkg.draw();

		SDL_RenderPresent(screen);

		SDL_Delay(40);

	}
		

	extDest(bobby);

	SDL_UnloadObject(lib);
	SDL_DestroyWindow(wind);
	itemQuit();
}
