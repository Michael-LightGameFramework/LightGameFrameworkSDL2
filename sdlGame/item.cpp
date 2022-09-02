#include "item.h"
#include <iostream>

void itemInit()
{
	SDL_Init(SDL_INIT_EVERYTHING);
	IMG_Init(IMG_INIT_PNG);
	Mix_Init(MIX_INIT_MP3 | MIX_INIT_OPUS | MIX_INIT_OGG);
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 6, 4096);
}

void itemQuit()
{
	Mix_CloseAudio();
	Mix_Quit();
	IMG_Quit();
	SDL_Quit();
}

item::item()
{
	oldTick = 0;
	pos.x = 30;
	pos.y = 60;
	image = NULL;
	pos.w = 100;
	pos.h = 100;
	owned = false;
}

item::item(SDL_Renderer * screen) : item()
{
	ren = screen;
}

item::~item()
{
	if(owned && image)
	{
		SDL_DestroyTexture(image);
		image = NULL;
	}

}


bool item::loadImage(std::string filename)
{
	SDL_Surface * temp = IMG_Load(filename.c_str());
	if(temp != NULL)
	{
		image = SDL_CreateTextureFromSurface(ren, temp);
		SDL_FreeSurface(temp);
		if(image != NULL)
		{
			owned = true;
			return true;
		}
	}
	return false;
}
void item::freeImage()
{
	if(image != NULL)
	{
		SDL_DestroyTexture(image);
		image = NULL;
	}
	if(!owned)
	{
		SDL_Log("Deleted a texture not owned by item!\n");
	}
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

bool item::isClicked(int x, int y)
{
	int dx, dy, rs;
	dx = pos.x + center.x - x;
	dy = pos.y + center.y - y;
	rs = center.r;
	dx *= dx;
	dy *= dy;
	rs *= rs;
	return (dx + dy < rs);
}

circle item::getCenter()
{
	return center;
}

void item::setCenter(int x, int y, int r)
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
void item::drawHFlip()
{
	if(image != NULL)
	{
		SDL_RenderCopyEx(ren, image, NULL, &pos, 0, NULL, SDL_FLIP_HORIZONTAL);
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

void item::update(int tick)
{
	oldTick = tick;
}

SDL_Texture * item::getImage()
{
	return image;
}

void item::setImage(SDL_Texture * img)
{
	image = img;
}

////////////// CLASS ANIMATION

animation::animation()
{

}

animation::animation(SDL_Renderer * screen)
{
	ren = screen;
}

animation::~animation()
{
	this->freeImages();
}


bool animation::addImage(std::string filePath)
{
	if(loadImage(filePath))
	{
		images.push_back(image);
		return true;
	}
	return false;
}


bool animation::loadAnimation(std::string p, std::string zb, std::string ext)
{
	int leng = zb.length() - 1;
	while(addImage(p + zb + ext) && zb[0] < '9')
	{
		zb[leng] ++;
		for(int i = leng; i > 0; i --)
		{
			if(zb[i] > '9')
			{
				zb[i-1] ++;
				zb[i] = '0';
			}
		}
	}
	if(!(images.empty()))
	{
		return true;
	}
	else
	{
		image = NULL;
	}
	return false;
}

void animation::next()
{
	if(images.empty())
	{
		std::cout << "Tried to update empty animation" << std::endl;
	}
	else
	{
		frameCount ++;
		image = images[frameCount % (images.size())];
	}
}

void animation::freeImages()
{
	for(int i = 0; i < images.size(); i ++)
	{
		if(images[i] != NULL)
		{
			SDL_Texture * del = images[i];
			SDL_DestroyTexture(del);
			images[i] = NULL;
		}
	}
}


void animation::setFPS(int FPS)
{
	desiredDelta = 1000 / FPS;
}

void animation::update(int tick)
{
	if(tick - oldTick > desiredDelta)
	{
		next();
		oldTick = tick;
	}
}


////////////////////////////
// GROUP
////////////////////////////

void group::draw()
{
	for(int i = 0; i < items.size(); i ++)
	{
		items[i]->draw();
	}
}

void group::draw(double angle)
{
	for(int i = 0; i < items.size(); i ++)
	{
		items[i]->draw(angle);
	}
}

void group::addRef(item * other)
{
	if(other)
	{
		items.push_back(other);
	}
	else
	{
		SDL_Log("Tried to add NULL item pointer to group\n");
	}
}

void group::move(int x, int y)
{
	for(int i = 0; i < items.size(); i ++)
	{
		items[i]->move(x, y);
	}
}

std::vector <item*> group::getBoundedItems(SDL_Rect bounds)
{
	std::vector <item *> ret;
	int len = items.size();
	for(int i = 0; i < len; i ++)
	{
		if(SDL_HasIntersection(&bounds, items[i]->getPos()))
		{
			ret.push_back(items[i]);
		}
	}
	return ret;
}


////////////////////////////////////////
// Board Class
////////////////////////////////////

board::board()
{
}

board::board(SDL_Renderer * rend)
{
	ren = rend;
	int w, h;
	SDL_GetRendererOutputSize(ren, &w, &h);
	Player.setPos(w/2 - 32, h/2 - 32);
	bkg.setSize(w, h);
	bkg.setPos(0, 0);
}


void board::draw()
{
	bkg.draw();
	drawn.draw();
	Player.draw();
}

void board::move(int x, int y)
{
	drawn.move(-x, -y);
	click.move(-x, -y);
	collide.move(-x, -y);
}

void board::handleEvent(SDL_Event * ev)
{

}

void board::init()
{

}






