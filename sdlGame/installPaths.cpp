#include <SDL2/SDL.h>

int main()
{
	SDL_Init(SDL_INIT_EVERYTHING);
	
	SDL_Log(SDL_GetPrefPath("LightGameFramework", "NoTitle"));
	SDL_Quit();
	
}
