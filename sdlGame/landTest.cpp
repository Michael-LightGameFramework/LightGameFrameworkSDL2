#include <landTest.h>

item * spawn(SDL_Renderer * screen)
{
	return new landTest(screen);
}

void destroy(item * key)
{
	delete (landTest*) key;
}

landTest::landTest()
{

}

landTest::landTest(SDL_Renderer * screen)
{
	name = "test";

	thing = new platform(screen);
	thing->setPos(100, 100);
	thing->setSize(500, 100);
	((platform*)thing)->moveBetween(1300, 400);

	moon = new item(screen);
	moon->loadImage("res/images/MoonDog.png");
	moon->setPos(-12, -7);
	moon->setSize(250, 220);

	drawn.addRef(moon);
	drawn.addRef(thing);


}

void landTest::init()
{

}


landTest::~landTest()
{
	delete thing;
	delete moon;
}

void landTest::update(int tick)
{
	thing->update(tick);
}

void landTest::draw()
{
	drawn.draw();
}




