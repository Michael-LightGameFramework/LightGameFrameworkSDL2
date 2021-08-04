#include "item.h"
#include <iostream>
#include <cstring>

#include <fstream>

int fileSize(std::string path)
{
	std::ifstream file(path.c_str());
	file.seekg(file.end);
	int val = file.tellg();
	file.close();
	return val;
}

void itemInit()
{
	SDL_Init(SDL_INIT_EVERYTHING);
	IMG_Init(IMG_INIT_PNG || IMG_INIT_JPG);
	TTF_Init();
	SDLNet_Init();
}

void itemQuit()
{
	SDL_Quit();
	IMG_Quit();
	TTF_Quit();
	SDLNet_Quit();
}



// Classes

item::item()
{
	pos.x = 30;
	pos.y = 60;
	image = NULL;
	pos.w = 100;
	pos.h = 100;
	ID ++;
}

item::item(SDL_Renderer * renderer, std::string loadImg)
{
	this->ren = renderer;
	this->loadImage(loadImg);
	pos.x = 0;
	pos.y = 0;
	SDL_QueryTexture(image, NULL, NULL, &(pos.w), &(pos.h));
	ID ++;
//	pos.w = 100;
//	pos.h = 100;
}

item::~item()
{
	if(image != NULL)
	{
		SDL_DestroyTexture(image);
		image = NULL;
	}
}

item * item::getSelf()
{
	return this;
}


bool item::loadImage(std::string filename)
{
	SDL_Surface * temp = IMG_Load(filename.c_str());
	if(temp != NULL)
	{
		image = SDL_CreateTextureFromSurface(ren, temp);
		SDL_FreeSurface(temp);
		if(image != NULL)
		{
			return true;
		}
	}
	return false;
}
void item::freeImage()
{
	if(image != NULL)
	{
		SDL_DestroyTexture(image);
		image = NULL;
	}
}

void item::setRenderer(SDL_Renderer * dest)
{
	ren = dest;
}

void item::setSize(int w, int h)
{
	pos.w = w;
	pos.h = h;
}

void item::setPos(int x, int y)
{
	pos.x = x;
	pos.y = y;
}

SDL_Rect * item::getPos()
{
	return &pos;
}

void item::move(int x, int y)
{
	pos.x += x;
	pos.y += y;
}

bool item::getCollision(item * other)
{
	int dx, dy, rs;
	dx = pos.x + center.x - (other->getPos()->x + other->getCenter().x);
	dy = pos.y + center.y - (other->getPos()->y + other->getCenter().y);
	rs = center.r + other->getCenter().r;
	dx *= dx;
	dy *= dy;
	rs *= rs;
	if(dx + dy < rs)
	{
		return true;
	}
	return false;

}

bool item::isClicked(int x, int y)
{
	int dx, dy, rs;
	dx = pos.x + center.x - x;
	dy = pos.y + center.y - y;
	rs = center.r;
	dx *= dx;
	dy *= dy;
	rs *= rs;
	return (dx + dy < rs);
}

circle item::getCenter()
{
	return center;
}

void item::setCenter(int x, int y, int r)
{
	center.x = x;
	center.y = y;
	center.r = r;
}

void item::draw(double angle)
{

	if(image != NULL)
	{
		SDL_RenderCopyEx(ren, image, NULL, &pos, angle, NULL, SDL_FLIP_NONE);
	}
	else 
	{
		std::cout << "Help, image is NULL at draw()\n";
	}

}

void item::draw()
{
	if(image != NULL)
	{
		SDL_RenderCopy(ren, image, NULL, &pos);
	}
	else 
	{
		std::cout << "Help, image is NULL at draw()\n";
	}
}

void item::stamp(item * paper, int x, int y)
{
	//	SDL_Texture dest = SDL_CreateTexture(ren, 
}

void item::update(int tick)
{
	oldTick = tick;
}

SDL_Texture * item::getImage()
{
	return image;
}


void item::preloop()
{

}

bool item::ready()
{
	return true;
}

void item::handleEvent(SDL_Event * ev)
{

}

void item::cleanup()
{

}

////////////// CLASS ANIMATION

animation::animation()
{

}


animation::~animation()
{
	this->freeImages();
}


bool animation::addImage(std::string filePath)
{
	if(loadImage(filePath))
	{
		images.push_back(image);
		return true;
	}
	return false;
}


bool animation::loadAnimation(std::string p, std::string zb, std::string ext)
{
	int leng = zb.length() - 1;
	while(addImage(p + zb + ext) && zb[0] < '9')
	{
		zb[leng] ++;
		for(int i = leng; i >= 0; i --)
		{
			if(zb[i] > '9')
			{
				zb[i-1] ++;
				zb[i] = '0';
			}
		}
	}
	if(images[0] != NULL)
	{
		return true;
	}
	return false;
}

void animation::next()
{
	frameCount ++;
	image = images[frameCount % (images.size() -1)];
}

void animation::freeImages()
{
	for(int i = 0; i < images.size(); i ++)
	{
		if(images[i] != NULL)
		{
			SDL_DestroyTexture(images[i]);
			images[i] = NULL;
		}
	}
}


void animation::setFPS(int FPS)
{
	desiredDelta = 1000 / FPS;
}

void animation::update(int tick)
{
	if(tick - oldTick > desiredDelta)
	{
		next();
		oldTick = tick;
	}
}




///////////////// Label Text CLASS ///////////////////////


label::label(SDL_Renderer * rend, TTF_Font * fnt, std::string text, int x, int y)
{
	textColor.r = 255;
	textColor.g = 255;
	textColor.b = 255;
	textColor.a = 200;
	ren = rend;
	font = fnt;
	words = text;
	pos.x = x;
	pos.y = y;

	buildImage();
}

TTF_Font * label::getFont()
{
	return this->font;
}

bool label::buildImage()
{
	if(font != NULL)
	{
		SDL_Surface * textSrc = TTF_RenderText_Solid(font, words.c_str(), textColor);
		image = SDL_CreateTextureFromSurface(ren, textSrc);
		pos.w = textSrc->w;
		pos.h = textSrc->h;
		SDL_FreeSurface(textSrc);
	}
	return image != NULL;
}

void label::setColor(Uint8 r, Uint8 b, Uint8 g, Uint8 a)
{
	textColor.r = r;
	textColor.g = g;
	textColor.b = b;
	textColor.a = a;
}

void label::setText(std::string txt)
{
	words = txt;
}





////////  VBAR CLASS /////////////////////////////

box::box(SDL_Renderer *rend, int x, int y, int w, int h)
{
	pos.x = x;
	pos.y = y;
	pos.w = w;
	pos.h = -h;
	ren = rend;
	rbox = 20;
	gbox = 30;
	bbox = 200;
	abox = 255;
}

void box::draw()
{
	Uint8 r, g, b, a;
	int getter = SDL_GetRenderDrawColor(ren, &r, &g, &b, &a);
	SDL_SetRenderDrawColor(ren, rbox, gbox, bbox, abox);
	SDL_RenderFillRect(ren, &pos);
	if(!getter)
	{
		SDL_SetRenderDrawColor(ren, r, g, b, a);
	}
}

void box::setColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
	rbox = r;
	gbox = g;
	bbox = b;
	abox = a;
}



///////////////////  barGraph ////////////////////
barGraph::barGraph(SDL_Renderer *rend, int x, int y, int w, int h)
{
	ren = rend;
	image = SDL_CreateTexture(ren, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET,  w, h);
	pos.x = x;
	pos.y = y;
	pos.w = w;
	pos.h = h;
	rbar = 20;
	gbar = 30;
	bbar = 200;
	abar = 255;
	Uint8 r, g, b, a;
//	SDL_GetRenderDrawColor(ren, &r, &g, &b, &a);
	SDL_SetRenderTarget(ren, image);
	SDL_SetRenderDrawColor(ren, 130, 180, 255, 255);
	SDL_RenderClear(ren);
	SDL_RenderFillRect(ren, NULL);
	SDL_SetRenderTarget(ren, NULL);
//	SDL_SetRenderDrawColor(ren, r, g, b, a);
	maxVal = 2;
	minVal = 0;
	sampleCount = 0;
}

void barGraph::refresh()
{
	Uint8 r, g, b, a;
	SDL_GetRenderDrawColor(ren, &r, &g, &b, &a);
	SDL_SetRenderTarget(ren, image);
	SDL_SetRenderDrawColor(ren, 130, 180, 255, 255);
	SDL_RenderClear(ren);
	if(bkg.getImage() != NULL)
	{
		bkg.draw();
	}
	int width = pos.w/value.size();
	int zeroY = maxVal/(maxVal - minVal) * pos.h;
	if(minVal == maxVal)
	{
		maxVal += 1;
	}
	for(int i = 0; i < value.size(); i ++)
	{
		//box sq(ren, i * width, pos.h, width - 1, value[i]/(maxVal - minVal)*(pos.h + 5));
		box sq(ren, i * width, zeroY, width - 1, value[i]/(maxVal - minVal)*(pos.h + 5));
		sq.setColor(rbar, gbar, bbar, abar);
		sq.draw();
	}
	SDL_SetRenderTarget(ren, NULL);
	SDL_SetRenderDrawColor(ren, r, g, b, a);
}


void barGraph::addValue(std::string aname, double aval)
{
	if(aval > maxVal)
	{
		maxVal = aval;
	}
	if(aval < minVal)
	{
		minVal = aval;
	}
	name.push_back(aname);
	value.push_back(aval);
	sampleCount ++;

}

void barGraph::setBarColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
	rbar = r;
	gbar = g;
	bbar = b;
	abar = a;
}

void barGraph::setBkgImage(std::string ipath)
{
	bkg.setRenderer(ren);
	bkg.loadImage(ipath);
	bkg.setSize(pos.w, pos.h);
	bkg.setPos(0,0);
	
}
std::string barGraph::getLabel(int index)
{
	return name[index];
}

/*
box * barGraph::getBox(int x, int y)
{
	std::cout << "getBox not imp";
	return NULL
}
*/

//////////////////////////// webText Class //////////////////////////////

webText::webText(std::string siteLink)
{
	link = siteLink;
	char ln[1024];
	strcpy(ln, siteLink.c_str());
	connected = true;
	if(SDLNet_ResolveHost(&ip, ln, 80) < 0)
	{
		std::cout << "Failed to net_resolveHost() : " << SDLNet_GetError() << "\n";
		connected = false;
	}
}

std::string webText::get(std::string linkPath)
{
	std::string full;
	char cmd[1024];

	std::strcpy(cmd, "GET ");
	std::strcat(cmd, linkPath.c_str());
	std::strcat(cmd, " HTTP/1.1\r\nHost: ");
	std::strcat(cmd, link.c_str());
	std::strcat(cmd, "\r\n\r\n");

	if(connected)
	{
		client = SDLNet_TCP_Open(&ip);
		SDLNet_SocketSet sSet;
		sSet = SDLNet_AllocSocketSet(3);
		SDLNet_TCP_AddSocket(sSet, client);
		SDLNet_TCP_Send(client, cmd, strlen(cmd));
		char text[1024];
		char oldText[1024];
		int count = 1;
		while(count > 0)
		{
			count = 0;
			// recv causes blocking if timeout not closed
			if(SDLNet_CheckSockets(sSet, 5000) > 0)
			{
				count = SDLNet_TCP_Recv(client, text, 1023);
				if(count <= 0)
				{
					std::cout << "Possible error detected...\n";
					break;
				}
				else full += text;
			}

		}
		SDLNet_TCP_Close(client);
	}
	else
	{
		return "Could not connect.\n";
	}
	return full;
}

webText::~webText()
{

}







audioPlayer::audioPlayer()
{
	SDL_AudioSpec pref;
	SDL_memset(&pref, 0, sizeof(pref));
	SDL_memset(&specs, 0, sizeof(specs));
	pref.freq = 16000;
	pref.format = AUDIO_U8;
	pref.channels = 1;
	pref.samples = 1024;
	pref.callback = NULL;
	dev = SDL_OpenAudioDevice(NULL, 0, &pref, &specs, 0);
	if(dev < 1)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "AudioPlayer", "Could not open an audio playback device, please check that speakers are available to the system", NULL); 
	}
}

SDL_AudioSpec audioPlayer::getSpec()
{
	return this->specs;
}

void audioPlayer::setSpec(SDL_AudioSpec newSpec)
{
	SDL_PauseAudioDevice(dev, 1);
	SDL_CloseAudioDevice(dev);
	dev = SDL_OpenAudioDevice(NULL, SDL_FALSE, &newSpec, &specs, 0);
}

void audioPlayer::loadWav(std::string path)
{
	SDL_AudioSpec newspec;
	SDL_memset(&newspec, 0, sizeof(newspec));
	Uint32 length = fileSize(path);
	data.reserve(length + 1);
	Uint8 * buffer;
	SDL_LoadWAV(path.c_str(), &specs,  &buffer, &length);
	dev = SDL_OpenAudioDevice(NULL, SDL_FALSE, &newspec, &specs, 0);
	for(int i = 0; i < length; i ++)
	{
		data.push_back(buffer[i]);
	}
	SDL_QueueAudio(dev, &data[0], data.size());
	SDL_FreeWAV(buffer);
	std::cout << "Freq: " << specs.freq << " Formt: " << specs.format << " chans: " << specs.channels << " samples: " << specs.samples << std::endl;

}

void audioPlayer::loadRaw(std::string path)
{
	int length = fileSize(path);
	data.reserve(length + 1);
	std::ifstream ifile(path.c_str(), std::ios::binary);
	ifile.read((char*) (&data[0]), length);
	ifile.close();
	SDL_QueueAudio(dev, &data[0], length);

}

void audioPlayer::loadRaw(std::vector <Uint8> data)
{
	this->data = data;
	SDL_ClearQueuedAudio(dev);
	SDL_QueueAudio(dev, &data[0], data.size());
}

void audioPlayer::play()
{
	SDL_PauseAudioDevice(dev, 0);
}

void audioPlayer::pause()
{
	SDL_PauseAudioDevice(dev, 1);

}

void audioPlayer::stop()
{
	SDL_PauseAudioDevice(dev, 1);
	SDL_ClearQueuedAudio(dev);
	SDL_QueueAudio(dev, &data[0], data.size());
}

void audioPlayer::clear()
{
	pause();
	data.clear();
	SDL_ClearQueuedAudio(dev);
}






recorder::recorder()
{
	player = new audioPlayer;
	SDL_AudioSpec pref;
	SDL_memset(&pref, 0, sizeof(pref));
	SDL_memset(&specs, 0, sizeof(specs));
	pref.freq = 16000;
	pref.format = AUDIO_U8;
	pref.channels = 1;
	pref.samples = 1024;
	pref.callback = NULL;

	dev = SDL_OpenAudioDevice(NULL, 1, &pref, &specs, 0);
	if(dev < 1)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Recorder", "Please enable your microphone for recording", NULL); 
	}
}

recorder::~recorder()
{
	delete player;
}

SDL_AudioSpec recorder::getSpec()
{
	return specs;
}

void recorder::setSpec(SDL_AudioSpec newSpec)
{
	SDL_CloseAudioDevice(dev);
	player->setSpec(newSpec);
	SDL_AudioSpec tempSpec = player->getSpec();
	dev = SDL_OpenAudioDevice(NULL, SDL_FALSE, &newSpec, &specs, 0);
}

audioPlayer * recorder::getPlayer()
{
	return player;
}

void recorder::record()
{
	SDL_PauseAudioDevice(dev, 0);
}

void recorder::pause()
{
	player->pause();
	SDL_PauseAudioDevice(dev, 1);
}

void recorder::resume()
{
	SDL_PauseAudioDevice(dev, 0);
}

void recorder::play()
{
	int samples = specs.samples;
	Uint8 * buffer = new Uint8[samples];
	int ret = samples;
	while(ret == samples)
	{
		ret = SDL_DequeueAudio(dev, buffer, samples);
		for(int i = 0; i < ret; i ++)
		{
			data.push_back(buffer[i]);		
		}
	}
	delete [] buffer;
	player->loadRaw(this->data);
	player->play();
}

void recorder::clear()
{
	player->clear();
	data.clear();
	SDL_ClearQueuedAudio(dev);
}


std::vector <Uint8> recorder::getData()
{
	return data;
}

void recorder::save(std::string filePath)
{
	std::ofstream ofile(filePath.c_str(), std::ios::binary);
	ofile.write((char*)(&data[0]), data.size());
	ofile.close();
}







