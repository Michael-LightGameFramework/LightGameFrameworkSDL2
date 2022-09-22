#include <SDL2/SDL.h>
#include <iostream>

int main()
{
	SDL_Init(SDL_INIT_EVERYTHING);
	
	std::cout << (SDL_GetPrefPath("LightGameFramework", "NoTitle"));
	SDL_Quit();
	
}
