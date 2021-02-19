#include <iostream>
#include <time.h>
#include <string>
#include <fstream>

#include "../item.h"
#include "stonks.h"

using namespace std;

int main()
{

	getShortFiles();
	time_t start_t = time(NULL);
	tm * gmt = gmtime(&start_t);

	for(int i = 0; i < 5; i ++)
	{
		start_t -= 86400;
		gmt = gmtime(start_t);
		searchTopTen(gmt);
	}


}
