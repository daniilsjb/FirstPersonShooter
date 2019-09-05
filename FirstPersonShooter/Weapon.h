#pragma once

class EngineFPS;
class Sprite;
class Mob;

namespace Weapons
{
	const int CAPACITY = 99;
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
	void AddAmmo(int amount);
	void RestoreAmmo();

	void OnUpdate(float elapsedTime);

	virtual void OnFirePressed();
	virtual void OnFireHeld();
	virtual void OnFireReleased();

	virtual void Fire() = 0;

protected:
	EngineFPS* engine = nullptr;

	int ammo;

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