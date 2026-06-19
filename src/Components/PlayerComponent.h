#pragma once

struct PlayerComponent
{
	int score = 0;
	float bulletSpeed = 10.f;
	float maxSpeed = 15.f;
	float jumpVelocity = 12.f;
	bool isGrounded = false;
	bool wantsJump = false;
	bool wantsShoot = false;
	bool facingRight = true;
	int shootAnimTimer = 0; // frames left for shoot animation
};