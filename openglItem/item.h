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

static int ID;
class item
{
	public:
	item();
	item(SDL_Renderer * renderer, std::string loadImg);
	virtual ~item();
	item * getSelf();
	void setRenderer(SDL_Renderer * renderer);
	bool loadImage(std::string filename);
	virtual void freeImage();
	void setSize(int w, int h);
	void setPos(int x, int y);
	SDL_Rect * getPos();
	virtual void move(int x, int y);
	virtual bool getCollision(item* other);
	virtual bool isClicked(int x, int y);
	virtual circle getCenter();
	virtual void setCenter(int x, int y, int r);
	virtual void draw(double angle);
	virtual void stamp(item * paper, int x, int y);
	virtual void draw();
	virtual void update(int tick);
	SDL_Texture * getImage();

	// some virtual functions for plugins
	//
	// take a second to load resources, populate board, etc
	virtual void preloop();	
	// Make sure all threads are ready to run before going to user interaction
	virtual bool ready();
	// sent only on event detected 
	virtual void handleEvent(SDL_Event * ev);
	// clean up gracefully before a close. Might be a reset to menu
	// so it should be a clean exit of the plugin.
	virtual void cleanup();

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
	void kill(); // play animation of npc... ending.

	private:
	double maxHp, hp, atk, def;
//	std::vector <prompt> talks;
	item profile; // headshot image while prompt talking
	int activeTalk;
};									



class grid : public item
{

	public:
	grid(SDL_Renderer * rend, int tx, int ty);
	
	void fill(item * brush);
	// use stamp for static background images 
	void stamp(item * brush, int px, int py);
	// use set for animations, collision, and interactive tiles
	void set(item * tile, int tx, int ty);
	
	

	private:
	std::vector <std::vector <item *> > tiles;

};



class audioPlayer
{
	public:
	audioPlayer();
	SDL_AudioSpec getSpec();
	void setSpec(SDL_AudioSpec newSpec);
	void loadWav(std::string path);
	// loads raw data, assumes you the user know what settings for the specs
	void loadRaw(std::string path);
	void loadRaw(std::vector <Uint8> data);
	void play();
	// stop the music, allow to restart from here
	void pause();
	// stop the music and reset to beginning
	void stop();
	// stop the music and clear the buffers
	void clear(); 

	private:
	SDL_AudioSpec specs;
	SDL_AudioDeviceID dev;
	std::vector <Uint8> data;
};

class recorder
{
	public:
	// defaults to a data-saver spec
	recorder();
	~recorder();
	SDL_AudioSpec getSpec();
	void setSpec(SDL_AudioSpec newSpec);
	audioPlayer * getPlayer();


	void record();
	void pause();
	void resume();
	void play();
	void clear();
	std::vector <Uint8> getData();
	void save(std::string filePath);


	private:
	SDL_AudioSpec specs;
	SDL_AudioDeviceID dev;
	std::vector <Uint8> data;
	// SDL states that a player device should be opened
	// before a recording device, so now we have an internal
	// audioplayer for playback.
	audioPlayer *player;


};


//extern "C" {item* create();}
// extern "C" {void destroy(item * terminate);}

#endif
