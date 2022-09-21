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

std::string absPath()
{
	char * temp = SDL_GetPrefPath("LightGameFramework", "NoTitle");
	std::string path = temp;
	SDL_free(temp);
	return path;
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
	center.r = 12;
	center.x = 12;
	center.y = 12;
	shown = true;
	solid = false;
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
	if(image != NULL && owned)
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

	if(shown)
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

}
void item::drawHFlip()
{
	if(shown)
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
}

void item::draw()
{
	if(shown)
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

void item::drawHitShape()
{
	int px = center.x + pos.x;
	int py = center.y + pos.y;
	SDL_Point circumf[361];
	for(int i = 0; i < 360; i ++)
	{
		circumf[i].x = center.r * cos(px);
		circumf[i].y = center.r * sin(py);
	}
	SDL_RenderDrawPoints(ren, circumf, 360);
}

////////////// CLASS ANIMATION

animation::animation()
{
	loopCount = -1; // loop forever

}

animation::animation(SDL_Renderer * screen)
{
	loopCount = -1; // loop forever
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

bool animation::addImage(item * other)
{
	SDL_Texture * test = other->getImage();
	if(test)
	{
		images.push_back(test);
		image = test;
		return true;
	}
	else
	{
		std::cout << "The item you're trying to put into animation is null image\n";
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

void animation::setLoops(long long count)
{
	loopCount = count;
	loopDefault = count;
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
		int index = frameCount % images.size();
			
		if(images[index])
		{
			image = images[index];
		}
		else
		{
			SDL_Log("An animation image is loading as NULL!");
		}
		if(index == 0)
		{
			loopCount --;
			if(loopCount == 0)
			{
				if(onEndLoop)
				{
					onEndLoop(this);
				}
				loopCount = loopDefault;
			}
		}
	}
}

void animation::freeImages()
{
	if(owned)
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
		image = NULL;
		owned = false;
	}
}


void animation::setFPS(int FPS)
{
	desiredDelta = 1000 / FPS;
}

void animation::update(int tick)
{
	if(tick - oldTick > desiredDelta && loopCount)
	{
		next();
		oldTick = tick;
	}
}


////////////////////////////
// TILEMAP
//////////////////////////

tilemap::tilemap(SDL_Renderer * screen) : tilemap()
{
	ren = screen;
}
tilemap::tilemap() : item ()
{
	
}

tilemap::~tilemap()
{
	clear();
}

item * tilemap::get(int index)
{
	if(index < tiles.size() && index >= 0)
	{
		return tiles[index];
	}
	else
	{
		SDL_Log("Tried to access beyond tilemap index (%ld)\n", index);
		SDL_Log("Tiles range from 0 to %ld", tiles.size() - 1);
	}
	return NULL;
}

item * tilemap::last()
{
	if(tiles.empty())
	{
		std::cout << "Called \"last()\" on empty tilemap" << std::endl;
		return NULL;
	}
	return tiles[tiles.size() - 1];
}

void tilemap::addImage(std::string filePath, int w, int h)
{
	if(ren)
	{
		SDL_Surface * temp = IMG_Load(filePath.c_str());
		if(temp)
		{
			SDL_Texture * src = SDL_CreateTextureFromSurface(ren, temp);
			SDL_Texture * orig = SDL_GetRenderTarget(ren);
			int wBound = temp->w;
			int hBound = temp->h;
			SDL_FreeSurface(temp);
			for(int i = 0; i < hBound; i += h)
			{
				for(int k = 0; k < wBound; k += w)
				{
					tiles.push_back(new item(ren));
					last()->setSize(w, h);
					SDL_Texture * dest = SDL_CreateTexture(ren, 
						SDL_PIXELFORMAT_RGBA8888, 
						SDL_TEXTUREACCESS_TARGET, w, h);
					SDL_SetTextureBlendMode(dest, SDL_BLENDMODE_BLEND);
					SDL_SetRenderTarget(ren, dest);
					SDL_Rect subRect{k, i, w, h};
					SDL_RenderCopy(ren, src, &subRect, NULL);
					last()->setPos(i, k);
					last()->setImage(dest);
					if(!last()->getImage())
					{
						std::cout << "hmm. tilemap loaded a null image.\n";
					}
					SDL_SetRenderTarget(ren, orig);
				}
			}
			SDL_DestroyTexture(src);
		}
	}
	else
	{
		std::cout << "Cannot add image to tilemap while renderer is NULL: " << filePath << std::endl;
	}
}

void tilemap::draw()
{
	for(auto &n : tiles)
	{
		n->draw();
	}
}

void tilemap::clear()
{
	for(auto &n : tiles)
	{
		delete n;
	}
	if(image)
	{
		freeImage();
	}
}

int tilemap::size()
{
	return tiles.size();
}


////////////////////////////
// GROUP
////////////////////////////

group::group()
{

}

group::group(group & other)
{
	int len = other.items.size();
	for(int i = 0; i < len; i ++)
	{
		addRef(other.at(i));
	}
}
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

item * group::last()
{
	if(items.size() > 0)
	{
		return items[items.size() - 1];
	}
	return NULL;
}

item * group::at(int index)
{
	return items[index];
}

int group::size()
{
	return items.size();
}

group group::inRange(SDL_Rect * box)
{
	group temp;
	int len = items.size();
	for(int i = 0; i < len; i ++)
	{
		if(SDL_HasIntersection(items[i]->getPos(),  box))
		{
			temp.addRef(items[i]);
		}
	}
	return temp;
}

void group::free()
{
	int len = items.size();
	for(int i = 0; i < len; i ++)
	{
		item * thing = items[i];
		delete thing;
	}
}

////////////////////////////////////////
// Board Class
////////////////////////////////////

board::board()
{
	name = "board";
	request = "";
	fin = false;
	//Player = NULL;
	bkg = NULL;
	pause = false;
}

board::board(SDL_Renderer * rend) : board()
{
	ren = rend;
}


void board::draw()
{
	bkg->draw();
	drawn.draw();
	//Player->draw();
}

void board::move(int x, int y)
{
	drawn.move(-x, -y);
}

void board::handleEvent(SDL_Event * ev)
{

}

void board::init()
{
	fin = false;
	if(ren == NULL)
	{
		std::cout << "Called init on board with NULL renderer.\n";
	}
}

void board::togglePause()
{
	pause = !pause;
}





