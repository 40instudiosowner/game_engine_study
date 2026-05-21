#pragma once

struct GameState
{
	bool isGameOver = false;
	bool shouldRestart = false;
	int score = 0;

	// Asteroid spawn settings
	float asteroidSpawnInterval = 1.5f;
	bool spawnAsteroidRequest = false;
};