#include <item.h>
#include <string>
#include <iostream>

int main()
{
	itemInit();
	/**/
	webText cppFetch("cplusplus.com");
	std::string content = cppFetch.get("/reference/string/string/");
	/**/
	/** /
	webText fetch("www.google.com");
	std::string content = fetch.get("/");
	/** /
	 // this defaults to an https page, which seems to be a problem for SDL2_net
	 // maybe it's my configuration?
	webText fetch("gutenberg.org");
	std::string content = fetch.get("/");
	/** /

	/** /
	webText ducky("duckduckgo.com");
	std::string content = ducky.get("/");
	/**/
	std::cout << content;
	std::cout << std::endl;
	itemQuit();
}
