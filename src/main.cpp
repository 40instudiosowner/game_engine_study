#include "Window.h"

int main()
{
    setlocale(LC_ALL, "");

    // Вам нужно будет считать данные значения из конфига
    const int wWidth = 1280;
    const int wHeight = 720;

    Window window(wWidth, wHeight);
    window.Run();

    return 0;
}