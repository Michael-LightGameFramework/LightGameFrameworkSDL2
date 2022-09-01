#ifndef IMAGESANDITEMS
#define IMAGESANDITEMS

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <string>
#include <vector>

void itemInit();
void itemQuit();

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
	SDL_Texture * getImage();
	void setImage(SDL_Texture * img);
	void freeImage();
	void setSize(int w, int h);
	void setPos(int x, int y);
	SDL_Rect * getPos();
	void move(int x, int y);
	bool getCollision(item* other);
	bool isClicked(int x, int y);
	circle getCenter();
	void setCenter(int x, int y, int r);
	void draw(double angle);
	void draw();
	virtual void update(int tick);
	std::string toString();

	protected:
	SDL_Renderer * ren;
	SDL_Texture * image;
	bool owned;
	SDL_Rect pos;
	circle center;
	int oldTick;
	std::string type;
};

class animation : public item
{
	public:
	animation();
	animation(SDL_Renderer * screen);
	virtual ~animation();
	bool addImage(std::string filePath);
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

// text output
class label : public item
{
	public:
	label();
	label(SDL_Renderer * screen);
	void render();
	void update(int tick);
	void setText(std::string newText);

	void (*textChanged)(item *);

	public:
	TTF_Font * font;
	std::string text;
	std::string oldText;
	SDL_Color color;
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
	group();
	group(SDL_Renderer * screen);
	void draw();
	void addRef(item * other);
	void move(int x, int y);
	void update(int tick);

	public:
	std::vector <item *> items;

};


class board : public item
{
	public:
	board(SDL_Renderer * rend);
	void draw();
	void move(int x, int y);

	public:
	item Player;
	item bkg;
	group drawn;
	group click;
	group collide;

};




#endif
