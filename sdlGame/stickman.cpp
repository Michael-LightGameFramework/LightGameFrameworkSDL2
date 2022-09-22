#include <stickman.h>




item * spawn(SDL_Renderer * screen)
{
	return new world(screen);
}

void destroy(item * key)
{
	delete (world*) key;
}

/////////////////////////////////
// EXPLODE
////////////////////////

explode::explode(SDL_Renderer * screen) : item(screen)
{
	init();
}

void explode::init()
{
	cycle = 2000;
	loop = true;
	loadImage("explode.png");
	bomb = image;
	loadImage("litBomb.png");
}

void explode::boom(group & affected)
{

}

//////////////////////////////////
// STICKPERSON
/////////////////////////////////

stickperson::stickperson(SDL_Renderer * screen)
{
	ren = screen;
	stickInit();
}

void stickperson::stickInit()
{
	aggression = 2;
	health = 18;
	faceLeft = true;
	speedMaxX = 7;
	speedMaxY = 12;
	accelX = .8;
	gravity = 1;
	speedKnockBack = 7;
};

void stickperson::punch()
{
	isPunching = true;

}

void stickperson::kick()
{
	isKicking = true;

}

void stickperson::attack()
{

}

void stickperson::pushFrom(item * other)
{
	speedX = 20/(pos.x - other->getPos()->x);
}

void stickperson::jump()
{

}

void stickperson::stop()
{
	isPunching = false;
	speedX = 0;
	speedY = 0;
}

void stickperson::accelerate(float x, float y)
{

}

void stickperson::update(int tick)
{

}

bool stickperson::awarenessCheck(item * other)
{
	// the thinking is if there is something
	// that they should be aware of, then they should...
	// OK, going in the right direction, 
	// but this is more likely to be taken care of in
	// update()...
	return false;
}

void stickperson::draw()
{

}

void stickperson::mapToAnimation(std::string action, item * src)
{

}

///////////////////////////////////
// STICKMAN
/////////////////////////////////

stickman::stickman(SDL_Renderer * screen) : stickperson(screen)
{
}

void stickman::handleEvent(SDL_Event * ev)
{

}

void stickman::update(int tick)
{
	
}



/////////////////////////////////
// WORLD
////////////////////////////////

world::world(SDL_Renderer * screen)
{
	name = "fight";
	ren = screen;
	bkg = new animation(screen);
	bkg->loadImage(absPath() + "res/images/OverBkg0.png");

}


void world::update(int tick)
{
	
}
void world::handleEvent(SDL_Event * ev)
{

}
void world::draw()
{
	bkg->draw();

}

