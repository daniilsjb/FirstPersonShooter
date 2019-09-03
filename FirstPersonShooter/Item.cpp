#include "Item.h"
#include "Player.h"
#include "Weapon.h"

Item::Item(EngineFPS *engine) : GameObject(engine) {}

Item::~Item() {}

bool Item::IsRemoved() const
{
	return removed;
}

WeaponItem::WeaponItem(EngineFPS *engine, short weaponID, Sprite *spr) : Item(engine), weaponID(weaponID)
{
	texture = spr;
}

void WeaponItem::OnUse(Player *player)
{
	if (player->AddWeapon(weaponID))
	{
		engine->PlayAudio("Ammo");
		removed = true;
	}
	else if(player->AddAmmoFromWeapon(weaponID))
	{
		engine->PlayAudio("Ammo");
		removed = true;
	}
}

AmmoItem::AmmoItem(EngineFPS *engine, short weaponID, int amount, Sprite *spr) : Item(engine), weaponID(weaponID), amount(amount)
{
	texture = spr;
}

void AmmoItem::OnUse(Player *player)
{
	if (player->AddAmmo(weaponID, amount))
	{
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