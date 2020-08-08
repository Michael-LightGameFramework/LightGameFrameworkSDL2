#include "item.h"

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
	image = SDL_CreateRGBSurface(0, 100, 200, 32, 0,0,0,0);
	pos.w = image->clip_rect.w;
	pos.h = image->clip_rect.h;
	SDL_FillRect(image, NULL, 0xffff00);
}

item::~item()
{
	if(image != NULL)
	{
		SDL_FreeSurface(image);
		image = NULL;
	}
}

bool item::loadImage(std::string filename)
{
	if(image != NULL)
	{
		SDL_FreeSurface(image);
		image = NULL;
	}
	image = IMG_Load(filename.c_str());
	if(image != NULL)
	{
		return true;
	}
	return false;
}

void item::draw(SDL_Surface * dest)
{
	if(image != NULL)
	{
		SDL_BlitSurface(image, NULL, dest, &pos);
	}
}


