#include "AmmoItem.h"
#include "EngineFPS.h"
#include "Player.h"

AmmoItem::AmmoItem(EngineFPS* engine, int amount, Sprite* spr) : Item(engine), amount(amount)
{
	texture = spr;
}

void AmmoItem::OnUse(Player& player)
{
	if (player.AddAmmo(amount))
	{
		removed = true;
		engine->PlayAudio("Ammo");
	}
}