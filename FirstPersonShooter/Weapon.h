#pragma once

class EngineFPS;
class Sprite;
class Mob;

namespace Weapons
{
	enum { PISTOL = 0, MACHINE_GUN, COUNT };
}

class Weapon
{
public:
	Weapon(EngineFPS* engine, Mob* parent);
	virtual ~Weapon();

	Mob* parent = nullptr;

	Sprite* currentSpr = nullptr;

	bool Ready() const;
	bool IsFull() const;

	int GetAmmo() const;
	int GetCapacity() const;

	void AddAmmo(int amount);

	void OnUpdate(float elapsedTime);

	virtual void OnFirePressed();
	virtual void OnFireHeld();
	virtual void OnFireReleased();

	virtual void Fire() = 0;

protected:
	EngineFPS* engine = nullptr;

	int ammo;
	const int CAPACITY = 99;

	int minDmg;
	int maxDmg;

	enum { READY, SHOOTING, COOLDOWN } state = READY;

	Sprite* readySpr = nullptr;
	Sprite* shootingSpr = nullptr;
	Sprite* cooldownSpr = nullptr;

	float shooting = 0.0f;
	float cooldown = 0.0f;
	float stateTimer = 0.0f;
};