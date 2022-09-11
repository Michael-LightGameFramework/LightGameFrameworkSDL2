#include <level1_arrival.h>
#include <iostream>

item * spawn(SDL_Renderer * screen)
{
	return new level1(screen);
}

void destroy(item * key)
{
	delete (level1 *) key;
}


bool givePlanet = false;
int doNoth(int val)
{
	return val;
}

int bigSine(int val)
{
	return 1080*sin(val);
}
int random(int ignored)
{
	return random() %  1080;
}

void activatePlanet(item * caller)
{
	givePlanet = true;
}

//////////////////////////
// PLAYER
/////////////////////////

player::player()
{
        faceLeft = false;
        speedX = 0;
        speedY = 0;
        accelX = .4;
        accelY = .2;
        maxX = 10;
        maxY = 6;
        ren = NULL;
        angle = 0;
}
player::player(SDL_Renderer * screen) : player()
{
	ren = screen;
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

	if(speedX >= 0.0f && speedX < 1.0f)
	{
		faceLeft = false;
		angle = (5.0 * speedY)/0.43f;
	}
	else if(speedX <= 0.0f && speedX >= -1.0f)
	{
		faceLeft = true;
		angle = (-5.0 * speedY)/0.43f;
	}
	else if(speedX >= 1.0f)
        {
		faceLeft = false;
                angle = (5.0 *speedY)/speedX;
        }
        else if(speedX <= 1.0f)
        {
                faceLeft = true;
                angle = (5.0 *speedY)/speedX;
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
        if(pos.x < -22)
        {
                pos.x = -20;
                speedX *= -.8;
		faceLeft = false;
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



/////////////////////////
// SPAWNER 
////////////////////////

spawner::spawner()
{
        spawning = false;
        bounds = {-200, -100, 2400, 1400};
        moveXFunc = NULL;
        moveYFunc = bigSine;
	speedX = -1;
	speedY = 0;
        base = NULL;
        everyNMilisec = 400;
        particleSize = {pos.x, pos.y, 100, 100};
	oldTick = 0;
	gravidity = -1;
}

spawner::spawner(SDL_Renderer * screen) : spawner()
{
        ren = screen;
}

spawner::~spawner()
{
	clear();
}

void spawner::spawn()
{
	if(gravidity)
	{
		addRef(new item);
		items[items.size() -1]->setRenderer(ren);
		items[items.size() -1]->setImage(base->getImage());
		items[items.size() -1]->setPos(pos.x, pos.y);
		items[items.size() -1]->setSize(particleSize.w, particleSize.h);
		items[items.size() -1]->setCenter(center.x, center.y, center.r);
		gravidity --;
		if(gravidity == 0 && onEmpty)
		{
			onEmpty(this);
		}
	}
}

void spawner::drawSelf()
{
	if(base)
	{
		setImage(base->getImage());
		item::draw();
	}
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
        if(base && spawning)
        {
		if(moveYFunc)
		{
	        	pos.y = moveYFunc(engine);
		}
		if(moveXFunc)
		{
			pos.x = moveXFunc(engine);
		}
                if(oldTick == 0)
                {
                        oldTick = tick;
                }
                base->update(tick);
                if(tick - oldTick > everyNMilisec)
                {
                        oldTick = tick;
			engine ++;
                        if(gravidity)
                        {
				spawn();
                        }
                }
                for(int i = 0; i < items.size(); i ++)
                {
                        items[i]->move(speedX, speedY);
                        if(!(SDL_HasIntersection(&bounds, items[i]->getPos())))
                        {
                                remove(i);
                                i --;
                        }
                        if(!base->getImage())
			{
				std::cout << "NULL IMAGE IN BASE ITEM!\n";
			}
                }
        }
}
void spawner::fastForward(int mils)
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

item * spawner::checkCollision(item * other)
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





//////////////////////////
// LEVEL1
//////////////////////////

level1::level1(SDL_Renderer * screen): board(screen)
{
	name = "level1";
	ren = screen;

	SDL_GetRendererOutputSize(ren, &windowSize.w, &windowSize.h);
	up = false;
	down = false;
	left = false;
	right = false;
	pause = false;
	angle = 0;
	maxGravidity = 220;
	giveGoal = activatePlanet;

	bkg = new item(screen);
	bkg->loadImage("res/images/bkg.png");
	bkg->setSize(windowSize.w, windowSize.h);
	bkg->setPos(0,0);

	goal.setRenderer(screen);
	goal.loadImage("res/images/earth.png");
	goal.setSize(600, 600);
	goal.setCenter(150, 150, 150);

	refugee = new animation(screen);
	refugee->loadAnimation("res/images/refugee", "0", ".png");
	refugee->setFPS(3);

	refugeeSpawn.setRenderer(screen);
	refugeeSpawn.base = refugee;
	refugeeSpawn.setCenter(90, 40, 40);
	refugeeSpawn.setPos(windowSize.w + 200, 32);
	refugeeSpawn.setSize(240, 85);
	refugeeSpawn.everyNMilisec = 200;
	refugeeSpawn.onEmpty = giveGoal;

	star = new animation(screen);
	star->loadAnimation("res/images/star", "1", ".png");
	star->setPos(50, 50);

	starSpawn.setRenderer(screen);
	starSpawn.base = star;
	starSpawn.setPos(windowSize.w + 200, 12);
	starSpawn.setSize(22, 10);
	starSpawn.speedX = -3;
	starSpawn.bounds = {-100, -100, windowSize.w + 500, windowSize.h += 400};
	starSpawn.everyNMilisec = 500;
	starSpawn.moveYFunc = random;
	starSpawn.spawning = true;
	starSpawn.fastForward(15000);

	ship.setRenderer(screen);
	ship.loadImage("res/images/player.png");
	ship.setCenter(135, 59, 54);
	ship.setPos(30, windowSize.h/2 - 150);
	ship.setSize(245, 115);


	init();
}
level1::~level1()
{
	delete star;
	delete refugee;
}
// Set the variables that the game is going to reset on game restart;
void level1::init()
{
	board::init();
	pause = true;
	if(maxGravidity > 34)
	{
		maxGravidity -= 12;
	}
	givePlanet = false;
	goal.setPos(windowSize.w + 320, windowSize.h / 3);

	refugeeSpawn.clear();
	refugeeSpawn.gravidity = maxGravidity;

	ship.setPos(30, windowSize.h/2 - 150);
	ship.speedX = 0.0f;
	ship.speedY = 0.0f;
}

void level1::handleEvent(SDL_Event * ev)
{
	switch(ev->type)
	{
		case SDL_KEYDOWN:
			switch(ev->key.keysym.sym)
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
					pause = !pause;
					refugeeSpawn.spawning = true;
					break;
			}
			break;
		case SDL_KEYUP:
			switch(ev->key.keysym.sym)
			{
				case SDLK_UP:
					up = false;
					break;
				case SDLK_DOWN:
					down = false;
					break;
				case SDLK_LEFT:
					left = false;
					break;
				case SDLK_RIGHT:
					right = false;
					break;
			}
			break;
	}

}

void level1::update(int tick)
{
	if(pause)
	{
		return;
	}
	if(givePlanet)
	{
		goal.move(-1, 0);
		if(goal.getPos()->x < 1000)
		{
			givePlanet = false;
		}
	}
	ship.accelerate(left, right, up, down);
	ship.update(tick);
	refugeeSpawn.update(tick);
	if(right)
	{
		refugeeSpawn.speedX = -8;
	}
	else
	{
		refugeeSpawn.speedX = -5;
	}
	if(ship.getCollision(&goal))
	{
		// win state, activate next level!
		fin = true;
		request = "overworld";
	}
	// box check
	item * crash = refugeeSpawn.checkCollision(&ship);
	if(crash)
	{
		// circle check
		if(ship.getCollision(crash))
		{
			init();
		}
	}
	starSpawn.update(tick);
	

}

void level1::draw()
{
	bkg->draw();
	starSpawn.draw();
	refugeeSpawn.draw();
	ship.draw();
	goal.draw();
	starSpawn.drawSelf();

}


void level1::togglePause()
{
	pause = !pause;
	refugeeSpawn.spawning = pause;
}
