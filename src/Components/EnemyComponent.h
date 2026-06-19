#pragma once

#include <SFML/System/Vector2.hpp>

struct EnemyComponent
{
    float patrolSpeed = 2.f;
    float patrolLeft = 0.f;
    float patrolRight = 0.f;
    bool movingRight = false;
};
