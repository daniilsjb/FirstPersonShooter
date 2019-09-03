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
	Weapon(EngineFPS *engine, Mob *parent);
	virtual ~Weapon();

	Mob *parent = nullptr;

	Sprite *currentSpr = nullptr;

	bool Ready() const;

	int GetAmmo() const;
	int GetCapacity() const;

	void AddAmmo(int amount);

	void OnUpdate(float elapsedTime);
	virtual void Fire() = 0;

protected:
	EngineFPS *engine = nullptr;

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

class Pistol : public Weapon
{
public:
	Pistol(EngineFPS *engine, Mob *parent);

	void Fire() override;
};

class MachineGun : public Weapon
{
public:
	MachineGun(EngineFPS *engine, Mob *parent);

	void Fire() override;
};