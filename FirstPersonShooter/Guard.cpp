#include "Guard.h"
#include "EngineFPS.h"
#include "Weapon.h"
#include "Player.h"

Guard::Guard(EngineFPS* engine, float x, float y) : Enemy(engine, x, y)
{
	directionSprites[BACK] = engine->GetSprite("Guard Back");
	directionSprites[RIGHT] = engine->GetSprite("Guard Right");
	directionSprites[FRONT] = engine->GetSprite("Guard Front");
	directionSprites[LEFT] = engine->GetSprite("Guard Left");

	aimingSpr = engine->GetSprite("Guard Aim");
	shootingSpr = engine->GetSprite("Guard Fire");
	painSpr = engine->GetSprite("Guard Pain");

	currentHealth = maxHealth = 5;

	speed = 1.5f;

	delayTimer = 0.05f;

	weapon = engine->CreateWeapon(Weapons::PISTOL, this);
}

void Guard::OnHit(int damage)
{
	playerDetected = true;

	Damage(damage);
	state = PAIN;
	painTimer = 0.0f;
	if (currentHealth <= 0)
	{
		removed = true;
		if (engine->player->WeaponAcquired(Weapons::PISTOL))
			engine->AddItem((int)x, (int)y, 'B');
		else
			engine->AddItem((int)x, (int)y, 'P');

		engine->PlayAudio("Enemy Death 1");
	}

	if (rand() % 5 == 0)
		engine->PlayAudio("Enemy Pain");
}