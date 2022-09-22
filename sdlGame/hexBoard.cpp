#include <hexBoard.h>



item * spawn(SDL_Renderer * screen)
{
	return new hexBoard(screen);
}

void destroy(item * key)
{
	delete (hexBoard*) key;
}


//////////////////////////////////////////
// HEXBOARD
//////////////////////////////////////


hexBoard::hexBoard(SDL_Renderer * screen) : board(screen)
{
	layerIndex = 0;
	srcIndex = 0;
	snapping = true;
	stamping = false;
	int winW, winH;
	SDL_GetRendererOutputSize(ren, &winW, &winH);

	name = "test";
	bkg = new animation(screen);
	bkg->loadImage(absPath() + "res/images/OverBkg0.png");
	bkg->setPos(0, 0);
	bkg->setSize(winW, winH);

	tileSrc.setRenderer(screen);
	tileSrc.addImage(absPath() + "res/images/hexGrid.png", 32, 48);
	//setSnapSize(15, 11);

	bool isOffset = false;
	for(int i = 0; i < 6; i ++)
	{
		layers.push_back(new group);
		for(int j = 0; j < 1000; j += 42)
		{
			for(int k = 0; k < 1900; k += 64)
			{
				item * last = new item(screen);
				last->shown = false;
				last->setPos(k + (isOffset * 32), j);
				last->setSize(64, 96);
				layers[i]->addRef(last);
			}
			isOffset = !isOffset;
		}
	}
	cursor = tileSrc.get(srcIndex);
}

hexBoard::~hexBoard()
{
	for(int i = 0; i < layers.size(); i ++)
	{
		layers[i]->free();
	}
	layers.clear();
	if(bkg)
	{
		delete bkg;
	}
	
}
void hexBoard::init()
{
	fin = false;
}

void hexBoard::snapToGrid(item * tile)
{
	group * currentLayer = layers[layerIndex];
	int len = currentLayer->size();
	for(int i = 0; i < len; i ++)
	{
		if(SDL_HasIntersection(currentLayer->at(i)->getPos(), tile->getPos()))
		{
			hovered = currentLayer->at(i);
			if(hovered)
			{
				memcpy(tile->getPos(), hovered->getPos(), sizeof(SDL_Rect));
				break;
			}
		}
	}
}

void hexBoard::setBoardSize(int tilesW, int tilesH)
{
}

void hexBoard::moveCursorTo(int x, int y)
{
	if(cursor)
	{
		cursor->getPos()->x = x;
		cursor->getPos()->y = y;
		if(snapping)
		{
			snapToGrid(cursor);
		}
	}
}

void hexBoard::stamp()
{
	if(cursor)
	{
		if(hovered)
		{
			hovered->setImage(cursor->getImage());
			hovered->shown = true;
		}
	}
}

void hexBoard::update(int tick)
{
	for(int i = 0; i < layers.size(); i ++)
	{
		layers[i]->update(tick);
	}
}

void hexBoard::handleEvent(SDL_Event * ev)
{
	switch(ev->type)
	{
		case SDL_KEYDOWN:
			switch(ev->key.keysym.sym)
			{
				case SDLK_s:
					snapping = !snapping;
					break;
				case SDLK_UP:
					if(layerIndex < 5)
					{
						layerIndex ++;
					}
					break;
				case SDLK_DOWN:
					if(layerIndex > 0)
					{
						layerIndex --;
					}
					break;
			}
			break;
		case SDL_MOUSEMOTION:
			moveCursorTo(ev->motion.x, ev->motion.y);
			if(stamping)
			{
				if(cursor)
				{
					stamp();
				}

			}
			break;
		case SDL_MOUSEBUTTONDOWN:
			if(cursor)
			{
				stamp();
			}
			stamping = true;
			break;
		case SDL_MOUSEBUTTONUP:
			stamping = false;
			break;
		case SDL_MOUSEWHEEL:
			if(tileSrc.size() > 0)
			{
				if(ev->wheel.y < 0)
				{
					srcIndex ++;
					if(srcIndex >= tileSrc.size())
					{
						srcIndex = 0;
					}
				}
				else if(ev->wheel.y > 0)
				{
					srcIndex --;
					if(srcIndex < 0)
					{
						srcIndex = 0;
					}
				}
				SDL_Rect * temp = cursor->getPos();
				cursor = tileSrc.get(srcIndex);
				cursor->setPos(temp->x, temp->y);
				cursor->setSize(temp->w, temp->h);
			}
			else
			{
				SDL_Log("Tileset not loaded yet!\n");
			}
			break;

	}
}

void hexBoard::draw()
{
	bkg->draw();
	int len = layers.size();
	for(int i = 0; i < len; i ++)
	{
		layers[i]->draw();
	}
	if(cursor)
	{
		cursor->draw();
	}
}

