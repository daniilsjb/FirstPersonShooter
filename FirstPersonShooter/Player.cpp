#include "Player.h"
#include "EngineFPS.h"
#include "Weapon.h"
#include "Wall.h"

Player::Player(EngineFPS* engine) : Mob(engine)
{
	speed = 5.0f;
	currentHealth = maxHealth = 100;

	friendlyToPlayer = true;

	availableWeapons.resize(Weapons::COUNT, nullptr);
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

bool Player::WeaponAcquired(short weaponID) const
{
	return (availableWeapons[weaponID] != nullptr);
}

bool Player::AddWeapon(short weaponID)
{
	if (availableWeapons[weaponID] == nullptr)
	{
		weapon = availableWeapons[weaponID] = engine->CreateWeapon(weaponID, this);
		return true;
	}
	return false;
}

bool Player::AddAmmo(short weaponID, int amount)
{
	Weapon *wpn = availableWeapons[weaponID];
	if (wpn != nullptr)
	{
		if (!wpn->IsFull())
		{
			wpn->AddAmmo(amount);
			return true;
		}
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

	if (engine->GetKey('C').released)
		OnHit(999999);

	if (engine->GetKey('A').held)
		angle -= (speed * 0.45f) * elapsedTime;

	if (engine->GetKey('D').held)
		angle += (speed * 0.45f) * elapsedTime;

	if (engine->GetKey('I').released)
	{
		float rayX, rayY, distance;
		if (engine->CastRay(x, y, angle, rayX, rayY, distance, true, false))
		{
			if (distance < 2.0f)
			{
				engine->GetWall(rayX, rayY)->OnInteract();
			}
		}
	}

	for (int i = 1; i <= Weapons::COUNT; i++)
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

	if (rand() % 5 == 0)
		engine->PlayAudio("Player Pain 1");

	if (currentHealth <= 0)
		engine->PlayAudio("Player Death");
}

void Player::AddScore(int amount)
{
	score += amount;
}

int Player::GetScore() const
{
	return score;
}