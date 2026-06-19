#include "GameObjectFactory.h"
#include "../Components/TransformComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/BoxColliderComponent.h"
#include "../Components/CollisionComponent.h"
#include "../Components/AnimationStateComponent.h"
#include "../Components/AnimatorComponent.h"
#include "../ECS/EntityId.h"

namespace Editor
{

GameObjectFactory::GameObjectFactory(sf::RenderWindow& window, World& world)
    : _window(window)
    , _world(world)
{
}

int GameObjectFactory::CreateGameObjectEntity(const sf::Texture& tex) const
{
    auto entity = _world.CreateEntity();

    TransformComponent transform;
    transform.position = {100.f, 100.f};
    _world.AddComponent(entity, transform);

    SpriteComponent sprite;
    sprite.texture = &tex;
    sprite.textureRect = sf::IntRect({{0, 0}, sf::Vector2i(tex.getSize())});
    sprite.origin = {tex.getSize().x / 2.f, tex.getSize().y / 2.f};
    _world.AddComponent(entity, sprite);

    BoxColliderComponent collider;
    collider.size = sf::Vector2f(tex.getSize());
    _world.AddComponent(entity, collider);

    _world.AddComponent(entity, CollisionComponent{});

    return static_cast<int>(entity.id);
}

int GameObjectFactory::CreateGameObjectEntity(const Animation& anim) const
{
    auto entity = _world.CreateEntity();

    TransformComponent transform;
    transform.position = {100.f, 100.f};
    _world.AddComponent(entity, transform);

    SpriteComponent sprite;
    sprite.texture = anim.texture;
    sprite.textureRect = sf::IntRect({{0, 0}, {anim.frameWidth, anim.frameHeight}});
    sprite.origin = {anim.frameWidth / 2.f, anim.frameHeight / 2.f};
    _world.AddComponent(entity, sprite);

    BoxColliderComponent collider;
    collider.size = sf::Vector2f(anim.frameWidth, anim.frameHeight);
    _world.AddComponent(entity, collider);

    _world.AddComponent(entity, CollisionComponent{});

    AnimationStateComponent animState;
    animState.currentAnimation = anim.name;
    animState.nextAnimation = anim.name;
    animState.isLooping = true;
    _world.AddComponent(entity, animState);

    AnimatorComponent animator;
    animator.animations[anim.name] = anim;
    _world.AddComponent(entity, animator);

    return static_cast<int>(entity.id);
}

}
