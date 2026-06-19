#include "../ECS/Systems/IUpdateSystem.h"

class AnimationSystem final : public IUpdateSystem
{
public:
	void Update(World& world, float dt) override;
};