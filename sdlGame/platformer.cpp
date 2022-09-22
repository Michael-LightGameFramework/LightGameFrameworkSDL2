#include <platformer.h>

////////////////////////////////////////
// PLATFORM
////////////////////////////////////////


platform::platform(SDL_Renderer * screen)
{
	ren = screen;
	speedX = 2.0f;
	speedY = 2.0f;
	crushable = false;
	tide = false;
	moving = false;
	busting = false;
	busted = NULL;
	loadImage(absPath() + "res/images/platformWood.png");
}

void platform::loadBustAnimation(std::string pre, std::string zb, std::string ext)
{
	if(busted)
	{
		delete busted;
	}
	busted = new animation(ren);
	busted->loadAnimation(pre, zb, ext);
	busted->loopCount = 1;

}

void platform::bust()
{
	busting = true;
}

void platform::moveBetween(int x, int y)
{
	startX = pos.x;
	startY = pos.y;
	destX = x;
	destY = y;
	moving = true;
}

void platform::update(int tick)
{
	offsetX = pos.x;
	offsetY = pos.y;
	if(moving)
	{
		if(offsetX <= destX)
		{
			offsetX += speedX;
			if(offsetX > destX)
			{
				float hold = destX;
				destX = startX;
				startX = hold;
			}
		}
		else if(offsetX > destX)
		{
			offsetX -= speedX;
			if(offsetX < destX)
			{
				float hold = destX;
				destX = startX;
				startX = hold;
			}
		}	
		if(offsetY <= destY)
		{
			offsetY += speedY;
			if(offsetY > destY)
			{
				float hold = destY;
				destY = startY;
				startY = hold;
			}
		}
		else if(offsetY >= destY)
		{
			offsetY -= speedY;
			if(offsetY < destY)
			{
				float hold = destY;
				destY = startY;
				startY = hold;
			}
		}
	}
	if(busting && busted)
	{
		busted->update(tick);
	}
	pos.x = offsetX;
	pos.y = offsetY;
}

////////////////////////////////////////
// SPAWNER
////////////////////////////////////////



////////////////////////////////////////
// SHOOTER
///////////////////////////////////////


////////////////////////////////////////
// ENEMY
//////////////////////////////////////

////////////////////////////////////////
// PLAYER
///////////////////////////////////////





//////////////////////////////////////
// PLATFORMLEVEL
////////////////////////////////////


