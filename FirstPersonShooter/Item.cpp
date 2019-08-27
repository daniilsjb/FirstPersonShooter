#include "Item.h"
#include "Player.h"

Item::Item(EngineFPS *engine) : GameObject(engine) {}

Item::~Item() {}

Medpack::Medpack(EngineFPS *engine) : Item(engine)
{
	texture = engine->sprites["item medpack"];
}

void Medpack::OnUse(Player *player)
{
	if (player->GetHealth() < player->GetMaxHealth())
	{
		player->Heal(healthBonus);
		removed = true;
	}
}

Medkit::Medkit(EngineFPS *engine) : Item(engine)
{
	texture = engine->sprites["item medkit"];
}

void Medkit::OnUse(Player *player)
{
	if (player->GetHealth() < player->GetMaxHealth())
	{
		player->Heal(healthBonus);
		removed = true;
	}
}