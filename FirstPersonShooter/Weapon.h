#pragma once

class EngineFPS;
class Sprite;
class Mob;

class Weapon
{
public:
	Weapon(EngineFPS *engine, Mob *parent, short index);
	virtual ~Weapon();

	Mob *parent = nullptr;

	const short WEAPON_INDEX;

	void OnUpdate(float elapsedTime);
	virtual void Fire() = 0;

	bool Ready() const;

	int GetAmmo() const;
	int GetCapacity() const;

	void AddAmmo(int amount);

	Sprite *currentSpr = nullptr;

protected:
	EngineFPS *engine = nullptr;


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