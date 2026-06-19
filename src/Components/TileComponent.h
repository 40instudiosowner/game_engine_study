#pragma once

enum class TileType
{
	Normal,
	Brick,
	Question
};

struct TileComponent
{
	TileType type = TileType::Normal;
};