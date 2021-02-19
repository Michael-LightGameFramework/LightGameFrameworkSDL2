#include <sstream>
#include <string>
#include <ctime>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <vector>

#include "../item.h"
#include "stonks.h"

/* Functions moved to "stonks.h"
std::string getDateStr(tm * gmt)
bool isBusinessDay(tm * gmt);
*/

void toAllCaps(char * str)
{
	for(int i = 0; i <= strlen(str); i ++)
	{
		if(str[i] <= 'z' && str[i] >= 'a')
		{
			str[i] -= 'a';
			str[i] += 'A';
		}
	}
}

void help(char * name)
{
	std::cout << name << " will show you a bargraph of recent short-sale volumes of the stock symbol that you entered.\n Use case: "\
		<< name << " AAPL\n";
}


int main(int argc, char ** argv)
{

	std::string symbol = "|";
	if(argc == 2)
	{
		toAllCaps(argv[1]);
		symbol += argv[1];
		symbol += "|";
	}
	else
	{
		help(argv[0]);
		exit(0);
	}


/// LOAD ALL THE FILES!
	time_t start_t = time(0);
	tm * gmt = gmtime(&start_t);
	std::cout <<"(GMT): " << asctime(gmt);
	std::vector <int> counts;

	std::string site = "regsho.finra.org";
	itemInit();
	int x = 1900, y = 1152;
	SDL_Window * win = SDL_CreateWindow("Daily Short Volume", 30, 30, x, y, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
	SDL_Renderer * screen = SDL_CreateRenderer(win, -1, 0);
	webText shake("regsho.finra.org");

	item bkgScreen;
	bkgScreen.setRenderer(screen);
	bkgScreen.setSize(x, y);
	bkgScreen.setPos(1,1);
	bkgScreen.loadImage("pleaswait.png");
	bkgScreen.draw();
	SDL_RenderPresent(screen);

	barGraph bob(screen, 100, 100, 1800, 800);
	SDL_SetRenderDrawBlendMode(screen, SDL_BLENDMODE_BLEND);
	bob.setBarColor(30, 90, 120, 80);
//	bob.setBkgImage("graphBack.png");


	std::vector <std::string> dates;
	std::string today = getDateStr(gmt);
	for(int h = 53; today > "20201012"; h --)
	{

		start_t -= 86400; // seconds in a day...
		gmt = gmtime(&start_t);
		today = getDateStr(gmt);

		// if not a weekend
		//if(asctime(gmt)[0] != 'S' && today != "20201126")
		if(isBusinessDay(gmt))
		{
		
			std::vector <std::string> filePaths;
			filePaths.push_back("/FNQCshvol" + today + ".txt");
			filePaths.push_back("/CNMSshvol" + today + ".txt");
			filePaths.push_back("/FNSQshvol" + today + ".txt");
			filePaths.push_back("/FNYXshvol" + today + ".txt");
		
		
			int count = 0;
			std::vector <std::string> data;
			dates.push_back(today);


			for(int i = 0; i < filePaths.size(); i ++)
			{
				data.push_back("");
				std::ifstream ifile("res/" + filePaths[i].substr(1), std::ios::binary | std::ios::ate);
				bool existsAndFull = false;
				if(ifile.good())
				{
					// file exists, but is it full with data, or an empty header?
					int size = ifile.tellg();
					ifile.seekg(0);
					if(size > 500)
					{
						existsAndFull = true;
						std::stringstream ss;
						ss << ifile.rdbuf();
						data[i] = ss.str();
					}
				
				}
				ifile.close();
				if(!existsAndFull)
				{
					// get the file from online.
					data[i] = shake.get(filePaths[i]);
					std::ofstream of("res/" + filePaths[i].substr(1));
					of << data[i];
					of.close();
					std::cout << "Downloading data for " << getDateStr(gmt) << "\n";
				}
				
				int pos = data[i].find(symbol);
				int pos2 = data[i].find('|', pos + symbol.length());
				if(std::string::npos != pos)
				{
					std::stringstream ss;
					ss << data[i].substr(pos + symbol.length(), pos2 - pos - symbol.length()) << std::endl;
					int temp;
					ss >> temp;
					
					count += temp;
				}
			}
		
			counts.push_back(count);
		}
	}
	for(int i = counts.size() - 1; i >= 0; i --)
	{
		bob.addValue(dates[i], counts[i]);
		std::cout << dates[i] << ": " << counts[i] << std::endl;
	}
	bob.refresh();

	TTF_Font * acme = TTF_OpenFont("Acme/Acme-Regular.ttf", 179);
	label hi(screen, acme, symbol, 30, 30);
	hi.setColor(2, 2, 2, 200);
	hi.buildImage();
//	hi.setSize(400, 400);
	bool run = true;
	SDL_SetRenderDrawColor(screen, 30, 180, 20, 255);
	SDL_RenderClear(screen);
	int fps = 4;
	int desiredDelta = 1000 / fps;
	while(run)
	{
		int startLoop = SDL_GetTicks();
		SDL_Event ev;
		while(SDL_PollEvent(&ev))
		{
			switch(ev.type)
			{
				case SDL_QUIT:
					run = false;
					break;
				case SDL_KEYDOWN:
					switch(ev.key.keysym.sym)
					{
						case SDLK_UP:
							bob.addValue("new", rand() % 50000 - 12000);
							bob.refresh();
							break;
						case SDLK_DOWN:
							break;
						case SDLK_LEFT:
							break;
						case SDLK_RIGHT:
							break;
						case SDLK_w:
							break;
						case SDLK_s:
							break;
						case SDLK_a:
							break;
						case SDLK_d:
							break;
					}

					break;
				case SDL_KEYUP:
					switch(ev.key.keysym.sym)
					{
						case SDLK_UP:
						case SDLK_DOWN:
							break;
						case SDLK_LEFT:
						case SDLK_RIGHT:
							break;
						case SDLK_w:
							run = false;
							break;
						case SDLK_s:
							break;
						case SDLK_a:
						case SDLK_d:
							break;
					}
					break;
				case SDL_WINDOWEVENT:

					if(ev.window.event == SDL_WINDOWEVENT_RESIZED)
					{
						x = ev.window.data1;
						y = ev.window.data2;
					}
					break;
			}
		}
		SDL_RenderFillRect(screen, NULL);

	 	bob.draw();
		hi.draw();

		SDL_RenderPresent(screen);

		int delta = SDL_GetTicks() - startLoop;
		if(delta < desiredDelta)
		{
			SDL_Delay(desiredDelta - delta);
		}
	}
	TTF_CloseFont(acme);

	SDL_DestroyWindow(win);
	itemQuit();

}
