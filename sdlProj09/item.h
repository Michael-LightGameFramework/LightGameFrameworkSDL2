#ifndef IMAGESANDITEMS
#define IMAGESANDITEMS

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
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
	virtual ~item();
	void setRenderer(SDL_Renderer * renderer);
	bool loadImage(std::string filename);
	void freeImage();
	void setSize(int w, int h);
	void setPos(int x, int y);
	SDL_Rect * getPos();
	void move(int x, int y);
	bool getCollision(item* other);
	circle getCenter();
	void setCenter(int x, int y, int r);
	void draw(double angle);
	void draw();

	protected:
	SDL_Renderer * ren;
	SDL_Texture * image;
	SDL_Rect pos;
	circle center;

};

class animation : public item
{
	public:
	animation();
	bool addImage(std::string filePath);
	void next();

	protected:
	int frameCount;
	std::vector <SDL_Texture *> images;


};











#endif
