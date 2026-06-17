#include "GameScene.h"
#include "../GameEngine.h"
#include "../AssetManager.h"

#include "../Components/TransformComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/MovementComponent.h"
#include "../Components/BoxColliderComponent.h"
#include "../Components/CollisionComponent.h"
#include "../Components/PlayerComponent.h"
#include "../Components/BulletComponent.h"
#include "../Components/VelocityComponent.h"
#include "../Components/GravityComponent.h"
#include "../Components/TileComponent.h"
#include "../Components/DecorComponent.h"
#include "../Components/FinishComponent.h"
#include "../Components/DefaultCameraComponent.h"
#include "../Components/FollowXCameraComponent.h"
#include "../Components/AnimationStateComponent.h"
#include "../Components/AnimatorComponent.h"

#include "../Systems/RenderSystem.h"
#include "../Systems/AnimationSystem.h"
#include "../Systems/CameraSystem.h"
#include "../Systems/GravitySystem.h"
#include "../Systems/MovementSystem.h"
#include "../Systems/CollisionSystem.h"
#include "../Systems/PlayerControlSystem.h"
#include "../Systems/PlayerAnimationSystem.h"
#include "../Systems/BrickSystem.h"
#include "../Systems/TileCollisionSystem.h"
#include "../Systems/ShooterSystem.h"

#include <fstream>
#include <iostream>
#include <imgui.h>

static constexpr int TILE_SIZE = 64;
static constexpr int WINDOW_HEIGHT = 768;
static constexpr int TOTAL_ROWS = WINDOW_HEIGHT / TILE_SIZE; // 12

static float GridYToPixel(int gridY)
{
	return static_cast<float>((TOTAL_ROWS - 1 - gridY) * TILE_SIZE);
}

void GameScene::Init(GameEngine* engine)
{
	_engine = engine;
	_levelTime = 0.f;
	_renderMode = 0;
	_isPaused = false;

	LoadGameConfig("assets/configs/game.json");
	LoadLevelConfig("assets/configs/level.json");
	SetupInputActions();

	// Camera entity
	auto camera = _world.CreateEntity();
	_world.AddComponent(camera, DefaultCameraComponent{});

	auto followCam = _world.CreateEntity();
	_world.AddComponent(followCam, FollowXCameraComponent{});

	// Systems
	_systems.AddSystem<PlayerControlSystem>(_engine->GetInputManager());
	_systems.AddSystem<ShooterSystem>(_gameState, _engine->GetAssets());
	_systems.AddSystem<GravitySystem>();
	_systems.AddSystem<MovementSystem>();
	_systems.AddSystem<CollisionSystem>();
	_systems.AddSystem<TileCollisionSystem>();
	_systems.AddSystem<BrickSystem>(_engine->GetAssets(), _engine->GetWindow(),
		_gameState, [this](const std::string& scene) { _engine->ChangeScene(scene); });
	_systems.AddSystem<PlayerAnimationSystem>();
	_systems.AddSystem<AnimationSystem>();
	_systems.AddSystem<CameraSystem>(_engine->GetWindow(), *_engine);
	_systems.AddSystem<RenderSystem>(_engine->GetWindow(), _gameState, _engine->GetAssets(), _renderMode);

	std::cout << "GameScene initialized\n";
}

void GameScene::SetupInputActions()
{
	auto& input = _engine->GetInputManager();
	input.Clear();
	input.RegisterAction("MoveLeft", sf::Keyboard::Key::A, ActionType::Hold, []{});
	input.RegisterAction("MoveRight", sf::Keyboard::Key::D, ActionType::Hold, []{});
	input.RegisterAction("Jump", sf::Keyboard::Key::W, ActionType::Press, []{});
	input.RegisterAction("Shoot", sf::Keyboard::Key::Space, ActionType::Press, []{});
}

void GameScene::LoadGameConfig(const std::string& path)
{
	std::ifstream file(path);
	if (!file.is_open())
	{
		std::cerr << "Could not open game config: " << path << "\n";
		return;
	}

	json config;
	file >> config;

	const auto& player = config["Player"];
	_playerBBox = sf::Vector2f{ player["Bbox"][0], player["Bbox"][1] };
	_playerSpeedX = player["SX"];
	_playerJumpY = player["JY"];
	_playerMaxSpeed = player["SM"];
	_playerGravity = player["GY"];
	_playerBaseAnim = player["BasePose"];
	for (const auto& anim : player["Animations"])
		_playerAnimations.push_back(anim);

	const auto& bullet = config["Bullet"];
	_bulletSpeedX = bullet["SX"];
	_bulletRadius = bullet["Radius"];

	std::cout << "Loaded game config\n";
}

void GameScene::LoadLevelConfig(const std::string& path)
{
	std::ifstream file(path);
	if (!file.is_open())
	{
		std::cerr << "Could not open level config: " << path << "\n";
		return;
	}

	json level;
	file >> level;

	for (const auto& obj : level)
	{
		std::string name = obj["name"];
		int gx = obj["x"];
		int gy = obj["y"];

		if (name == "Player")
			CreatePlayer(json::object(), gx, gy);
		else if (name == "Tile")
			CreateTile(gx, gy, "Tile");
		else if (name == "Brick")
			CreateTile(gx, gy, "Brick");
		else if (name == "Question")
			CreateTile(gx, gy, "Question");
		else if (name == "BigHill" || name == "SmallHill" || name == "Cloud")
			CreateDecor(gx, gy, name);
		else if (name == "PipeL" || name == "PipeR" || name == "PipeUpL" || name == "PipeUpR")
			CreateTile(gx, gy, name);
		else if (name == "Finish")
			CreateFinish(gx, gy);
	}

	std::cout << "Loaded level config\n";
}

void GameScene::CreatePlayer(const json&, int gridX, int gridY)
{
	auto& assets = _engine->GetAssets();
	auto entity = _world.CreateEntity();

	float px = static_cast<float>(gridX) * TILE_SIZE + TILE_SIZE / 2.f;
	float py = GridYToPixel(gridY) + TILE_SIZE / 2.f;

	TransformComponent transform;
	transform.position = { px, py };
	_world.AddComponent(entity, transform);

	SpriteComponent sprite;
	const auto* baseAnim = assets.GetAnimation(_playerBaseAnim);
	if (baseAnim && baseAnim->texture)
	{
		sprite.texture = baseAnim->texture;
		sprite.textureRect = sf::IntRect({{0, 0}, {baseAnim->frameWidth, baseAnim->frameHeight}});
		sprite.origin = { _playerBBox.x / 2.f, _playerBBox.y / 2.f };
		sprite.scale = { 3.f, 3.f };
	}
	_world.AddComponent(entity, sprite);

	MovementComponent movement;
	movement.speed = _playerSpeedX;
	_world.AddComponent(entity, movement);

	// Velocity component for gravity
	VelocityComponent velocity;
	velocity.velocity = { 0.f, 0.f };
	_world.AddComponent(entity, velocity);

	BoxColliderComponent collider;
	collider.size = _playerBBox;
	_world.AddComponent(entity, collider);

	PlayerComponent player;
	player.bulletSpeed = _bulletSpeedX;
	player.maxSpeed = _playerMaxSpeed;
	player.jumpVelocity = _playerJumpY;
	_world.AddComponent(entity, player);

	GravityComponent gravity;
	gravity.gravity = _playerGravity;
	_world.AddComponent(entity, gravity);

	_world.AddComponent(entity, CollisionComponent{});

	AnimationStateComponent animState;
	animState.currentAnimation = _playerBaseAnim;
	animState.nextAnimation = _playerBaseAnim;
	animState.isLooping = true;
	_world.AddComponent(entity, animState);

	AnimatorComponent animator;
	for (const auto& animName : _playerAnimations)
	{
		const auto* animPtr = assets.GetAnimation(animName);
		if (animPtr)
			animator.animations[animName] = *animPtr;
	}
	_world.AddComponent(entity, animator);

	// Set follow camera target to player
	auto* followPool = _world.GetPool<FollowXCameraComponent>();
	if (followPool && followPool->Size() > 0)
	{
		for (auto cam : followPool->GetDenseEntities())
		{
			followPool->Get(cam).targetEntityId = entity.id;
		}
	}

	std::cout << "Created player at grid (" << gridX << "," << gridY << ")\n";
}

void GameScene::CreateTile(int gridX, int gridY, const std::string& type)
{
	auto& assets = _engine->GetAssets();
	auto entity = _world.CreateEntity();

	float px = static_cast<float>(gridX) * TILE_SIZE;
	float py = GridYToPixel(gridY);
	if (type == "Player" || type == "Finish")
		py += TILE_SIZE / 2.f;

	TransformComponent transform;
	transform.position = { px, py };
	_world.AddComponent(entity, transform);

	SpriteComponent sprite;

	if (type == "Brick")
	{
		const auto* tex = assets.GetTexture(AssetManager::TEX_BRICK_TILE);
		if (tex)
		{
			sprite.texture = tex;
			sprite.textureRect = sf::IntRect({{0, 0}, sf::Vector2i(tex->getSize())});
		}
		_world.AddComponent(entity, TileComponent{ TileType::Brick });

		AnimatorComponent animator;
		const auto* explosionAnim = assets.GetAnimation(AssetManager::ANIM_EXPLOSION);
		if (explosionAnim)
			animator.animations[AssetManager::ANIM_EXPLOSION] = *explosionAnim;
		_world.AddComponent(entity, animator);
	}
	else if (type == "Question")
	{
		const auto* tex = assets.GetTexture(AssetManager::TEX_QUESTION_TILE);
		if (tex)
		{
			sprite.texture = tex;
			sprite.textureRect = sf::IntRect({{0, 0}, sf::Vector2i(tex->getSize())});
		}
		_world.AddComponent(entity, TileComponent{ TileType::Question });

		AnimatorComponent animator;
		const auto* coinAnim = assets.GetAnimation(AssetManager::ANIM_COIN);
		if (coinAnim)
			animator.animations[AssetManager::ANIM_COIN] = *coinAnim;
		_world.AddComponent(entity, animator);
	}
	else if (type == "PipeL" || type == "PipeR" || type == "PipeUpL" || type == "PipeUpR")
	{
		std::string texName;
		if (type == "PipeL") texName = AssetManager::TEX_PIPE_L;
		else if (type == "PipeR") texName = AssetManager::TEX_PIPE_R;
		else if (type == "PipeUpL") texName = AssetManager::TEX_PIPE_UP_L;
		else if (type == "PipeUpR") texName = AssetManager::TEX_PIPE_UP_R;

		const auto* tex = assets.GetTexture(texName);
		if (tex)
		{
			sprite.texture = tex;
			sprite.textureRect = sf::IntRect({{0, 0}, sf::Vector2i(tex->getSize())});
		}
		_world.AddComponent(entity, TileComponent{ TileType::Normal });
	}
	else
	{
		const auto* tex = assets.GetTexture(AssetManager::TEX_TILE);
		if (tex)
		{
			sprite.texture = tex;
			sprite.textureRect = sf::IntRect({{0, 0}, sf::Vector2i(tex->getSize())});
		}
		_world.AddComponent(entity, TileComponent{ TileType::Normal });
	}

	_world.AddComponent(entity, sprite);

	BoxColliderComponent collider;
	collider.size = { TILE_SIZE, TILE_SIZE };
	_world.AddComponent(entity, collider);

	_world.AddComponent(entity, CollisionComponent{});
}

void GameScene::CreateDecor(int gridX, int gridY, const std::string& name)
{
	auto& assets = _engine->GetAssets();
	auto entity = _world.CreateEntity();

	float px = static_cast<float>(gridX) * TILE_SIZE;
	float py = GridYToPixel(gridY);

	TransformComponent transform;
	transform.position = { px, py };
	_world.AddComponent(entity, transform);

	SpriteComponent sprite;
	std::string texName;
	if (name == "BigHill") texName = AssetManager::TEX_BIG_HILL;
	else if (name == "SmallHill") texName = AssetManager::TEX_SMALL_HILL;
	else if (name == "Cloud") texName = AssetManager::TEX_CLOUD;

	const auto* tex = assets.GetTexture(texName);
	if (tex)
	{
		sprite.texture = tex;
		sprite.textureRect = sf::IntRect({{0, 0}, sf::Vector2i(tex->getSize())});
	}
	_world.AddComponent(entity, sprite);
	_world.AddComponent(entity, DecorComponent{});
}

void GameScene::CreateFinish(int gridX, int gridY)
{
	auto& assets = _engine->GetAssets();
	auto entity = _world.CreateEntity();

	float px = static_cast<float>(gridX) * TILE_SIZE;
	float py = GridYToPixel(gridY) + TILE_SIZE / 2.f;

	TransformComponent transform;
	transform.position = { px, py };
	_world.AddComponent(entity, transform);

	SpriteComponent sprite;
	const auto* tex = assets.GetTexture(AssetManager::TEX_FINISH);
	if (tex)
	{
		sprite.texture = tex;
		sprite.textureRect = sf::IntRect({{0, 0}, sf::Vector2i(tex->getSize())});
		sprite.origin = { 0.f, static_cast<float>(tex->getSize().y) - 8.f };
	}
	_world.AddComponent(entity, sprite);

	BoxColliderComponent collider;
	collider.size = { 64.f, 1000.f };
	collider.offset = { 0.f, -500.f };
	_world.AddComponent(entity, collider);

	_world.AddComponent(entity, CollisionComponent{});
	_world.AddComponent(entity, FinishComponent{});
}

void GameScene::OnActivate()
{
	SetupInputActions();

	// Reset world for fresh game
	_world = World();
	_gameState = {};
	_systems = SystemManager();
	_levelTime = 0.f;

	LoadGameConfig("assets/configs/game.json");
	LoadLevelConfig("assets/configs/level.json");

	// Re-create camera entities
	auto camera = _world.CreateEntity();
	_world.AddComponent(camera, DefaultCameraComponent{});
	auto followCam = _world.CreateEntity();
	_world.AddComponent(followCam, FollowXCameraComponent{});

	// Re-register systems
	_systems.AddSystem<PlayerControlSystem>(_engine->GetInputManager());
	_systems.AddSystem<ShooterSystem>(_gameState, _engine->GetAssets());
	_systems.AddSystem<GravitySystem>();
	_systems.AddSystem<MovementSystem>();
	_systems.AddSystem<CollisionSystem>();
	_systems.AddSystem<TileCollisionSystem>();
	_systems.AddSystem<BrickSystem>(_engine->GetAssets(), _engine->GetWindow(),
		_gameState, [this](const std::string& scene) { _engine->ChangeScene(scene); });
	_systems.AddSystem<PlayerAnimationSystem>();
	_systems.AddSystem<AnimationSystem>();
	_systems.AddSystem<CameraSystem>(_engine->GetWindow(), *_engine);
	_systems.AddSystem<RenderSystem>(_engine->GetWindow(), _gameState, _engine->GetAssets(), _renderMode);
}

void GameScene::Update(float dt)
{
	if (_isPaused)
		return;

	_levelTime += dt;

	// Check player fell below screen
	auto* playerPool = _world.GetPool<PlayerComponent>();
	auto* transformPool = _world.GetPool<TransformComponent>();
	if (playerPool && transformPool)
	{
		for (auto entity : playerPool->GetDenseEntities())
		{
			auto& transform = transformPool->Get(entity);
			if (transform.position.y > 1000.f)
			{
				// Respawn at start position
				transform.position = {
					1.f * TILE_SIZE + TILE_SIZE / 2.f,
					GridYToPixel(2) + TILE_SIZE / 2.f
				};
			}
		}
	}

	_systems.Update(_world, dt);
}

void GameScene::Render()
{
	auto& window = _engine->GetWindow();
	window.clear(sf::Color(0x64, 0x64, 0xFF));

	_systems.Update(_world, 0.f);

	auto* renderSys = _systems.GetSystem<RenderSystem>();
	if (renderSys)
		renderSys->Render(_world, window, _engine->GetAssets(), _renderMode);

	// HUD — draw in screen space (reset view)
	auto* font = _engine->GetAssets().GetFont(AssetManager::FONT_MAIN);
	sf::View prevView = window.getView();
	window.setView(window.getDefaultView());

	if (font)
	{
		char buf[64];

		int minutes = static_cast<int>(_levelTime) / 60;
		float seconds = _levelTime - minutes * 60;
		snprintf(buf, sizeof(buf), "Time: %d:%05.2f", minutes, seconds);

		sf::Text hudText(*font, buf, 20);
		hudText.setFillColor(sf::Color::White);
		hudText.setPosition({ 10.f, 10.f });
		window.draw(hudText);

		snprintf(buf, sizeof(buf), "Score: %d", _gameState.score);
		sf::Text scoreText(*font, buf, 20);
		scoreText.setFillColor(sf::Color::White);
		scoreText.setPosition({ 10.f, 40.f });
		window.draw(scoreText);
	}

	window.setView(prevView);
}

void GameScene::HandleEvent(const sf::Event& event)
{
	if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>())
	{
		if (keyPressed->scancode == sf::Keyboard::Scancode::Escape)
		{
			_engine->ChangeScene("menu");
		}
		if (keyPressed->scancode == sf::Keyboard::Scancode::P)
		{
			_isPaused = !_isPaused;
		}
	}
}