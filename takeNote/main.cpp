#include <fstream>
#include <string>
#include <string.h>
#include <iostream>
#include "item.h"


class textBox : item
{
	public:
	textBox(SDL_Renderer * rend, TTF_Font * fnt, int x, int y, int w, int h);
	bool handleEvent(SDL_Event * ev);
	void wrapText(bool);
	void scroll(int);
	void move(int);
	void addText(std::string); // add text at the cursor
	bool hasFocus();
	void setFont(TTF_Font * fnt);
	void setFontSize(int fntSize);

	void buildImage(); // only call when a change is made (new font, new text, new color, etc);
	private:
	void populate();// set up correct number of label lines for box size
	std::vector <label> out;
	std::vector <std::string> line;
	int currentLine, cursor, topLine, fontSize; // x and y positions in the text
	bool wrap, focus;
	TTF_Font * font;
};

textBox::textBox(SDL_Renderer * rend, TTF_Font * fnt,int x, int y, int w, int h)
{
	ren = rend;
	rect.x = x;
	rect.y = y;
	rect.w = w;
	rect.h = h;
	font = fnt;
	int tmpy = y;
	populate();
}


textBox::populate()
{
	for(int i = 0; i < 100; i ++)
	{
		out.push_back(new label(rend, font, "", rect.x, tmpy));
		tmpy += fontSize + fontSize/3;
		if(tmpy > h)
		{
			break;
		}
	}
}

bool textBox::handleEvent(SDL_Event * ev)
{
	if(!hasFocus())
	{
		return false; // did not handle event
	}
	switch(ev->type)
	{
		case SDL_MOUSECLICK:

	}

}

using namespace std;
int main()
{
	itemInit();
	cout << SDL_GetPlatform();
	SDL_Window * win = SDL_CreateWindow("Take Note: newFile.txt", 0, 0, 1200, 100, SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALWAYS_ON_TOP | \
			SDL_WINDOW_SKIP_TASKBAR); 
	int winH, winW;
	SDL_Renderer * screen = SDL_CreateRenderer(win, -1, 0);
	bool run = true;
	SDL_StartTextInput();
	TTF_Font * acme = TTF_OpenFont("Acme/Acme-Regular.ttf", 30);
	for(int i = 0; i < 20; i ++)
	{
		line.push_back("Hi");
		out.push_back(new label(screen, acme, line[i], 20, i * 40));
		out[i]->buildImage();
	}

	SDL_GetRendererOutputSize(screen, &winW, &winH); // get the drawable size of the renderer/window

	int currentLine = 0;
	int topLine = 0;
	int cursor = 0;
	while(run)
	{
		
		SDL_Event ev;
		while(SDL_PollEvent(&ev))
		{
			switch(ev.type)
			{
				case SDL_WINDOWEVENT:
					switch(ev.window.event)
					{
						case SDL_WINDOWEVENT_FOCUS_LOST:
								SDL_SetWindowOpacity(win, 0.2);
								SDL_SetWindowBordered(win, SDL_FALSE);
							break;
						case SDL_WINDOWEVENT_FOCUS_GAINED:
								SDL_SetWindowOpacity(win, 0.6);
								SDL_SetWindowBordered(win, SDL_TRUE);
							break;
						case SDL_WINDOWEVENT_CLOSE:
							run = false;
							break;
						case SDL_WINDOWEVENT_RESIZED:
							SDL_GetRendererOutputSize(screen, &winW, &winH);

							break;
					}
					break;
				case SDL_TEXTINPUT:
					if(currentLine > line.size() - 1)
					{
						line.push_back("");
					}
					line[currentLine] += ev.text.text;
					cursor += strlen(ev.text.text);
					
					out[currentLine]->setText(line[currentLine]);
					out[currentLine]->buildImage();

					break;
				case SDL_KEYDOWN:
					switch(ev.key.keysym.sym)
					{
						case SDLK_BACKSPACE:
							line[currentLine] = line[currentLine].substr(cursor - 1, line[currentLine].size() - cursor - 1);
						break;
						case SDLK_DOWN:
							currentLine ++;
							topLine ++;
							if(currentLine > line.size() - 1)
							{
								line.push_back("do");
							}
						break;
						case SDLK_UP:
							currentLine --;
							if(currentLine < topLine)
							{
								topLine --;
								if(topLine < 0)
								{
									topLine = 0;
									currentLine = 0;
								}
							}
						break;
						case SDLK_LEFT:
							cursor --;
							if(cursor < 0)
							{

							}
						break;
						case SDLK_RIGHT:
							cursor ++;

						break;

					}


			}
		}
		SDL_RenderFillRect(screen, NULL);

		for(int i = 0; i < out.size(); i ++)
		{
			out[i]->draw();
			if(out[i]->getPos()->y > winH)
			{
				break;
			}
		}

		SDL_RenderPresent(screen);
		SDL_Delay(100);
	}
	SDL_StopTextInput();
	SDL_DestroyRenderer(screen);
	SDL_DestroyWindow(win);
	itemQuit();
}
