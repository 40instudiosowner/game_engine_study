#include "GameEngine.h"
#include "Scenes/MenuScene.h"
#include "Scenes/GameScene.h"
#ifdef EDITOR_ENABLED
#include "Editor/EditorScene.h"
#endif

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
#ifdef EDITOR_ENABLED
	engine.RegisterScene("editor", std::make_unique<Editor::EditorScene>());
#endif

	// Start with menu
	engine.ChangeScene("menu");

	engine.Run();

	return 0;
}