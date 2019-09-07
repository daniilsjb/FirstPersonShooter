#include "AmmoItem.h"
#include "EngineFPS.h"
#include "Player.h"

AmmoItem::AmmoItem(EngineFPS* engine, float x, float y, int amount, Sprite* spr) : Item(engine, x, y), amount(amount)
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