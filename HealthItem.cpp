#include "HealthItem.h"
#include "EngineFPS.h"
#include "Player.h"

HealthItem::HealthItem(EngineFPS* engine, float x, float y, int healthBonus, Sprite* spr) : Item(engine, x, y), healthBonus(healthBonus)
{
	texture = spr;
}

void HealthItem::OnUse(Player& player)
{
	if (!player.FullHealth())
	{
		player.Heal(healthBonus);
		removed = true;
		engine->PlayAudio("Health");
	}
}