#pragma once

#include "../Animation.h"
#include <string>
#include <unordered_map>

struct AnimatorComponent
{
	std::unordered_map<std::string, Animation> animations;
};