#include "GameEngine.h"
#include "Scenes/MenuScene.h"
#include "Scenes/GameScene.h"

#include <iostream>

int main()
{
	setlocale(LC_ALL, "");

#ifdef DEBUG
	sf::err().rdbuf(std::cout.rdbuf());
	std::cout << "Debug mode enabled\n";
#endif

	GameEngine engine(1280, 768);

	// Register scenes
	engine.RegisterScene("menu", std::make_unique<MenuScene>());
	engine.RegisterScene("game", std::make_unique<GameScene>());

	// Start with menu
	engine.ChangeScene("menu");

	engine.Run();

	return 0;
}