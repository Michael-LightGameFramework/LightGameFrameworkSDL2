#include "item.h"
#include <iostream>

bool givePlanet;
void giveGoal(item * caller)
{
	givePlanet = true;
}
int random(int ignored)
{
	return random() % 1080;
}

int doNoth(int val)
{
	return val;
}

int bigSine(int val)
{
	return 1080 * sin(val);
}	

int bigCos(int val)
{
	return 1900 * cos(val);
}	



class player : public item
{
	public:
	player(SDL_Renderer * screen);
	void accelerate(bool left, bool right, bool up, bool down);
	void update(int tick);
	void draw();

	public:
	double angle;
	bool faceLeft;
	float speedX;
	float speedY;
	float accelX;
	float accelY;
	float maxX;
	float maxY;
};



player::player(SDL_Renderer * screen)
{
	faceLeft = false;
	speedX = 0;
	speedY = 0;
	accelX = .4;
	accelY = .2;
	maxX = 10;
	maxY = 6;
	ren = screen;
	angle = 0;
}

void player::accelerate(bool left, bool right, bool up, bool down)
{

	if(up && speedY > -maxY)
	{
		speedY -= accelY;
	}
	if(down && speedY < maxY)
	{
		speedY += accelY;
	}
	if(left && speedX > -maxX)
	{
		speedX -= accelX;
	}
	if(right && speedX < maxX)
	{
		speedX += accelX;
	}

	if(speedX > 0.16f)
	{
		faceLeft = false;
		angle = (5.0 *speedY)/speedX;
	}
	else if(speedX < -0.16f)
	{
		faceLeft = true;
		angle = (5.0 *speedY)/speedX;
	}
	else
	{
		angle = (5*speedY)/0.33f;
	}
}

void player::update(int tick)
{
	this->oldTick = tick;
	if(pos.y > 1080)
	{
		pos.y = -pos.h;
	}
	else if(pos.y + pos.h < -10)
	{
		pos.y = 1080;
	}
	if(pos.x < -20)
	{
		pos.x = -20;
		speedX *= -.8;
	}
	if(pos.x > 1880)
	{
		pos.x = -pos.w;
	}

	
	move((int) speedX, (int) speedY);
}



void player::draw()
{
	SDL_Point ct = {center.x, center.y};
	if(faceLeft)
	{
		if(image)
		{
			SDL_RenderCopyEx(ren, image, NULL, &pos, angle, &ct, SDL_FLIP_HORIZONTAL );
		}
	}
	else
	{
		if(image)
		{
			SDL_RenderCopyEx(ren, image, NULL, &pos,  angle, &ct, SDL_FLIP_NONE);
		}
	}

}



class spawner : public group
{
	public:
	spawner(SDL_Renderer * screen);
	void spawn();
	void setSize(int w, int h);
	void update(int tick);
	void remove(int index);
	void clear();

	item* getCollisions(item * other);
	void fastForeward(int countFrames);

	int (*moveXFunc)(int x);
	int (*moveYFunc)(int y);
	void (*onEmpty)(item *);


	public:
	int everyNMilisec;
	animation * base;
	SDL_Rect bounds;
	SDL_Point dest; // where you want child to go towards.
	int engine = 0;
	int speedX = -5;
	SDL_Rect particleSize;
	bool spawning;
	int gravidity = -1; // how many items can spawn in a lifetime(-1 == infinite)

};



spawner::spawner(SDL_Renderer * screen)
{
	spawning = false;
	ren = screen;
	bounds = {-200, -100, 2400, 1400};
	moveXFunc = doNoth;
	moveYFunc = bigSine;
	base = NULL;
	everyNMilisec = 400;
	particleSize = {pos.x, pos.y, 100, 100};

}

void spawner::setSize(int w, int h)
{
	particleSize.w = w;
	particleSize.h = h;
}

void spawner::remove(int index)
{
	item * del = items[index];
	if(del)
	{
		delete(del);
	}
	items.erase(items.begin() + index);
}

void spawner::clear()
{
	for(int i = items.size()-1; i >= 0; i --)
	{
		item * del = items[i];
		if(del)
		{
			delete(del);
		}
	}
	items.clear();
	oldTick = 0;
}

void spawner::update(int tick)
{
	pos.y = moveYFunc(engine);
	if(base && spawning)
	{
		if(oldTick == 0)
		{
			oldTick = tick;
		}
		base->update(tick);
		if(tick - oldTick > everyNMilisec)
		{
			oldTick = tick;
			if(gravidity)
			{
				addRef(new item(ren));
				items[items.size() -1]->setImage(base->getImage());
				items[items.size() -1]->setPos(pos.x, pos.y);
				items[items.size() -1]->setSize(particleSize.w, particleSize.h);
				items[items.size() -1]->setCenter(center.x, center.y, center.r);
				engine ++;
				gravidity --;
				if(gravidity == 0 && onEmpty)
				{
					onEmpty(this);
				}
					
			}
		}
		for(int i = 0; i < items.size(); i ++)
		{

			items[i]->move(speedX, 0);
			if(!(SDL_HasIntersection(&bounds, items[i]->getPos())))
			{
				remove(i);
				i --;
			}
			//items[i]->setImage(base->getImage());
		}
	}
}

void spawner::fastForeward(int mils)
{
	bool reset = spawning;
	spawning = true;
	oldTick = 0;
	for(int i = 1; i < mils; i += 13)
	{
		update(i);
	}
	oldTick = 0;
	
	spawning = reset;
}

item * spawner::getCollisions(item * other)
{
	for(int i = 0; i < items.size(); i ++)
	{
		if(SDL_HasIntersection(other->getPos(), items[i]->getPos()))
		{
			return items[i];
		}
	}
	return NULL;
}



int main(int argc, char ** arg)
{
	itemInit();
	SDL_Window * win = SDL_CreateWindow("title", 30, 30, 1600, 900, SDL_WINDOW_SHOWN | SDL_WINDOW_FULLSCREEN_DESKTOP);
	SDL_Renderer * screen = SDL_CreateRenderer(win, -1, SDL_RENDERER_PRESENTVSYNC);
	SDL_SetRenderDrawColor(screen, 30, 180, 20, 255);

	SDL_Rect windowSize{0};
       	SDL_GetWindowSize(win, &windowSize.w, &windowSize.h);

	item bkg(screen);
	bkg.setSize(2000, 1080);
	bkg.setPos(0,0);
	bkg.loadImage("res/images/bkg.png");

	givePlanet = false;
	item goal(screen);
	goal.loadImage("res/images/earth.png");
	goal.setPos(windowSize.w + 320, windowSize.h/3);
	goal.setSize(600, 600);
	goal.setCenter(150, 150, 150);

	animation * enemy = new animation;
	enemy->setRenderer(screen);
	enemy->loadAnimation("res/images/refugee", "0", ".png");
	enemy->setFPS(3);

	animation * star = new animation(screen);
	star->loadAnimation("res/images/star", "1", ".png");

	spawner starSpawn(screen);
	starSpawn.base = star;
	starSpawn.setPos(windowSize.w + 100, 12);
	starSpawn.bounds = {-100, -100, windowSize.w + 400, windowSize.h + 400};
	starSpawn.setSize(22, 12);
	starSpawn.speedX = -3;
	starSpawn.everyNMilisec = 500;
	starSpawn.moveYFunc = random;
	starSpawn.spawning = true;
	starSpawn.fastForeward(30000);

	spawner enemySpawn(screen);
	enemySpawn.setCenter(90, 40, 40);
	enemySpawn.base = enemy;
	enemySpawn.setPos(windowSize.w + 200, 12);
	enemySpawn.setSize(240, 85);
	enemySpawn.everyNMilisec = 200;
	int maxGrav = 220;
	enemySpawn.gravidity = maxGrav;
	enemySpawn.onEmpty = giveGoal;

	player ship(screen);
	ship.setCenter(135, 59, 56);
	ship.loadImage("res/images/player.png");
	ship.setPos(30, windowSize.h/2 - 150);
	ship.setSize(245, 115);

	bool pause = true;

	bool up = false;
	bool down = false;
	bool left = false;
	bool right = false;
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
							up = true;
							break;
						case SDLK_DOWN:
							down = true;
							break;
						case SDLK_RIGHT:
							right = true;
							break;
						case SDLK_LEFT:
							left = true;
							break;
						case SDLK_SPACE:
							enemySpawn.spawning = true;
							pause = !pause;
							break;
						case SDLK_ESCAPE:
							run = false;
							break;
					}

					break;
				case SDL_KEYUP:
					switch(ev.key.keysym.sym)
					{
						case SDLK_UP:
							up = false;
							break;
						case SDLK_DOWN:
							down = false;
							break;
						case SDLK_RIGHT:
							right = false;
							break;
						case SDLK_LEFT:
							left = false;
							break;
					}
					break;
			}
		}

		if(!pause)
		{
			if(givePlanet)
			{
				goal.move(-1, 0);
				if(goal.getPos()->x < 1000)
				{
					givePlanet = false;
				}

			}
			ship.accelerate(left, right, up, down);
			ship.update(startLoop);
			enemySpawn.update(startLoop);
			if(left)
			{
				enemySpawn.speedX = -5;
			}
			if(right)
			{
				enemySpawn.speedX = -8;
			}
			else
			{
				enemySpawn.speedX = -5;
			}
			if(ship.getCollision(&goal))
			{
				std::cout << "Win!\n";
			}
			item * crash = enemySpawn.getCollisions(&ship);
			if(crash)
			{
				if(ship.getCollision(crash))
				{
					pause = true;
					ship.setPos(30, 500);
					ship.speedX = 0;
					ship.speedY = 0;
					enemySpawn.clear();
					enemySpawn.spawning = false;
					enemySpawn.gravidity = maxGrav;
					maxGrav -= 12;
					givePlanet = false;
					goal.setPos(2200, 400);
				}
			}
			starSpawn.update(startLoop);

		}

		SDL_RenderClear(screen);
		bkg.draw();
		starSpawn.draw();
		enemySpawn.draw();
		ship.draw();
		goal.draw();
		SDL_RenderPresent(screen);

	}

	
	delete enemy;
	delete star;

	SDL_DestroyWindow(win);
	itemQuit();

}
