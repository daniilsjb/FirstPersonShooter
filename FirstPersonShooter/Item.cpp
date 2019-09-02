#include "Item.h"
#include "Player.h"
#include "Weapon.h"

Item::Item(EngineFPS *engine) : GameObject(engine) {}

Item::~Item() {}

WeaponItem::WeaponItem(EngineFPS *engine, Weapon *weapon, Sprite *spr) : Item(engine), weapon(weapon)
{
	texture = spr;
}

void WeaponItem::OnUse(Player *player)
{
	if (!player->AddWeapon(weapon))
	{
		if (player->AddAmmoFromWeapon(weapon))
		{
			removed = true;
			delete weapon;
		}
	}
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