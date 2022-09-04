#ifndef LEVELONEARRIVAL
#define LEVELONEARRIVAL
#include <item.h>

class player : public item
{
	public:
	player();
	player(SDL_Renderer * screen);
	void accelerate(bool left, bool right, bool up, bool down);
	void update(int tick);
	void draw();

	public:
	double angle;
	bool faceLeft;
	float speedX;
	float speedY;
	float accelX;
	float accelY;
	float maxX;
	float maxY;

};

class spawner: public group
{
	public:
	spawner();
	spawner(SDL_Renderer * screen);
	~spawner();
	void spawn();
	void drawSelf();
	void setSize(int w, int h);
	void update(int tick);
	void remove(int index);
	void clear();
	item * checkCollision(item * other);
	void fastForward(int ticks);
	
	int (*moveXFunc)(int x);
	int (*moveYFunc)(int y);
	void (*onEmpty)(item *);

	public:
	int everyNMilisec;
	item * base;
	SDL_Rect bounds;
	int engine;
	int speedX;
	int speedY;
	SDL_Rect particleSize;
	bool spawning;
	int gravidity;

};


class level1 : public board
{
	public:
	level1(SDL_Renderer * screen);
	~level1();
	// use to initialize begin state
	void init();
	void handleEvent(SDL_Event * ev);
	void update(int tick);
	void draw();

	void (*giveGoal)(item *);

	// onDeath();

	public:
	bool up;
	bool down;
	bool left;
	bool right;
	bool pause;
	double angle;
	int maxGravidity;
	SDL_Rect windowSize;

	item bkg;
	item goal;
	animation * refugee;
	spawner refugeeSpawn;
	animation * star;
	spawner starSpawn;
	player ship;
	

};





#endif
