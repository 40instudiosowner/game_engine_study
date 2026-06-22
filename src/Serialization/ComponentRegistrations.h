#pragma once

#include "ComponentsMetadata.h"
#include "ComponentSerializerSerialization.h"
#include "../Components/TransformComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/MovementComponent.h"
#include "../Components/VelocityComponent.h"
#include "../Components/GravityComponent.h"
#include "../Components/BoxColliderComponent.h"
#include "../Components/CircleColliderComponent.h"
#include "../Components/CollisionComponent.h"
#include "../Components/PlayerComponent.h"
#include "../Components/BulletComponent.h"
#include "../Components/ShooterComponent.h"
#include "../Components/EnemyComponent.h"
#include "../Components/TileComponent.h"
#include "../Components/BrickComponent.h"
#include "../Components/DecorComponent.h"
#include "../Components/FinishComponent.h"
#include "../Components/AnimationStateComponent.h"
#include "../Components/AnimatorComponent.h"
#include "../Components/BoundsComponent.h"
#include "../Components/AsteroidComponent.h"
#include "../Components/DefaultCameraComponent.h"
#include "../Components/FollowXCameraComponent.h"

// Transform
COMPONENT(TransformComponent,
    FIELD(position),
    FIELD(rotation),
    FIELD(scale)
)

// Movement
COMPONENT(MovementComponent,
    FIELD(direction),
    FIELD(speed)
)

// Velocity
COMPONENT(VelocityComponent,
    FIELD(velocity)
)

// Gravity
COMPONENT(GravityComponent,
    FIELD(gravity)
)

// BoxCollider
COMPONENT(BoxColliderComponent,
    FIELD(radius),
    FIELD(size),
    FIELD(offset)
)

// CircleCollider
COMPONENT(CircleColliderComponent,
    FIELD(radius)
)

// Collision
COMPONENT(CollisionComponent,
    FIELD(collidedEntities)
)

// Player
COMPONENT(PlayerComponent,
    FIELD(score),
    FIELD(bulletSpeed),
    FIELD(maxSpeed),
    FIELD(jumpVelocity),
    FIELD(isGrounded),
    FIELD(wantsJump),
    FIELD(wantsShoot),
    FIELD(facingRight),
    FIELD(shootAnimTimer)
)

// Sprite
COMPONENT(SpriteComponent,
    FIELD(textureName),
    FIELD(textureRect),
    FIELD(color),
    FIELD(visible),
    FIELD(origin),
    FIELD(scale),
    FIELD(rotation)
)

// Bullet
COMPONENT(BulletComponent,
    FIELD(spawnPosition),
    FIELD(maxDistance)
)

// Shooter
COMPONENT(ShooterComponent,
    FIELD(cooldown),
    FIELD(currentCooldown),
    FIELD(wantsShoot)
)

// Enemy
COMPONENT(EnemyComponent,
    FIELD(patrolSpeed),
    FIELD(patrolLeft),
    FIELD(patrolRight),
    FIELD(movingRight)
)

// Tile
COMPONENT(TileComponent,
    FIELD(type)
)

// Brick (empty - no fields)
COMPONENT(BrickComponent)

// Decor (empty)
COMPONENT(DecorComponent)

// Finish (empty)
COMPONENT(FinishComponent)

// AnimationState
COMPONENT(AnimationStateComponent,
    FIELD(currentAnimation),
    FIELD(nextAnimation),
    FIELD(currentFrame),
    FIELD(frameTimer),
    FIELD(isLooping),
    FIELD(isFinished)
)

// Camera components
COMPONENT(DefaultCameraComponent)
COMPONENT(FollowXCameraComponent,
    FIELD(targetEntityId)
)

// Bounds
COMPONENT(BoundsComponent,
    FIELD(min),
    FIELD(max),
    FIELD(bounce)
)

// Asteroid (empty)
COMPONENT(AsteroidComponent)