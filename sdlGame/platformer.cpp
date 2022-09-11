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
	if(moving)
	{
		if(offsetX <= destX)
		{
			offsetX += speedX;
			if(offsetX > destX)
			{
				float hold = offsetX;
				offsetX = startX;
				startX = hold;
			}
		}
		else if(offsetX > destX)
		{
			offsetX -= speedX;
			if(offsetX < destX)
			{
				float hold = offsetX;
				offsetX = startX;
				startX = hold;
			}
		}	
		if(offsetY <= destY)
		{
			offsetY += speedY;
			if(offsetY > destY)
			{
				float hold = offsetY;
				offsetY = startY;
				startY = hold;
			}
		}
		else if(offsetY >= destY)
		{
			offsetY -= speedY;
			if(offsetY < destY)
			{
				float hold = offsetY;
				offsetY = startY;
				startY = hold;
			}
		}
	}
	if(busting && busted)
	{
		busted->update(tick);
	}
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


