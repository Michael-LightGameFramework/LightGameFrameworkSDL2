// A simple pong-like game and proof of concept.
// To Do:
// 	Flip animation for both ninjaGirl and dragon's direction
// 	Fix 'dragon grab'
// 	Allow user to choose characters
// 	Add an attack feature that changes ball's speed/direction
// 	Scoring system

#include "item.h"
#include <unordered_map>
#include <iostream>

enum state
{
	A_IDLE = 0,
	A_WALK = 1,
	A_JUMP = 2,
	A_HURT = 4,
	A_ATTACK = 1 << 3,
	A_SLIDE = 1 << 4,
	A_LEFT = 1 << 24,
	A_RIGHT = 1 << 25
};



class npc : public animation
{
	public:
	npc();
	~npc();
	void walkTo(int x, int y);
	void play(int index);
	void update(int tick);
	void setActive(int index);
	animation * getAnimation(int index);

	private:
	int walkY, walkX;
	animation * idle;
	animation * playing;
	int speedX, speedY;
	std::unordered_map <int, animation*> acts;

};

npc::npc()
{
	desiredDelta = 1000/12;
	speedX = 1;
	speedY = 1;
}

npc::~npc()
{
	for(auto& temp : acts)
	{
		delete &temp;
	}
}

void npc::walkTo(int x, int y)
{
	if(x > pos.x)
	{
		setActive(A_WALK || A_RIGHT);
	}
	else if(x < pos.x)
	{
		setActive(A_WALK || A_LEFT);
	}
	walkX = x;
	walkY = y;
}

void npc::setActive(int index)
{
	this->images = acts[index]->getImages();	
}

animation * npc::getAnimation(int index)
{
	if(acts[index] == NULL)
	{
		acts[index] = new animation;
	}
	return acts[index];
}

void npc::update(int tick)
{
	animation::update(tick);
	int destX = pos.x+ speedX;
	int destY = pos.y + speedY;
	if(walkX != pos.x)
	{
		if(destX*pos.x + destX*walkX == walkX * pos.x)
		{
			pos.x = walkX;
		}
		else
		{
			pos.x = destX;
		}
	}
	if(walkY != pos.y)
	{
		if(destY * pos.y + destY * walkY == walkY * pos.y)
		{
			pos.y = walkY;
		}
		else
		{
			pos.y = destY;
		}
	}
}




int main(int argc, char ** arg)
{
	int WINX = 2048;
	int WINY = 1152;
	itemInit();
	SDL_Window * win = SDL_CreateWindow("Dragon Pong", 30, 30, 2048, 1152, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
	SDL_Renderer * screen = SDL_CreateRenderer(win, -1, 0);

	item bkg;
	bkg.setSize(WINX, WINY);
	bkg.setPos(0,0);
	bkg.setRenderer(screen);
	bkg.loadImage("battleback1.png");

	npc kara;
	kara.setRenderer(screen);
	kara.setSize(50, 100);
	kara.setPos(10, 10);
	kara.setCenter(25, 50, 14);

	animation *left = kara.getAnimation(A_WALK || A_LEFT);
	left->setRenderer(screen);
	left->setFPS(12);
	left->setSize(50, 100);
	left->setPos(10, 10);
	left->loadAnimation("png/Slide__", "000", ".png");

	kara.setActive(A_WALK || A_LEFT);

	animation * right = kara.getAnimation(A_WALK || A_RIGHT);
	right->setRenderer(screen);
	right->setFPS(12);
	right->setSize(50, 100);
	right->loadAnimation("png/Run__", "000", ".png");

	bool run = true;
	SDL_SetRenderDrawColor(screen, 30, 180, 20, 255);
	int fps = 40;
	int desiredDelta = 1000 / fps;

	kara.walkTo(1000, 300);

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
							kara.setActive(A_WALK || A_RIGHT);
							break;
						case SDLK_DOWN:
							kara.setActive(A_WALK || A_LEFT);
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
		kara.update(startLoop);
		bkg.draw();
		kara.draw();

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
