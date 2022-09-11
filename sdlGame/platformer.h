#ifndef PLATFORMERBASEWRAPPER
#define PLATFORMERBASEWRAPPER

#include <item.h>

class platform : public item
{
	public:
	platform(SDL_Renderer * screen);
	void bust();
	void loadBustAnimation(std::string, std::string, std::string);
	void moveBetween(int x, int y);
	void update(int tick);

	public:
	float speedX;
	float speedY;
	float offsetX;
	float offsetY;
	int startX;
	int startY;
	float destX;
	float destY;
	bool crushable;
	bool tide;
	bool moving;
	bool busting;
	animation * busted;
};

class spawner : public item
{
	public:
	spawner(SDL_Renderer * screen);
	// how often a particle is spawned given time tick, and return true to activate
	bool (*frequency)(int tick);
	void (*moveX)(int tick);
	void (*moveY)(int tick);
	void update(int tick);
	void draw();
	void spawn();
	bool isAbove(item * other);
	bool isBelow(item * other);
	bool isLeft(item * other);
	bool isRight(item * other);

	public:
	bool spawning; 
	int gravidity; 
	group children;
	SDL_Rect bounds;

};


// compatible with player character class
class shooter: public item
{
	public:
	void update(int tick);

	public:
	bool faceLeft;
	spawner gun1;
};


class enemy : public item
{
	public:
	enemy(SDL_Renderer * screen);
	void stepPath();
	void update();

	public:
	std::vector <SDL_Point> paths;
	bool hunting; // if they interact with player, do they track to it?

};

class player : public item
{
	public:
	player(SDL_Renderer * screen);
	bool boxCollision(item * other);
	bool walkTo(int x, int y);

	
};

class platformLevel : public board
{
	public:
	platformLevel(SDL_Renderer * screen);
	void draw();

	public:
	player jones;

};


#endif


/*
//////////////////////////////
// To Do
//////////////////////////////

Fading platforms, or platforms with a top level that can fade out for secret paths
  - would also require more advanced hitBoxing for when to fade, when to let walkthrough...
  - maybe have new logic for left wall, right wall, etc.


*/
