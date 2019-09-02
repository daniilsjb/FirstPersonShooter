#pragma once

class EngineFPS;
class Sprite;
class Mob;

class Weapon
{
public:
	EngineFPS *engine = nullptr;
	Mob *parent = nullptr;

	Weapon(EngineFPS *engine, Mob *parent, short index);
	virtual ~Weapon();

	Sprite *currentSpr = nullptr;

	const short WEAPON_INDEX;

	void OnUpdate(float elapsedTime);
	virtual void Fire() = 0;

	bool Ready();

	int GetAmmo();
	int GetCapacity();

	void AddAmmo(int amount);

protected:
	Sprite *sprIdle = nullptr;
	Sprite *sprFire = nullptr;

	int ammo;
	int capacity;

	int minDmg;
	int maxDmg;

	bool shooting = false;
	float cooldown;
	float timer = 0.0f;
};

struct Gun : Weapon
{
	Gun(EngineFPS *engine, Mob *parent);

	void Fire() override;
};

struct MachineGun : Weapon
{
	MachineGun(EngineFPS *engine, Mob *parent);

	void Fire() override;
};