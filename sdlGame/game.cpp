#include <game.h>

game::game(SDL_Renderer * screen)
{
	ren = screen;
	// these are all entry points, in order of priority given
	levels["test"] = NULL;
	levels["start"] = NULL;
	levels["level1"] = NULL;
	levels["overworld"] = NULL;
}

game::~game()
{
	
}

void game::addLevel(std::string filePath)
{
	if(filePath[0] != '#')
	{
		// let's get rid of leading whitespaces
		while(filePath[0] == ' ' || filePath[0] == '\t')
		{
			filePath = filePath.substr(1);
		}
		if(filePath.empty())
		{
			return;
		}
		filePath = absPath() + filePath;
		void * hndl = SDL_LoadObject(filePath.c_str());
		if(!(hndl))
		{
			if(filePath != absPath() + "test.so")
			{
				SDL_Log(SDL_GetError());
				std::cout << "failed to load " << filePath << std::endl;
			}
			// else just ignore the lack of test.so
			return;
		}
		handle.push_back(hndl);
		spawner.push_back((spawnType)SDL_LoadFunction((void*)hndl, "spawn"));
		destroyer.push_back((destroyType)SDL_LoadFunction((void *) hndl, "destroy"));
		if(spawner[spawner.size() - 1])
		{
			board* temp = spawner[spawner.size() - 1](ren);
			vlevels.push_back(temp);
			levels[temp->name] = temp;
		}
		else
		{
			std::cout << "Didn't find spawner in " << filePath << std::endl;
		}
	}
}

void game::init()
{
	if(!(currentLvl))
	{
		std::string cursor = levels.begin()->first;
		if(levels[cursor])
		{
			currentLvl = levels[cursor];
		}

	}
	if(levels["overworld"])
	{
		levels["start"] = levels["overworld"];
		levels["default"] = levels["overworld"];
		levels[""] = levels["overworld"];
	}
	if(levels["level1"])
	{
		levels["start"] = levels["level1"];
	}
	if(levels["start"])
	{
		currentLvl = levels["start"];
	}
	if(levels["test"])
	{
		currentLvl = levels["test"];
	}
	if(currentLvl)
	{
		currentLvl->init();
	}
	else
	{
		std::cout << "Game::init; currentLvl is null, please put a viable level in packages\n";
	}
}

void game::gameLoop()
{
	bool run = true;
	if(!currentLvl)
	{
		std::cout << "Current level was NULL at gameLoop()\n";
		run = false;
	}
	while(run)
	{
		int startLoop = SDL_GetTicks();
		SDL_Event ev;
		while(SDL_PollEvent(&ev))
		{
			switch(ev.type)
			{
				case SDL_QUIT:
					run = false;
					break;
				case SDL_KEYDOWN:
					switch(ev.key.keysym.sym)
				case SDLK_w:
						run = false;
					break;
				case SDLK_SPACE:
					currentLvl->togglePause();
					break;

			}
			currentLvl->handleEvent(&ev);
		}
		currentLvl->update(startLoop);
		if(currentLvl->fin)
		{
			if(currentLvl->request.empty())
			{
				currentLvl = levels["overworld"];
			}
			else if(currentLvl != NULL)
			{
				currentLvl = levels[currentLvl->request];
			}
			else
			{
				SDL_Log("The requested level named %s was not loaded, but was requested by %s. Please check packages is correctly configured\n", currentLvl->request, currentLvl->name);
				run = false;
			}

		}
		SDL_RenderClear(ren);
		currentLvl->draw();
		SDL_RenderPresent(ren);
	}
	clear();
}


void game::clear()
{
	if(!(vlevels.empty()))
	{
		for(int i = 0; i < vlevels.size(); i ++)
		{
			destroyer[i](vlevels[i]);
			SDL_UnloadObject((void*)handle[i]);
		}
		destroyer.clear();
		vlevels.clear();
	}
}
	


