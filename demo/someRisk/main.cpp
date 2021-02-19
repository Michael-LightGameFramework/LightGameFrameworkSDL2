// Risk-like game

#include "item.h"

const int NUMSOLDIERS = 7;
const int NUMPLAYERS = 4;
const int NUMPADS = 12;

int main(int argc, char ** arg)
{
	itemInit();
	SDL_Window * win = SDL_CreateWindow("Dragon Pong", 30, 30, 2048, 1152, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
	SDL_Renderer * screen = SDL_CreateRenderer(win, -1, 0);

	bool run = true;
	SDL_SetRenderDrawColor(screen, 30, 180, 20, 255);
	int fps = 25;
	int desiredDelta = 1000 / fps;

	item bkg(screen, "res/map01.png");
	bkg.setSize(2048, 1152);
	bkg.setPos(0, 0);

	// load assets for up to 4 players
	item player[4];
	for(int i = 0; i < 4; i ++)
	{
		player[i].setRenderer(screen);
		player[i].setSize(100, 100);
	}
	player[0].loadImage("res/greenJester.png");
	player[1].loadImage("res/blueBerret.png");
	player[2].loadImage("res/redTricorn.png");
	player[3].loadImage("res/yellowSunhat.png");

	item p1[NUMPLAYERS][NUMSOLDIERS];
	for(int i = 0; i < NUMPLAYERS; i ++)
	{
		for(int j = 0; j < NUMSOLDIERS; j ++)
		{
			p1[i][j].setRenderer(screen);
			p1[i][j].setImage(player[i].getImage());
			p1[i][j].setPos(j * 100, i * 100);
		}
	}

	// load city pads
	item pads[NUMPADS];
	for(int i = 0; i < NUMPADS; i ++)
	{
		pads[i].setRenderer(screen);
		pads[i].loadImage("res/city01.png");
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
					{
						case SDLK_UP:
							break;
						case SDLK_DOWN:
							break;
						case SDLK_LEFT:
							break;
						case SDLK_RIGHT:
							break;
						case SDLK_w:
							break;
						case SDLK_s:
							break;
						case SDLK_a:
							break;
						case SDLK_d:
							break;
					}

					break;
				case SDL_KEYUP:
					switch(ev.key.keysym.sym)
					{
						case SDLK_UP:
						case SDLK_DOWN:
							break;
						case SDLK_LEFT:
						case SDLK_RIGHT:
							break;
						case SDLK_w:
						case SDLK_s:
							break;
						case SDLK_a:
						case SDLK_d:
							break;
					}
					break;
				case SDL_WINDOWEVENT:

					if(ev.window.event == SDL_WINDOWEVENT_RESIZED)
					{
						// x = ev.window.data1;
						// y = ev.window.data2;
					}
					break;
			}
		}

		bkg.draw();
		for(int i = 0; i < NUMPADS; i ++)
		{
			pads[i].draw();
		}
		for(int i = 0; i < NUMPLAYERS; i ++)
		{
			for(int j = 0; j < NUMSOLDIERS; j ++)
			{
				p1[i][j].draw();
			}

		}
		SDL_RenderPresent(screen);

		int delta = SDL_GetTicks() - startLoop;
		if(delta < desiredDelta)
		{
			SDL_Delay(desiredDelta - delta);
		}
	}

	SDL_DestroyWindow(win);
	itemQuit();
}
