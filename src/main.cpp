#include "Window.h"
#include "ConfigReader.h"

int main()
{
    setlocale(LC_ALL, "");

    ConfigReader config;
    // Вам нужно будет считать данные значения из конфига
	const int wWidth = config.getWindowWidth();
    const int wHeight = config.getWindowHeight();

    Window window(wWidth, wHeight);
    window.Run();

    return 0;
}