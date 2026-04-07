#include "Window.h"
#include "ConfigReader.h"

int main()
{
    setlocale(LC_ALL, "");

#ifdef DEBUG
    sf::err().rdbuf(std::cout.rdbuf());
    std::cout<< "Debug mode enabled\n";
#endif

    ConfigReader config;
    // Вам нужно будет считать данные значения из конфига
	const int wWidth = config.getWindowWidth();
    const int wHeight = config.getWindowHeight();

    Window window(wWidth, wHeight, config);
    window.Run();

    return 0;
}