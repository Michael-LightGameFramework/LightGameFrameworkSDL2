#ifndef IMAGESANDITEMS
#define IMAGESANDITEMS

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <string>
#include <vector>
#include <unordered_map>

void itemInit();
void itemQuit();

enum Direction {Up, Down, Left, Right};

std::string absPath();

struct circle
{
	int x, y, r;
};

class item
{
	public:
	item();
	item(SDL_Renderer * screen);
	virtual ~item();
	void setRenderer(SDL_Renderer * renderer);
	bool loadImage(std::string filename);
	void freeImage();
	virtual void setSize(int w, int h);
	void setPos(int x, int y);
	SDL_Rect * getPos();
	virtual void move(int x, int y);
	bool getCollision(item* other);
	virtual bool isClicked(int x, int y);
	circle getCenter();
	void setCenter(int x, int y, int r);
	virtual void draw(double angle);
	virtual void drawHFlip();
	virtual void draw();
	virtual void update(int tick);
	SDL_Texture * getImage();
	void setImage(SDL_Texture * img);

	protected:
	SDL_Renderer * ren;
	SDL_Texture * image;
	SDL_Rect pos;
	circle center;
	int oldTick;
	bool owned; // do we need to dealocate image?

};

class animation : public item
{
	public:
	animation();
	animation(SDL_Renderer * screen);
	virtual ~animation();
	bool addImage(std::string filePath);
	bool addImage(item * other);
	bool loadAnimation(std::string pref, std::string zbuf, \
			std::string ext);
	void next();
	void freeImages();
	void setFPS(int FPS);
	virtual void update(int tick);

	protected:
	int frameCount;
	std::vector <SDL_Texture *> images;
	int desiredDelta;


};

class tilemap : public item
{
        public:
        tilemap();
        tilemap(SDL_Renderer * screen);
        item * get(int index);
        // tilemap image, tile w, tile h
        void addImage(std::string filePath, int w, int h);
        void clear();
        // get the last tile in the set.
        item * last();

        public:
        std::vector <item *> tiles;


};


class group : public item
{
	public:
	virtual void draw();
	virtual void draw(double angle);
	virtual void addRef(item * other);
	virtual void move(int x, int y);
	std::vector <item *> getBoundedItems(SDL_Rect bounds);
	item * last();

	public:
	std::vector <item *> items;

};


class board : public item
{
	public:
	board();
	board(SDL_Renderer * rend);
	// populate board with tiles, set to default state
	virtual void init();
	virtual void draw();
	void move(int x, int y);
	virtual void handleEvent(SDL_Event * ev);
	virtual void togglePause();


	public:
	std::string name;
	std::string request;
	// set fin = true when board is done running, also set request to new board, or leave Null to get default
	bool fin;
	bool pause;
	item *Player;
	animation *bkg;
	group drawn;
	group click;
	group collide;

	bool left, right, up, down; // direction keys, handleEvent

};


extern "C"
{
	extern item * spawn(SDL_Renderer * screen);
	extern void destroy(item * key);
}


#endif
