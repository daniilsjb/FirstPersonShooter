#include "Player.h"
#include "Wall.h"

Player::Player(EngineFPS *engine) : Mob(engine)
{
	speed = 5.0f;
	maxHealth = 100;
	currentHealth = 50;

	friendlyToPlayer = true;

	availableWeapons.resize(WEAPON_COUNT, nullptr);
}

Player::~Player()
{
	for (auto &w : availableWeapons)
	{
		delete w;
		w = nullptr;
	}
	weapon = nullptr;
}

bool Player::AddWeapon(Weapon *weapon)
{
	if (availableWeapons[weapon->WEAPON_INDEX] == nullptr)
	{
		availableWeapons[weapon->WEAPON_INDEX] = weapon;

		if (this->weapon == nullptr)
			this->weapon = weapon;

		return true;
	}
	return false;
}

void Player::OnUpdate(float elapsedTime)
{
	if (engine->GetKey('W').held)
	{
		x += cosf(angle) * speed * elapsedTime;
		y += sinf(angle) * speed * elapsedTime;
		if (engine->IsObstacle(x, y, this)) {
			x -= cosf(angle) * speed * elapsedTime;
			y -= sinf(angle) * speed * elapsedTime;
		}
	}

	if (engine->GetKey('S').held)
	{
		x -= cosf(angle) * speed * elapsedTime;
		y -= sinf(angle) * speed * elapsedTime;
		if (engine->IsObstacle(x, y, this)) {
			x += cosf(angle) * speed * elapsedTime;
			y += sinf(angle) * speed * elapsedTime;
		}
	}

	if (engine->GetKey('Q').held)
	{
		x += sinf(angle) * speed * elapsedTime;
		y -= cosf(angle) * speed * elapsedTime;
		if (engine->IsObstacle(x, y, this)) {
			x -= sinf(angle) * speed * elapsedTime;
			y += cosf(angle) * speed * elapsedTime;
		}
	}

	if (engine->GetKey('E').held)
	{
		x -= sinf(angle) * speed * elapsedTime;
		y += cosf(angle) * speed * elapsedTime;
		if (engine->IsObstacle(x, y, this)) {
			x += sinf(angle) * speed * elapsedTime;
			y -= cosf(angle) * speed * elapsedTime;
		}
	}

	if (engine->GetKey('A').held)
		angle -= (speed * 0.75f) * elapsedTime;

	if (engine->GetKey('D').held)
		angle += (speed * 0.75f) * elapsedTime;

	if (engine->GetKey('I').released)
	{
		float rayX, rayY, distance;
		if (engine->CastRay(x, y, angle, rayX, rayY, distance, true, false))
		{
			if (distance < 2.0f)
			{
				Wall* wall = engine->GetWall(rayX, rayY);
				wall->OnInteract();
			}
		}
	}

	for (int i = 1; i <= WEAPON_COUNT; i++)
	{
		if (engine->GetKey('0' + i).released)
		{
			weapon = availableWeapons[i - 1];
		}
	}

	if (engine->GetKey(' ').released)
	{
		if (weapon != nullptr)
			weapon->Fire();
	}

	if (weapon != nullptr)
		weapon->OnUpdate(elapsedTime);
}

void Player::OnHit(int damage)
{
	Damage(damage);
}