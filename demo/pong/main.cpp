// A simple pong-like game and proof of concept.
// To Do:
// 	Flip animation for both ninjaGirl and dragon's direction
// 	Fix 'dragon grab'
// 	Allow user to choose characters
// 	Add an attack feature that changes ball's speed/direction
// 	Scoring system

#include "item.h"
void saveScreen(const char* file_name, SDL_Renderer* renderer) {
    SDL_Surface* surface = SDL_CreateRGBSurface(0, 2048, 1152, 32, 0, 0, 0, 0);
    SDL_RenderReadPixels(renderer, NULL, surface->format->format, surface->pixels, surface->pitch);
    IMG_SavePNG(surface, file_name);
    SDL_FreeSurface(surface);
}

int main(int argc, char ** arg)
{
	itemInit();
	SDL_Window * win = SDL_CreateWindow("title", 30, 30, 2048, 1152, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
	SDL_Renderer * screen = SDL_CreateRenderer(win, -1, 0);
	SDL_Texture * target = SDL_GetRenderTarget(screen); 

	item bkg;
	int x, y;
	SDL_GetWindowSize(win, &x, &y);
	bkg.setRenderer(screen);
	bkg.setSize(x, y);
	bkg.setPos(0,0);
	bkg.loadImage("battleback1.png");

	animation first;
	first.setRenderer(screen);
	first.setCenter(75, 100, 120);
	first.setSize(150, 240);
	first.setPos(90, 500);
	first.loadAnimation("transparent PNG/run/frame-", "1", ".png");

	animation second;
	second.setRenderer(screen);
	second.setCenter(75, 100, 120);
	second.setSize(150, 240);
	second.setPos(1520, 450);
	second.loadAnimation("png/Run__", "000", ".png");

	animation ball;
	ball.setRenderer(screen);
	ball.setCenter(25, 25, 12);
	ball.setSize(150,150);
	ball.setPos(980, 550);
	ball.loadAnimation("PNG/frame-", "1", ".png");

	bool run = true;
	SDL_SetRenderDrawColor(screen, 30, 180, 20, 255);
	int fps = 8;
	int desiredDelta = 1000 / fps;
	int speedy = 0;
	int speedy2 = 0;
	int ballSpeedX = -14;
	int ballSpeedY = 15;
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
							speedy2 = -12;
							break;
						case SDLK_DOWN:
							speedy2 = 12;
							break;
						case SDLK_w:
							speedy = -12;
							break;
						case SDLK_s:
							speedy = 12;
							break;
					}

					break;
				case SDL_KEYUP:
					switch(ev.key.keysym.sym)
					{
						case SDLK_UP:
							speedy2 = 0;
							break;
						case SDLK_DOWN:
							speedy2 = 0;
							break;
						case SDLK_w:
							speedy = 0;
							break;
						case SDLK_s:
							speedy = 0;
					}
					break;
				case SDL_WINDOWEVENT:

					if(ev.window.event == SDL_WINDOWEVENT_RESIZED)
					{
						x = ev.window.data1;
						y = ev.window.data2;
						bkg.setSize(x, y);
						second.setPos(x - (2*second.getPos()->w), second.getPos()->y);
					}

					break;

			}
		}
		first.move(0, speedy);
		second.move(0, speedy2);
		ball.move(ballSpeedX, ballSpeedY);

		bkg.draw();
		first.draw();
		second.draw();
		ball.draw();

		SDL_RenderPresent(screen);

		first.next();
		second.next();
		ball.next();
		if(first.getCollision(&ball) || second.getCollision(&ball))
		{
			ballSpeedX *= -1;
		}
		if(ball.getPos()->y < 0 || ball.getPos()->y > y - ball.getPos()->h)
		{
			ballSpeedY *= -1;
		}
		if(ball.getPos()->x < 0)
		{
			// second player scores
			ball.setPos(x / 2, y / 2); 
			ballSpeedX *= -1;
		}
		if(ball.getPos()->x + ball.getPos()->w > x)
		{
			// second player scores
			ball.setPos(x / 2, y / 2); 
			ballSpeedX *= -1;
		}
		if(ball.getPos()->y > y)
		{
			ball.setPos(x / 2, y / 2);
		}

		int delta = SDL_GetTicks() - startLoop;
		if(delta < desiredDelta)
		{
			SDL_Delay(desiredDelta - delta);
		}
	}


	SDL_DestroyWindow(win);
	itemQuit();

}
