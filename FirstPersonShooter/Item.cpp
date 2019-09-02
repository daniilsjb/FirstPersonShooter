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

AmmoItem::AmmoItem(EngineFPS *engine, int weaponIndex, int amount, Sprite *spr) : Item(engine), weaponIndex(weaponIndex), amount(amount)
{
	texture = spr;
}

void AmmoItem::OnUse(Player *player)
{
	if (player->weapon->GetAmmo() < player->weapon->GetCapacity())
	{
		player->weapon->AddAmmo(amount);
		removed = true;
		engine->PlayAudio("Ammo");
	}
}

ScoreItem::ScoreItem(EngineFPS *engine, int worth, Sprite *spr) : Item(engine), worth(worth)
{
	texture = spr;
}

void ScoreItem::OnUse(Player *player)
{
	player->AddScore(worth);
	removed = true;
	engine->PlayAudio("Pickup");
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