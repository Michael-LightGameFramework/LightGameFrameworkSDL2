#include <game.h>

game::game(SDL_Renderer * screen)
{
	ren = screen;
}

game::~game()
{
	
}

void game::addLevel(std::string filePath)
{
	if(filePath.empty())
	{
		std::cout << "Tried to load empty string as plugin\n";
		return;
	}
	void * hndl = SDL_LoadObject(filePath.c_str());
	if(!(hndl))
	{
		SDL_Log(SDL_GetError());
		std::cout << "failed to load " << filePath << std::endl;
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

void game::init()
{
	//levels["start"] = levels["level1"];
	 levels["start"] = levels["overworld"];
	levels["default"] = levels["overworld"];
	levels[""] = levels["overworld"];
	currentLvl = levels["start"];
	if(currentLvl)
	{
		currentLvl->init();
	}
	else
	{
		std::cout << "Game::init; currentLvl is null\n";
	}
}

void game::gameLoop()
{
	bool run = true;
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
			else
			{
				currentLvl = levels[currentLvl->request];
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
	


