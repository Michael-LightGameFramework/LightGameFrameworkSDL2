#include "stonks.h"
#include "item.h"
#include <iostream>
#include <sstream>

bool isBusinessDay(tm * gmt)
{
	std::string asciiStr = asctime(gmt);

	if(asciiStr[0] == 'S')
	{
		// Saturday or sunday. Not a stock day.
		return false;
	}
	if(gmt->tm_mon == 10) // Nov
	{
		if(gmt->tm_mday > 21)
		{
			if(asciiStr[1] == 'h')
			{
				return false; // Happy Thanksgiving.
			}
		}
	}
	else if(gmt->tm_mon == 11) // Dec
	{
		// actual day that stocks close is a bit random. Easter is worse.(Phase of the moon).
		int year = gmt->tm_year + 1900;
		if(year == 2020 && gmt->tm_mday == 25)
		{
			return false;
		}
		else if(year == 2021 && gmt->tm_mday == 24)
		{
			return false;
		}
	}
	else if(gmt->tm_mon == 0) // Jan 
	{
		if(gmt->tm_mday == 1)
		{
				return false; // Happy New Year
		}
		else if(gmt->tm_mday == 2 && asciiStr[0] == 'M')
		{
			return false; // new years was on a Sunday, so they gave us the next day off.
		}
		else if(gmt->tm_mday >= 15 && gmt->tm_mday <= 21 && asciiStr[0] == 'M')
		{
			return false; // happy Martin L. King Day
		}
	}
	else if(gmt->tm_mon == 1) // Feb
	{
		if(gmt->tm_mday >= 15 && gmt->tm_mday <= 21 && asciiStr[0] == 'M')
		{
			return false; // happy Presidents' Day
		}
	}
	else if(gmt->tm_mon == 8) // Sept
	{
		if(asciiStr[0] == 'M' && gmt->tm_mday <= 7)
		{
			return false; // labor day.
		}
	}
//	std::cout << asciiStr << std::endl;

	return true;
}

bool isBusinessDay()
{
	time_t t = time(0);
	tm * gmt = gmtime(&t);
	return isBusinessDay(gmt);
	
}

std::string getDateStr(tm * gmt)
{
    // year month day as per file format of regsho

	std::stringstream ss;
    ss << gmt->tm_year + 1900;
	if(gmt->tm_mon < 9)
	{
		ss << 0;
	}
    ss << gmt->tm_mon + 1;
    if (gmt->tm_mday < 10)
    {
        ss << "0";
    }
    ss << gmt->tm_mday;
    return ss.str();
}

std::string getDateStr()
{
	time_t t = time(0);
	tm * gmt = gmtime(&t);
	return getDateStr(gmt);
}

// grab the files from the internet if required.
void getShortFiles()
{
	time_t start_t = time(NULL);
	tm * gmt = gmtime(&start_t);
	std::string asciiTime = asctime(gmt);
	for(int i = 0; i < 5; i ++)
	{
		start_t -= 86400; // reduce by one day
		gmt = gmtime(&start_t);
		if(isBusinessDay(gmt))
		{
			std::cout << getDateStr(gmt) << " ";
		}
	}
}
