#pragma once

struct ShooterComponent
{
	float cooldown = 0.25f;

	float currentCooldown = 0.f;

	bool wantsShoot = false;
};