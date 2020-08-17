// A simple pong-like game and proof of concept.
// To Do:
// 	Flip animation for both ninjaGirl and dragon's direction
// 	Fix 'dragon grab'
// 	Allow user to choose characters
// 	Add an attack feature that changes ball's speed/direction
// 	Scoring system

#include "item.h"

class flipped : public animation
{
	public:
	void draw();
};

void flipped::draw()
{
	SDL_RenderCopyEx(ren, image, NULL, &pos, 0, NULL, SDL_FLIP_HORIZONTAL);
}


void saveScreen(const char* file_name, SDL_Renderer* renderer) {
    SDL_Surface* surface = SDL_CreateRGBSurface(0, 2048, 1152, 32, 0, 0, 0, 0);
    SDL_RenderReadPixels(renderer, NULL, surface->format->format, surface->pixels, surface->pitch);
    IMG_SavePNG(surface, file_name);
    SDL_FreeSurface(surface);
}

int main(int argc, char ** arg)
{
	itemInit();
	SDL_Window * win = SDL_CreateWindow("Dragon Pong", 30, 30, 2048, 1152, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
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

	flipped second;
	second.setRenderer(screen);
	second.setCenter(75, 100, 120);
	second.setSize(150, 240);
	second.setPos(1520, 450);
	second.loadAnimation("png/Run__", "000", ".png");

	animation ball;
	ball.setRenderer(screen);
	ball.setCenter(75, 75, 25);
	ball.setSize(150,150);
	ball.setPos(980, 550);
	ball.loadAnimation("PNG/frame-", "1", ".png");

	bool run = true;
	SDL_SetRenderDrawColor(screen, 30, 180, 20, 255);
	int fps = 10;
	int desiredDelta = 1000 / fps;
	int speedy = 0, speedy2 = 0, speedx = 0, speedx2 = 0;
	int ballSpeedX = -18;
	int ballSpeedY = 15;
	int playerSpeed = 8;
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
							speedy2 = -playerSpeed;
							break;
						case SDLK_DOWN:
							speedy2 = playerSpeed;
							break;
						case SDLK_LEFT:
							speedx2 = -playerSpeed;
							break;
						case SDLK_RIGHT:
							speedx2 = playerSpeed;
							break;
						case SDLK_w:
							speedy = -playerSpeed;
							break;
						case SDLK_s:
							speedy = playerSpeed;
							break;
						case SDLK_a:
							speedx = -playerSpeed;
							break;
						case SDLK_d:
							speedx = playerSpeed;
							break;
					}

					break;
				case SDL_KEYUP:
					switch(ev.key.keysym.sym)
					{
						case SDLK_UP:
						case SDLK_DOWN:
							speedy2 = 0;
							break;
						case SDLK_LEFT:
						case SDLK_RIGHT:
							speedx2 = 0;
							break;
						case SDLK_w:
						case SDLK_s:
							speedy = 0;
							break;
						case SDLK_a:
						case SDLK_d:
							speedx = 0;
							break;
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
		first.move(speedx, speedy);
		second.move(speedx2, speedy2);
		ball.move(ballSpeedX, ballSpeedY);

		bkg.draw();
		first.draw();
		second.draw();
		ball.draw();

		SDL_RenderPresent(screen);

		if(speedx || speedy)
		{
			first.next();
		}
		if(speedx2 || speedy2)
		{
			second.next();
		}
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
