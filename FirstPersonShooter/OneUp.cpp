#include "OneUp.h"
#include "EngineFPS.h"
#include "Player.h"

OneUp::OneUp(EngineFPS* engine, Sprite* spr) : Item(engine)
{
	texture = spr;
}

void OneUp::OnUse(Player& player)
{
	if (!player.FullHealth())
	{
		player.RestoreHealth();
		removed = true;
		engine->PlayAudio("Health");
	}
	else if (!player.FullAmmo())
	{
		player.RestoreAmmo();
		removed = true;
		engine->PlayAudio("Ammo");
	}
}