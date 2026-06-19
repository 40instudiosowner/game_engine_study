#include "GoombaSystem.h"
#include "../Components/EnemyComponent.h"
#include "../Components/TransformComponent.h"
#include "../Components/MovementComponent.h"
#include "../Components/SpriteComponent.h"
#include "../ECS/World.h"
#include "../ECS/FilterBuilder.h"

void GoombaSystem::Update(World& world, float dt)
{
    auto filter = FilterBuilder(world)
        .With<EnemyComponent>()
        .With<TransformComponent>()
        .With<MovementComponent>()
        .Build();

    auto* enemyPool = world.GetPool<EnemyComponent>();
    auto* transformPool = world.GetPool<TransformComponent>();
    auto* movementPool = world.GetPool<MovementComponent>();
    auto* spritePool = world.GetPool<SpriteComponent>();

    if (!enemyPool || !transformPool || !movementPool)
        return;

    for (auto entity : filter)
    {
        auto& enemy = enemyPool->Get(entity);
        auto& transform = transformPool->Get(entity);
        auto& movement = movementPool->Get(entity);

        // Patrol logic: move left/right within bounds
        if (enemy.movingRight)
        {
            movement.direction.x = 1.f;
            if (transform.position.x >= enemy.patrolRight)
                enemy.movingRight = false;
        }
        else
        {
            movement.direction.x = -1.f;
            if (transform.position.x <= enemy.patrolLeft)
                enemy.movingRight = true;
        }
        movement.speed = enemy.patrolSpeed;

        // Flip sprite based on direction
        if (spritePool && spritePool->Has(entity))
        {
            auto& sprite = spritePool->Get(entity);
            if (!enemy.movingRight)
                sprite.scale.x = -std::abs(sprite.scale.x);
            else
                sprite.scale.x = std::abs(sprite.scale.x);
        }
    }
}
