// Example of opening a window in a separate thread that is controlled by
// the plugin's object.
#include "../../item.h"
#include <iostream>


class duck : public item
{
	public:
	duck();
	void preloop();
	bool ready();
	void cleanup();
	void loop();
	void draw();

	// Image assets and renderer should be assigned
	// inside of the thread, so most of the work will
	// be done in the function that is being threadified, 
	// in this case, the loop function.
	friend int threadifyLoop(void * ptr);

	void handleEvent(SDL_Event * ev);


	private:
	SDL_Window * window;
	item button;
	item bkg;
	int windID;
	bool run;
	bool hasEvent;
	SDL_Event * event;
	SDL_Thread * thread;
};



duck::duck()
{
	thread = NULL;
	run = false;
}

void duck::draw()
{
	
}
bool duck::ready()
{
	return run;
}

void duck::loop()
{
	run = false;
	hasEvent = false;
	event = NULL;
	window = SDL_CreateWindow("Ducky", 900, 100, 500, 500, SDL_WINDOW_SHOWN);
	windID = SDL_GetWindowID(window);
	ren = SDL_CreateRenderer(window, -1, 0);

	button.setPos(30, 30);
	button.setSize(90, 40);
	button.setRenderer(this->ren);
	button.loadImage("res/ducky.png");

	run = true;
	while(run)
	{
		if(hasEvent)
		{

			if(this->event->type == SDL_QUIT)
			{
				run = false;
			}
			else if(this->event->type == SDL_KEYDOWN)
			{
				if(this->event->key.keysym.sym == SDLK_DOWN)
				{
					button.setPos(rand() % 500, rand() % 500);
				}
			}
			hasEvent = false;
		}
	
		button.move(rand()%4 - 2, rand()%4-2);
		SDL_RenderClear(this->ren);
		button.draw();
		SDL_RenderPresent(this->ren);
		SDL_Delay(42);
		std::cout << hasEvent;
	}
}

int threadifyLoop(void * ptr)
{
	duck * goose = (duck *) ptr;
	goose->loop();
	return 0;
}

void duck::preloop()
{
	// going to set this window to a new thread here if needed.
	// we'll see if new window is non-blocking by default...
	this->thread = SDL_CreateThread(threadifyLoop, "testWindowDucky", (void*) this);

}

void duck::handleEvent(SDL_Event * ev)
{
	this->event = ev;
	hasEvent = true;
}

void duck::cleanup()
{
	SDL_Event close;
	close.type = SDL_QUIT;
	handleEvent(&close);
	run = false;
	int retval = 0;
	SDL_WaitThread(this->thread, &retval);
	SDL_DestroyWindow(window);
}

extern "C" item * create()
{

	return new duck;
}

extern "C" void destroy(item * done)
{
	
	delete(done);
	done = NULL;
}
