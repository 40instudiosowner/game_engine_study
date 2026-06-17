#include "AssetManager.h"
#include <iostream>
#include <fstream>

const std::string AssetManager::TEX_MEGA_IDLE = "MegaIdle";
const std::string AssetManager::TEX_MEGA_RUN = "MegaRun";
const std::string AssetManager::TEX_MEGA_JUMP = "MegaJump";
const std::string AssetManager::TEX_MEGA_SHOOT_IDLE = "MegaShootIdle";
const std::string AssetManager::TEX_MEGA_SHOOT_RUN = "MegaShootRun";
const std::string AssetManager::TEX_MEGA_SHOOT_JUMP = "MegaShootJump";
const std::string AssetManager::TEX_BULLET = "Bullet";
const std::string AssetManager::TEX_TILE = "Tile";
const std::string AssetManager::TEX_BRICK_TILE = "BrickTile";
const std::string AssetManager::TEX_QUESTION_TILE = "QuestionTile";
const std::string AssetManager::TEX_QUESTION_INACTIVE = "QuestionInactiveTile";
const std::string AssetManager::TEX_BIG_HILL = "BigHill";
const std::string AssetManager::TEX_SMALL_HILL = "SmallHill";
const std::string AssetManager::TEX_CLOUD = "Cloud";
const std::string AssetManager::TEX_PIPE_L = "PipeL";
const std::string AssetManager::TEX_PIPE_R = "PipeR";
const std::string AssetManager::TEX_PIPE_UP_L = "PipeUpL";
const std::string AssetManager::TEX_PIPE_UP_R = "PipeUpR";
const std::string AssetManager::TEX_FINISH = "Finish";
const std::string AssetManager::TEX_TILE1 = "Tile1";

const std::string AssetManager::ANIM_MEGA_IDLE = "MegaIdle";
const std::string AssetManager::ANIM_MEGA_RUN = "MegaRun";
const std::string AssetManager::ANIM_MEGA_JUMP = "MegaJump";
const std::string AssetManager::ANIM_MEGA_SHOOT_IDLE = "MegaShootIdle";
const std::string AssetManager::ANIM_MEGA_SHOOT_RUN = "MegaShootRun";
const std::string AssetManager::ANIM_MEGA_SHOOT_JUMP = "MegaShootJump";
const std::string AssetManager::ANIM_EXPLOSION = "Explosion";
const std::string AssetManager::ANIM_COIN = "CoinAnim";

const std::string AssetManager::FONT_MAIN = "MainFont";

bool AssetManager::LoadAssets(const std::string& configPath)
{
	std::ifstream file(configPath);
	if (!file.is_open())
	{
		std::cerr << "Could not open assets config: " << configPath << "\n";
		return false;
	}

	json config;
	try
	{
		file >> config;
	}
	catch (const std::exception& e)
	{
		std::cerr << "Error parsing assets config: " << e.what() << "\n";
		return false;
	}

	if (config.contains("Textures"))
	{
		if (!LoadTextures(config["Textures"]))
			return false;
	}

	if (config.contains("Animations"))
	{
		if (!LoadAnimations(config["Animations"]))
			return false;
	}

	if (config.contains("Fonts"))
	{
		if (!LoadFonts(config["Fonts"]))
			return false;
	}

	return true;
}

bool AssetManager::LoadTextures(const json& textures)
{
	for (const auto& tex : textures)
	{
		std::string name = tex["name"];
		std::string path = tex["path"];

		auto texture = std::make_unique<sf::Texture>();
		if (!texture->loadFromFile(path))
		{
			std::cerr << "Failed to load texture: " << path << "\n";
			return false;
		}

		_textures[name] = std::move(texture);
		std::cout << "Loaded texture: " << name << " from " << path << "\n";
	}
	return true;
}

bool AssetManager::LoadAnimations(const json& animations)
{
	for (const auto& anim : animations)
	{
		Animation animation;
		animation.name = anim["name"];
		std::string texName = anim["textureName"];
		animation.frameCount = anim["frameCount"];
		animation.frameDuration = anim["frameDuration"];

		auto it = _textures.find(texName);
		if (it == _textures.end())
		{
			std::cerr << "Animation references unknown texture: " << texName << "\n";
			return false;
		}

		animation.texture = it->second.get();
		sf::Vector2u texSize = it->second->getSize();
		animation.frameWidth = texSize.x / animation.frameCount;
		animation.frameHeight = texSize.y;

		_animations[animation.name] = std::move(animation);
		std::cout << "Loaded animation: " << animation.name
			<< " frames=" << animation.frameCount
			<< " duration=" << animation.frameDuration
			<< " frameSize=" << animation.frameWidth << "x" << animation.frameHeight << "\n";
	}
	return true;
}

bool AssetManager::LoadFonts(const json& fonts)
{
	for (const auto& font : fonts)
	{
		std::string name = font["name"];
		std::string path = font["path"];

		auto sfFont = std::make_unique<sf::Font>();
		if (!sfFont->openFromFile(path))
		{
			std::cerr << "Failed to load font: " << path << "\n";
			return false;
		}

		_fonts[name] = std::move(sfFont);
		std::cout << "Loaded font: " << name << " from " << path << "\n";
	}
	return true;
}

const sf::Texture* AssetManager::GetTexture(const std::string& name) const
{
	auto it = _textures.find(name);
	if (it != _textures.end())
		return it->second.get();
	return nullptr;
}

sf::Font* AssetManager::GetFont(const std::string& name)
{
	auto it = _fonts.find(name);
	if (it != _fonts.end())
		return it->second.get();
	return nullptr;
}

const Animation* AssetManager::GetAnimation(const std::string& name) const
{
	auto it = _animations.find(name);
	if (it != _animations.end())
		return &it->second;
	return nullptr;
}