#pragma once

#include <SFML/Graphics.hpp>
#include <nlohmann/json.hpp>
#include <string>
#include <unordered_map>
#include <memory>
#include "Animation.h"

using json = nlohmann::json;

class AssetManager
{
public:
	bool LoadAssets(const std::string& configPath);

	const sf::Texture* GetTexture(const std::string& name) const;
	sf::Font* GetFont(const std::string& name);
	const Animation* GetAnimation(const std::string& name) const;

	const std::unordered_map<std::string, std::unique_ptr<sf::Texture>>& GetAllTextures() const { return _textures; }
	const std::unordered_map<std::string, Animation>& GetAllAnimations() const { return _animations; }
	const std::unordered_map<std::string, std::unique_ptr<sf::Font>>& GetAllFonts() const { return _fonts; }

	// Asset name constants
	static const std::string TEX_MEGA_IDLE;
	static const std::string TEX_MEGA_RUN;
	static const std::string TEX_MEGA_JUMP;
	static const std::string TEX_MEGA_SHOOT_IDLE;
	static const std::string TEX_MEGA_SHOOT_RUN;
	static const std::string TEX_MEGA_SHOOT_JUMP;
	static const std::string TEX_BULLET;
	static const std::string TEX_TILE;
	static const std::string TEX_BRICK_TILE;
	static const std::string TEX_QUESTION_TILE;
	static const std::string TEX_QUESTION_INACTIVE;
	static const std::string TEX_BIG_HILL;
	static const std::string TEX_SMALL_HILL;
	static const std::string TEX_CLOUD;
	static const std::string TEX_PIPE_L;
	static const std::string TEX_PIPE_R;
	static const std::string TEX_PIPE_UP_L;
	static const std::string TEX_PIPE_UP_R;
	static const std::string TEX_FINISH;
	static const std::string TEX_TILE1;

	static const std::string ANIM_MEGA_IDLE;
	static const std::string ANIM_MEGA_RUN;
	static const std::string ANIM_MEGA_JUMP;
	static const std::string ANIM_MEGA_SHOOT_IDLE;
	static const std::string ANIM_MEGA_SHOOT_RUN;
	static const std::string ANIM_MEGA_SHOOT_JUMP;
	static const std::string ANIM_EXPLOSION;
	static const std::string ANIM_COIN;

	static const std::string FONT_MAIN;

private:
	bool LoadTextures(const json& textures);
	bool LoadAnimations(const json& animations);
	bool LoadFonts(const json& fonts);

	std::unordered_map<std::string, std::unique_ptr<sf::Texture>> _textures;
	std::unordered_map<std::string, Animation> _animations;
	std::unordered_map<std::string, std::unique_ptr<sf::Font>> _fonts;
};