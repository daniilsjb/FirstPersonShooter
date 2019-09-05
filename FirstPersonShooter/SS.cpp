#include "SS.h"
#include "EngineFPS.h"
#include "Weapon.h"
#include "Player.h"

SS::SS(EngineFPS* engine) : Enemy(engine)
{
	directionSprites[BACK] = engine->GetSprite("SS Back");
	directionSprites[RIGHT] = engine->GetSprite("SS Right");
	directionSprites[FRONT] = engine->GetSprite("SS Front");
	directionSprites[LEFT] = engine->GetSprite("SS Left");

	aimingSpr = engine->GetSprite("SS Aim");
	shootingSpr = engine->GetSprite("SS Fire");
	painSpr = engine->GetSprite("SS Pain");

	currentHealth = maxHealth = 12;

	speed = 1.5f;

	weapon = engine->CreateWeapon(Weapons::MACHINE_GUN, this);
}

void SS::OnHit(int damage)
{
	playerDetected = true;

	Damage(damage);
	state = PAIN;
	painTimer = 0.0f;
	if (currentHealth <= 0)
	{
		removed = true;
		if (engine->player->WeaponAcquired(Weapons::MACHINE_GUN))
			engine->AddItem((int)x, (int)y, 'B');
		else
			engine->AddItem((int)x, (int)y, 'G');

		engine->PlayAudio("Enemy Death 1");
	}

	if (rand() % 5 == 0)
		engine->PlayAudio("Enemy Pain");
}