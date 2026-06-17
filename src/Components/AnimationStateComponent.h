#pragma once

#include <string>

struct AnimationStateComponent
{
	std::string currentAnimation;
	std::string nextAnimation;
	int currentFrame = 0;
	int frameTimer = 0;
	bool isLooping = true;
	bool isFinished = false;
};