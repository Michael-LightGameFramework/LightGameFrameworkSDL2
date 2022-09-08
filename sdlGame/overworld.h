#ifndef THEDEFAULTOVERWORLDEXPLORE
#define THEDEFAULTOVERWORLDEXPLORE

#include <item.h>
#include <unordered_map>

class player : public item
{
	public:
	player(SDL_Renderer * screen);
	void mapAnimation(std::string name, int index);
	void walkTo(int x, int y);
	void update(int tick);
	void draw();
	// void activate(); // check facing tile for activity
	void setAnimation(std::string keyword);
	

	public:
	tilemap frames;
	bool faceLeft;
	std::unordered_map <std::string, animation*> actions;
	animation * currentAnimation;

};

class overworld : public board
{
	public:
	overworld(SDL_Renderer * screen);
	void init();
	void handleEvent(SDL_Event * ev);
	void draw();
	void update(int tick);

	public:
	int speedX;
	int speedY;
	group land;
	group bridges;
	player * jones;

};


#endif
