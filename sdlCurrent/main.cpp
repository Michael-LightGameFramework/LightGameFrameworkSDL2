#include "item.h"

int main(int argc, char ** arg)
{
	itemInit();
	SDL_Window * win = SDL_CreateWindow("title", 30, 30, 1600, 900, SDL_WINDOW_SHOWN | SDL_WINDOW_FULLSCREEN_DESKTOP);
	SDL_Renderer * screen = SDL_CreateRenderer(win, -1, SDL_RENDERER_PRESENTVSYNC);
	SDL_Rect windowSize{0};
	SDL_GetWindowSize(win, &windowSize.w, &windowSize.h);
	SDL_SetRenderDrawColor(screen, 30, 180, 20, 255);
	item bkg;
	bkg.setRenderer(screen);
	bkg.setPos(0,0);
	bkg.setSize(windowSize.w, windowSize.h);
	bkg.loadImage("bkg.png");

	label out[] = {label(screen), label(screen), label(screen)};
	out[0].setText("Hit w to close the window");
	out[1].setText("or space to pause");
	out[2].setText("Up button plays a \'hah\' sound");

	out[1].move(0, out[1].getPos()->h);
	out[2].move(0, out[1].getPos()->h * 2);


	Mix_Music * music = Mix_LoadMUS("BlueDanube.mp3");
	Mix_PlayMusic(music, 0);

	Mix_Chunk * jumpSound = Mix_LoadWAV("jump.mp3");
	Mix_Volume(2, 50);
	Mix_VolumeChunk(jumpSound, 70);
	// Mix_MasterVolume(100);

	bool run = true;
	while(run)
	{
		Uint64 startLoop = SDL_GetTicks64();
		SDL_Event ev;
		while(SDL_PollEvent(&ev))
		{
			switch(ev.type)
		{
				case SDL_QUIT:
					run = false;
					break;
				case SDL_MOUSEBUTTONUP:
					break;
				case SDL_KEYDOWN:
					switch(ev.key.keysym.sym)
					{
						case SDLK_UP:
							Mix_PlayChannel(2, jumpSound, 0); 
							break;
						case SDLK_DOWN:
							break;
						case SDLK_RIGHT:
							break;
						case SDLK_LEFT:
							break;
						case SDLK_SPACE:
							if(Mix_PausedMusic())
							{
								Mix_ResumeMusic();
							}
							else
							{
								Mix_PauseMusic();
							}
							break;
						case SDLK_w:
							run = false;
							break;
					}

					break;
				case SDL_KEYUP:
					switch(ev.key.keysym.sym)
					{
						case SDLK_UP:
							break;
						case SDLK_DOWN:
							break;
						case SDLK_RIGHT:
							break;
						case SDLK_LEFT:
							break;
					}
					


					break;

			}
		}

		SDL_RenderClear(screen);
		bkg.draw();
		out[0].draw();
		out[1].draw();
		out[2].draw();
		SDL_RenderPresent(screen);

	}

	Mix_HaltMusic();
	Mix_FreeChunk(jumpSound);
	Mix_FreeMusic(music);
	SDL_DestroyWindow(win);
	itemQuit();

}
