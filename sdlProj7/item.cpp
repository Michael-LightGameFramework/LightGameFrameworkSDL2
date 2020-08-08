#include "item.h"
#include <iostream>

void itemInit()
{
	SDL_Init(SDL_INIT_EVERYTHING);
	IMG_Init(IMG_INIT_PNG);
}

void itemQuit()
{
	SDL_Quit();
	IMG_Quit();
}

item::item()
{
	pos.x = 30;
	pos.y = 60;
	image = NULL;
	pos.w = 100;
	pos.h = 100;
}

item::~item()
{
	if(image != NULL)
	{
		SDL_DestroyTexture(image);
		image = NULL;
	}
}

bool item::loadImage(std::string filename)
{
	if(image != NULL)
	{
		SDL_DestroyTexture(image);
		image = NULL;
	}
	SDL_Surface * temp = IMG_Load(filename.c_str());
	if(temp != NULL)
	{
		image = SDL_CreateTextureFromSurface(ren, temp);
		SDL_FreeSurface(temp);
		if(image != NULL)
		{
			return true;
		}
	}
	return false;
}

void item::setRenderer(SDL_Renderer * dest)
{
	ren = dest;
}

void item::setSize(int w, int h)
{
	pos.w = w;
	pos.h = h;
}

void item::setPos(int x, int y)
{
	pos.x = x;
	pos.y = y;
}

SDL_Rect * item::getPos()
{
	return &pos;
}

void item::move(int x, int y)
{
	pos.x += x;
	pos.y += y;
}

bool item::getCollision(item * other)
{
	int dx, dy, rs;
	dx = pos.x + center.x - (other->getPos()->x + other->getCenter().x);
	dy = pos.y + center.y - (other->getPos()->y + other->getCenter().y);
	rs = center.r + other->getCenter().r;
	dx *= dx;
	dy *= dy;
	rs *= rs;
	if(dx + dy < rs)
	{
		return true;
	}
	return false;

}

circle item::getCenter()
{
	return center;
}

void item:: setCenter(int x, int y, int r)
{
	center.x = x;
	center.y = y;
	center.r = r;
}

void item::draw(double angle)
{

	if(image != NULL)
	{
		SDL_RenderCopyEx(ren, image, NULL, &pos, angle, NULL, SDL_FLIP_NONE);
	}
	else 
	{
		std::cout << "Help, image is NULL at draw()\n";
	}

}

void item::draw()
{
	if(image != NULL)
	{
		SDL_RenderCopy(ren, image, NULL, &pos);
	}
	else 
	{
		std::cout << "Help, image is NULL at draw()\n";
	}
}


