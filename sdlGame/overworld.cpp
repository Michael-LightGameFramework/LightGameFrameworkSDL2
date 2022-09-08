#include <overworld.h>
#include <iostream>


item * spawn(SDL_Renderer * screen)
{
	return new overworld(screen);
}

void destroy(item * key)
{
	delete (overworld *) key;
}

/////////////////////////////////
// PLAYER
////////////////////////////////


player::player(SDL_Renderer * screen)
{
	faceLeft = false;
	ren = screen;
	frames.setRenderer(screen);
	frames.addImage("res/images/classicPlayer.png", 16, 16);
	actions["die"] = new animation(screen);
	mapAnimation("die", 0);
	mapAnimation("die", 1);
	mapAnimation("die", 2);
	mapAnimation("die", 3);
	mapAnimation("die", 4);
	actions["die"]->setFPS(6);

	actions["idle"] = new animation(screen);
	mapAnimation("idle", 0);
	mapAnimation("idle", 1);
	mapAnimation("idle", 2);
	mapAnimation("idle", 3);
	actions["idle"]->setFPS(6);

	actions["walk"] = new animation(screen);
	mapAnimation("walk", 6);
	mapAnimation("walk", 7);
	mapAnimation("walk", 8);
	mapAnimation("walk", 9);
	mapAnimation("walk", 10);
	mapAnimation("walk", 11);
	actions["walk"]->setFPS(6);

	currentAnimation = actions["idle"];

}

void player::mapAnimation(std::string name, int index)
{
	if(actions[name])
	{
		actions[name]->addImage(frames.get(index));
	}
	else
	{
		std::cout << name << " needs to be allocated before maping images to it." << std::endl;
	}
}

void player::draw()
{
	if(currentAnimation)
	{
		if(faceLeft)
		{
			currentAnimation->drawHFlip();
		}
		else
		{
			currentAnimation->draw();
		}
	}
}

void player::update(int tick)
{
	if(currentAnimation)
	{
		currentAnimation->update(tick);
		currentAnimation->setPos(pos.x, pos.y);
	}
}

void player::setAnimation(std::string keyword)
{
	if(actions[keyword])
	{
		currentAnimation = actions[keyword];
	}
}



////////////////////////////////
// OVERWORLD
////////////////////////////////

overworld::overworld(SDL_Renderer * screen) : board(screen)
{
	up = false;
	down = false;
	left = false;
	right = false;
	speedX = 4;
	speedY = 4;
	ren = screen;
	name = "overworld";
	jones = new player(screen);
	jones->setSize(40, 40);
	jones->setPos(400, 300);

	bkg = new animation(screen);
	bkg->loadAnimation("res/images/OverBkg", "0", ".png");
	bkg->setSize(6000, 6000);
	bkg->setPos(0, 0);


	land.addRef(new item(screen));
	land.last()->loadImage("res/images/land02.png");
	land.last()->setPos(100, 20);
	land.last()->setSize(4000, 2335);

	land.addRef(new item(screen));
	land.last()->loadImage("res/images/land01.png");
	land.last()->setPos(700, 220);
	land.last()->setSize(1500, 835);

	
}

void overworld::init()
{
}


void overworld::draw()
{
	bkg->draw();
	land.draw();
	jones->draw();
}

void overworld::handleEvent(SDL_Event * ev)
{
	switch(ev->type)
	{
		case SDL_KEYDOWN:
			switch(ev->key.keysym.sym)
			{
				case SDLK_LEFT:
					left = true;
					break;
				case SDLK_RIGHT:
					right = true;
					break;
				case SDLK_UP:
					up = true;
					break;
				case SDLK_DOWN:
					down = true;
					break;
			}
			break;
		case SDL_KEYUP:
			switch(ev->key.keysym.sym)
			{
				case SDLK_LEFT:
					left = false;
					break;
				case SDLK_RIGHT:
					right = false;
					break;
				case SDLK_UP:
					up = false;
					break;
				case SDLK_DOWN:
					down = false;
					break;
			}
			break;
	}

}

void overworld::update(int tick)
{
	jones->setAnimation("idle");
	if(up)
	{
		jones->move(0, -speedY);
	
	}
	if(down)
	{
		jones->move(0, speedY);
	}
	if(right)
	{
		jones->setAnimation("walk");
		jones->move(speedX, 0);
		jones->faceLeft = false;
	}
	if(left)
	{
		jones->setAnimation("walk");
		jones->faceLeft = true;
		jones->move(-speedX, 0);
	}
	if(jones->getPos()->x > 900)
	{
		bkg->move(-speedX, 0);
		land.move(-speedX, 0);
		jones->move(-speedX, 0);
	}
	else if(jones->getPos()->x < 90)
	{
		bkg->move(speedX, 0);
		land.move(speedX, 0);
		jones->move(speedX, 0);
	}
	if(jones->getPos()->y > 600)
	{
		bkg->move(0, -speedY);
		land.move(0, -speedY);
		jones->move(0, -speedY);
	}
	else if(jones->getPos()->y < 200)
	{
		bkg->move(0, speedY);
		land.move(0, speedY);
		jones->move(0, speedY);

	}
	jones->update(tick);
}
