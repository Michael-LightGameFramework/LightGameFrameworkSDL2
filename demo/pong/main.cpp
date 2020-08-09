// A simple pong-like game and proof of concept.
// To Do:
// 	Flip animation for both ninjaGirl and dragon's direction
// 	Fix 'dragon grab'
// 	Allow user to choose characters
// 	Add an attack feature that changes ball's speed/direction
// 	Scoring system

#include "item.h"

int main(int argc, char ** arg)
{
	itemInit();
	SDL_Window * win = SDL_CreateWindow("title", 30, 30, 600, 500, SDL_WINDOW_SHOWN);
	SDL_Renderer * screen = SDL_CreateRenderer(win, -1, 0);

	item bkg;
	bkg.setRenderer(screen);
	bkg.setSize(600, 500);
	bkg.setPos(0,0);
	bkg.loadImage("battleback1.png");

	animation first;
	first.setRenderer(screen);
	first.setCenter(25, 40, 25);
	first.setSize(50,80);
	first.setPos(30, 180);
	first.loadAnimation("transparent PNG/run/frame-", "1", ".png");

	animation second;
	second.setRenderer(screen);
	second.setCenter(25, 40, 25);
	second.setSize(50, 80);
	second.setPos(520, 250);
	second.loadAnimation("png/Run__", "000", ".png");

	animation ball;
	ball.setRenderer(screen);
	ball.setCenter(25, 25, 12);
	ball.setSize(50,50);
	ball.setPos(280, 250);
	ball.loadAnimation("PNG/frame-", "1", ".png");

	bool run = true;
	SDL_SetRenderDrawColor(screen, 30, 180, 20, 255);
	int fps = 8;
	int desiredDelta = 1000 / fps;
	int speedy = 0;
	int speedy2 = 0;
	int ballSpeedX = -5;
	int ballSpeedY = 2;
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
							speedy2 = -4;
							break;
						case SDLK_DOWN:
							speedy2 = 4;
							break;
						case SDLK_w:
							speedy = -4;
							break;
						case SDLK_s:
							speedy = 4;
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
		if(ball.getPos()->y < 0 || ball.getPos()->y > 450)
		{
			ballSpeedY *= -1;
		}
		if(ball.getPos()->x < 0)
		{
			// second player scores
			ball.setPos(280, 250); 
		}
		if(ball.getPos()->x > 600)
		{
			// first player scores
			ball.setPos(280, 250);
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
