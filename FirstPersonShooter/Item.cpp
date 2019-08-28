#include "Item.h"
#include "Player.h"
#include "Weapon.h"

Item::Item(EngineFPS *engine) : GameObject(engine) {}

Item::~Item() {}

GunItem::GunItem(EngineFPS *engine) : Item(engine)
{
	texture = engine->sprites["item gun"];
}

void GunItem::OnUse(Player *player)
{
	Weapon *gun = new Gun(engine, player);

	if (!player->AddWeapon(gun))
		delete gun;
	else
		removed = true;
}

Medkit::Medkit(EngineFPS *engine, int healthBonus, Sprite *spr) : Item(engine), healthBonus(healthBonus)
{
	texture = spr;
}

void Medkit::OnUse(Player *player)
{
	if (player->GetHealth() < player->GetMaxHealth())
	{
		player->Heal(healthBonus);
		removed = true;
	}
}