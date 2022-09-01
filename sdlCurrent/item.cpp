#include "item.h"
#include <iostream>

TTF_Font * DefaultFont;
void itemInit()
{
	SDL_Init(SDL_INIT_EVERYTHING);
	TTF_Init();
	IMG_Init(IMG_INIT_PNG);
	Mix_Init(MIX_INIT_MP3 | MIX_INIT_OPUS | MIX_INIT_OGG);

	DefaultFont = TTF_OpenFont("res/fonts/roboto.ttf", 40);
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 6, 4096);
}

void itemQuit()
{
	TTF_CloseFont(DefaultFont);
	Mix_CloseAudio();
	Mix_Quit();
	IMG_Quit();
	TTF_Init();
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
	type = "item";
}

item::item(SDL_Renderer * screen) : item()
{
	ren = screen;
}

item::~item()
{
	if(image != NULL && owned)
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

SDL_Texture * item::getImage()
{
	return image;
}

void item::setImage(SDL_Texture * img)
{
	image = img;
}

void item::freeImage()
{
	// if this is specifically called, let's
	// not check against owned?
	// likely the cause of error if you use this incorrectly,
	// but the training wheels are off here...
	if(image != NULL)
	{
		SDL_DestroyTexture(image);
		image = NULL;
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

std::string item::toString()
{
	// what info do we need to reload this object?
	return "<item "; 
}


////////////// CLASS ANIMATION

animation::animation()
{
	type = "animation";
}

animation::animation(SDL_Renderer * screen) : animation()
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
		image = images[frameCount % (images.size() -1)];
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



///////////////////////////
// TILEMAP
//////////////////////////

tilemap::tilemap(SDL_Renderer * screen) : tilemap()
{
	ren = screen;
}
tilemap::tilemap() : item()
{
	type = "tilemap";
}

item * tilemap::get(int index)
{
	if(index < tiles.size() && index >= 0)
	{
		return tiles[index];
	}
	else
	{
		SDL_Log("Tried to access beyond tilemap index range\n");
		SDL_Log("Tiles range from 0 to %ld", tiles.size());
		return NULL;
	}
}

item * tilemap::last()
{
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
			int wbound = temp->w;
			int hbound = temp->h;
			SDL_FreeSurface(temp);
			for(int i = 0; i < hbound - h; i += h)
			{
				for(int k = 0; k < wbound; k += w)
				{
					tiles.push_back(new item(ren));
					last()->setSize(w, h);
					SDL_Texture * dest = SDL_CreateTexture(ren, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, w, h);
					SDL_SetTextureBlendMode(dest, SDL_BLENDMODE_BLEND);
					SDL_SetRenderTarget(ren, dest);
					SDL_Rect subRect{k, i, w, h};
					SDL_RenderCopy(ren, src, &subRect, NULL);
					last()->setImage(dest);
					SDL_SetRenderTarget(ren, orig);
				}
			}
			SDL_DestroyTexture(src);
		}
	}
	else
	{
		SDL_Log("Couldn't load %s", filePath.c_str());
	}
}


///////////////////////////
// LABEL
///////////////////////////

label::label()
{
	type = "label";
	font = DefaultFont;
	textChanged = NULL;
	color = {35, 45, 55, 255};
}

label::label(SDL_Renderer * screen) : label()
{
	ren = screen;
}

void label::render()
{
	if(image && owned)
	{
		SDL_DestroyTexture(image);
	}
	if(font && !(text.empty()))
	{
		SDL_Surface * temp = TTF_RenderText_Solid(font, text.c_str(), color);
		if(temp)
		{
			image = SDL_CreateTextureFromSurface(ren, temp);
			pos.w = temp->w;
			pos.h = temp->h;
			SDL_FreeSurface(temp);
		}
	}
}

void label::setText(std::string textStr)
{
	text = textStr;
	render();
	if(textChanged)
	{
		textChanged(this);
	}
}

// this function should only be used
// if you are worried about public access
// to the text string. 
// It can be skipped if you choose to use setText()
// every time.
void label::update(int tick)
{
	if(oldText != text)
	{
		oldText = text;
		render();
		if(textChanged)
		{
			textChanged(this);
		}
	}

};




////////////////////////////
// GROUP
////////////////////////////
group::group()
{
	type = "group";
}
group::group(SDL_Renderer * screen): group()
{
	ren = screen;
}

void group::draw()
{
	int len = items.size();
	for(int i = 0; i < len; i ++)
	{
		items[i]->draw();
	}
}

void group::addRef(item * other)
{
	items.push_back(other);
}

void group::move(int x, int y)
{
	int len = items.size();
	for(int i = 0; i < len; i ++)
	{
		items[i]->move(x, y);
	}
}

void group::update(int tick)
{
	int len = items.size();
	for(int i = 0; i < len; i ++);
}


////////////////////////////////////////
// Board Class
////////////////////////////////////

board::board(SDL_Renderer * rend)
{
	ren = rend;
	int w, h;
	SDL_GetRendererOutputSize(ren, &w, &h);
	Player.setPos(w/2 - 32, h/2 - 32);
	bkg.setSize(w, h);
	bkg.setPos(0, 0);
	type = "board";
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







