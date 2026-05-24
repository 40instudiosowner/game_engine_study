#include "Window.h"

#include "ConfigReader.h"

#include <iostream>

int main()
{
	setlocale(LC_ALL, "");

#ifdef DEBUG
	sf::err().rdbuf(std::cout.rdbuf());

	std::cout
		<< "Debug mode enabled\n";
#endif

	ConfigReader config;

	Window window(
		config.getWindowWidth(),
		config.getWindowHeight(),
		config);

	window.Run();

	return 0;
}