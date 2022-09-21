#ifndef STICKMANWRAPPER
#define STICKMANWRAPPER

#include <item.h>
#include <unordered_map>

class explode : public item
{
	public:
	explode(SDL_Renderer * screen);
	void init();
	void boom(group & affected);

	public:
	int cycle;
	bool loop;
	SDL_Texture * bomb;
};

class stickperson : public item
{
	public:
	
	stickperson(SDL_Renderer * screen);
	void stickInit();
	void punch();
	void kick();
	void attack();
	void pushFrom(item * other);
	void jump();
	void stop();
	void accelerate(float x, float y);
	void update(int tick);
	// check for obstacles, and if in range for an attack
	bool awarenessCheck(item * other);
	void draw();
	void mapToAnimation(std::string action, item * src);


	public:
	bool isPunching;
	bool isKicking;
	int aggression;
	bool faceLeft;
	std::unordered_map <std::string, animation *> actions;
	SDL_Rect awarenessZone;
	SDL_Rect attackZone;
	int health;

	//Physics;
	float speedX;
	float speedY;
	float speedMaxX;
	float speedMaxY;
	float accelX;
	float gravity;
	float speedKnockBack;


};


class stickman : public stickperson
{
	public:
	stickman(SDL_Renderer * screen);
	void handleEvent(SDL_Event * ev);
	void update(int tick);
};

class world : public board
{
	public:
	world(SDL_Renderer * screen);
	void update(int tick);
	void handleEvent(SDL_Event * ev);
	void draw();

	public:

};


#endif
