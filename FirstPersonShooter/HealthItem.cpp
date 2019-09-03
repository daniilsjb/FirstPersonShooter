#include "HealthItem.h"
#include "EngineFPS.h"
#include "Player.h"

HealthItem::HealthItem(EngineFPS *engine, int healthBonus, Sprite *spr) : Item(engine), healthBonus(healthBonus)
{
	texture = spr;
}

void HealthItem::OnUse(Player *player)
{
	if (player->GetHealth() < player->GetMaxHealth())
	{
		engine->PlayAudio("Health");
		player->Heal(healthBonus);
		removed = true;
	}
}