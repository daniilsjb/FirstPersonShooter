#include "Guard.h"
#include "EngineFPS.h"
#include "Weapon.h"

Guard::Guard(EngineFPS *engine) : Enemy(engine)
{
	directionSprites[BACK] = engine->GetSprite("Guard Back");
	directionSprites[RIGHT] = engine->GetSprite("Guard Right");
	directionSprites[FRONT] = engine->GetSprite("Guard Front");
	directionSprites[LEFT] = engine->GetSprite("Guard Left");

	reloadingSpr = engine->GetSprite("Guard Reload");
	shootingSpr = engine->GetSprite("Guard Fire");

	currentHealth = maxHealth = 50;

	state = PATROL;

	angle = 3.14159f;

	speed = 1.5f;

	weapon = engine->CreateWeapon(Weapons::MACHINE_GUN, this);
}

void Guard::OnHit(int damage)
{
	playerDetected = true;

	Damage(damage);
	if (currentHealth <= 0)
	{
		removed = true;
		engine->AddItem((int)x, (int)y, 'G');
		engine->PlayAudio("Enemy Death 1");
	}

	if (rand() % 5 == 0)
		engine->PlayAudio("Enemy Pain");
}