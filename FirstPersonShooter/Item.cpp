#include "Item.h"
#include "Player.h"
#include "Weapon.h"

Item::Item(EngineFPS *engine) : GameObject(engine) {}

Item::~Item() {}

bool Item::IsRemoved() const
{
	return removed;
}

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
			engine->PlayAudio("Ammo");
			removed = true;
			delete weapon;
		}
	}
	else
	{
		engine->PlayAudio("Ammo");
		removed = true;
	}	
}

Medkit::Medkit(EngineFPS *engine, int healthBonus, Sprite *spr) : Item(engine), healthBonus(healthBonus)
{
	texture = spr;
}

void Medkit::OnUse(Player *player)
{
	if (player->GetHealth() < player->GetMaxHealth())
	{
		engine->PlayAudio("Health");
		player->Heal(healthBonus);
		removed = true;
	}
}