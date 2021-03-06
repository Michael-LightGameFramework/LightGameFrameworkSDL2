#ifndef IMAGESANDITEMS
#define IMAGESANDITEMS

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_net.h>
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
	bool isClicked(int x, int y);
	circle getCenter();
	void setCenter(int x, int y, int r);
	void draw(double angle);
	virtual void draw();
	virtual void update(int tick);
	SDL_Texture * getImage();

	protected:
	SDL_Renderer * ren;
	SDL_Texture * image;
	SDL_Rect pos;
	circle center;
	int oldTick;

};

class animation : public item
{
	public:
	animation();
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

// Basic text output
class label : public item
{
	public:
	label(SDL_Renderer * rend, TTF_Font * fnt , std::string text, int x, int y);
	bool setFont(TTF_Font * fnt);
	void setText(std::string txt);
	TTF_Font * getFont();
	bool buildImage();
	void setColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a);

	private:
	TTF_Font * font;
	SDL_Color textColor;
	std::string words;
	item bkg;
};

class textInput : public item
{
	public:
	textInput(SDL_Renderer * rend);

	private:
	label txt;
	item bkg, bkgInset, bkgOutset;
};



class box : public item
{
	public:
	box(SDL_Renderer *rend, int x, int y, int w, int h);
	void setColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a);
	virtual void draw();

	private:
	double max, min, val;
	Uint8 rbox, gbox, bbox, abox;
};

// Create a bar graph out of given data points.
// Each value input should also be given a string name, name may be a date,
// 	a place, a description. 
// todo: sort by name, sort by value
class barGraph : public item
{
	public:
	barGraph(SDL_Renderer *rend, int x, int y, int w, int h);
	// alpha is ignored unless user calls SDL_SetRenderDrawBlendMode(...);
	void setBarColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a);
	void setBkgImage(std::string imagePath);
	void addValue(std::string name, double val);
//	box * getBox(int x, int y); // get the box for witch the points collide.
	std::string getLabel(int index);
	void refresh();

	private:
	item bkg;
	double maxVal, minVal;
	int sampleCount;
	// Consider using a map instead of two vectors...
	std::vector <std::string> name;
	std::vector <double> value;
	Uint8 rbar, gbar, bbar, abar;
};


class webText : public item
{
	public:
	webText(std::string siteLink);
	std::string get(std::string wholeLink);
	~webText();
	private:
	std::string link, cmd;
	IPaddress ip;
	TCPsocket client;
	bool connected;
};

// may also be used for loot boxes and item interaction.
class npc : public item
{

	public:
	npc(SDL_Renderer * rend);
//	void addTalk(prompt talk);
	void playTalk();
	void hurt(double atk);
	void heal(double val);
	void kill(); // play animation of npc... endinga

	private:
	double maxHp, hp, atk, def;
//	std::vector <prompt> talks;
	item profile; // headshot image while prompt talking
	int activeTalk;
};									



#endif
