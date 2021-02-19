
// Board game engine

#include <string.h>
#include <string>
#include <iostream>
#include <vector>

#include "item.h"


const int NUMSOLDIERS = 7;
const int NUMPLAYERS = 4;
const int NUMPADS = 12;

class pad : public item
{
	public:
	pad(SDL_Renderer * ren, std::string imagePath);
	// uni-directional links
	void addLinkTo(pad * other);
	void requestLinkFrom(pad * other);
	// two-way linking
	void link(pad * other);
	bool isLinkedTo(pad * other);
	private:
	std::vector <pad*> links;
	std::vector <item *> pieces;
};

pad::pad(SDL_Renderer * ren, std::string imagePath)
{
	this->ren = ren;
	loadImage(imagePath);
}


void addLinkTo(pad * other)
{
	links.push_back(other);
}

void requestLinkFrom(pad * other)
{
	other.addLinkTo(this);
}

void link(pad * other)
{
	addLinkTo(other);
	requestLinkFrom(other);
}

bool isLinkedTo(pad * other)
{
	for(int i = 0; i < links.size(); i ++)
	{
		if(links[i] == other)
		{
			return true;
		}
	}
	return false;
}




class group
{
	public:
	void add(item * thing);
	void add(group * other);
	void remove(item * thing);
	bool contains(item * thing);
	group * inRect(SDL_Rect boarder);
	group * inRect(int x, int y, int w, int h);
	group * inCircle(circle bounds);
	group * getClicked(int x, int y);
	item * at(int index);
	int size();
	void draw();

	private:
	std::vector <item *> stuff;
};



void group::add(item * thing)
{
	if(!(contains(thing)))
	{
		stuff.push_back(thing);
	}
}

void group::add(group * other)
{
	for(int i = 0; i < other.size(); i ++)
	{
		add(other->at(i));
	}
}

void group::remove(item * thing)
{
	for(int i = 0; i < stuff.size(); i ++)
	{
		if(stuff[i] == thing)
		{
			stuff.erase(i);
		}
	}
}

bool group::contains(item * thing)
{
	for(int i = 0; i < stuff.size(); i ++)
	{
		if(stuff[i] == thing)
		{
			return true;
		}
	}
	return false;
}

item * group::at(int index)
{
	return stuff[index];
}

int group::size()
{
	return stuff.size();
}

group * group::inRect(SDL_Rect * boarder)
{
	return SDL_HasIntersection(this->pos, boarder)
}

group * group::inRect(int x, int y, int w, int h)
{
	SDL_Rect boarder(x, y, w, h);
	return SDL_HasIntersection(this->pos, &boarder)

}

group * group::inCircle(circle bounds)
{
	
}

group * group::touches(int x, int y)
{

}




int main(int argc, char ** arg)
{
	itemInit();
	SDL_Window * win = SDL_CreateWindow("Game Builder", 30, 30, 2048, 1152, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
	SDL_Renderer * screen = SDL_CreateRenderer(win, -1, 0);

	bool run = true;
	SDL_SetRenderDrawColor(screen, 30, 180, 20, 255);
	int fps = 25;
	int desiredDelta = 1000 / fps;

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
							break;
						case SDLK_DOWN:
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
