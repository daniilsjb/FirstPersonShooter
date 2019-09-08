#include "AssetManager.h"
#include "ConsoleGameEngine.h"

AssetManager::AssetManager() {}

AssetManager::~AssetManager()
{
	for (auto &spr : sprites)
	{
		delete spr.second;
		spr.second = nullptr;
	}
}

Sprite* AssetManager::GetSprite(std::string spriteName) const
{
	return sprites.at(spriteName);
}

void AssetManager::LoadSprites()
{
	auto load = [&](std::string spriteName, std::wstring fileName)
	{
		Sprite *spr = new Sprite(fileName);
		sprites[spriteName] = spr;
	};

	//Walls
	load("Blue Wall Cage", L"Sprites/Walls/Blue Wall Cage.spr");
	load("Blue Wall", L"Sprites/Walls/Blue Wall.spr");
	load("Brick Wall", L"Sprites/Walls/Brick Wall.spr");
	load("Metal Door", L"Sprites/Walls/Metal Door.spr");
	load("Next Level", L"Sprites/Walls/Next Level.spr");
	load("Stone Wall Eagle", L"Sprites/Walls/Stone Wall Eagle.spr");
	load("Stone Wall Flag", L"Sprites/Walls/Stone Wall Flag.spr");
	load("Stone Wall", L"Sprites/Walls/Stone Wall.spr");

	//Decorations
	load("Armor", L"Sprites/Decorations/Armor.spr");
	load("Barrel", L"Sprites/Decorations/Barrel.spr");
	load("Bones", L"Sprites/Decorations/Bones.spr");
	load("Bowl", L"Sprites/Decorations/Bowl.spr");
	load("Empty Well", L"Sprites/Decorations/Empty Well.spr");
	load("Filled Well", L"Sprites/Decorations/Filled Well.spr");
	load("Flag", L"Sprites/Decorations/Flag.spr");
	load("Floor Lamp", L"Sprites/Decorations/Floor Lamp.spr");
	load("Golden Lamp", L"Sprites/Decorations/Golden Lamp.spr");
	load("Green Barrel", L"Sprites/Decorations/Green Barrel.spr");
	load("Jug", L"Sprites/Decorations/Jug.spr");
	load("Puddle", L"Sprites/Decorations/Puddle.spr");
	load("Simple Lamp", L"Sprites/Decorations/Simple Lamp.spr");
	load("Skeleton", L"Sprites/Decorations/Skeleton.spr");
	load("Table", L"Sprites/Decorations/Table.spr");
	load("Tree In Blue Jug", L"Sprites/Decorations/Tree In Blue Jug.spr");
	load("Tree", L"Sprites/Decorations/Tree.spr");

	//Enemies
	load("Guard Aim", L"Sprites/Enemies/Guard Aim.spr");
	load("Guard Back", L"Sprites/Enemies/Guard Back.spr");
	load("Guard Fire", L"Sprites/Enemies/Guard Fire.spr");
	load("Guard Front", L"Sprites/Enemies/Guard Front.spr");
	load("Guard Left", L"Sprites/Enemies/Guard Left.spr");
	load("Guard Pain", L"Sprites/Enemies/Guard Pain.spr");
	load("Guard Right", L"Sprites/Enemies/Guard Right.spr");

	load("SS Aim", L"Sprites/Enemies/SS Aim.spr");
	load("SS Back", L"Sprites/Enemies/SS Back.spr");
	load("SS Fire", L"Sprites/Enemies/SS Fire.spr");
	load("SS Front", L"Sprites/Enemies/SS Front.spr");
	load("SS Left", L"Sprites/Enemies/SS Left.spr");
	load("SS Pain", L"Sprites/Enemies/SS Pain.spr");
	load("SS Right", L"Sprites/Enemies/SS Right.spr");

	//Items
	load("Item Chalice", L"Sprites/Items/Chalice.spr");
	load("Item Chest", L"Sprites/Items/Chest.spr");
	load("Item Cross", L"Sprites/Items/Cross.spr");
	load("Item Crown", L"Sprites/Items/Crown.spr");
	load("Item Dinner", L"Sprites/Items/Dinner.spr");
	load("Item Ammo", L"Sprites/Items/Ammo.spr");
	load("Item Machine Gun", L"Sprites/Items/Machine Gun.spr");
	load("Item Medkit", L"Sprites/Items/Medkit.spr");
	load("Item One Up", L"Sprites/Items/One Up.spr");
	load("Item Pistol", L"Sprites/Items/Pistol.spr");

	//Weapons
	load("Pistol", L"Sprites/Weapons/Pistol.spr");
	load("Pistol Fire", L"Sprites/Weapons/Pistol Fire.spr");
	load("Pistol Cooldown", L"Sprites/Weapons/Pistol Cooldown.spr");
	load("Machine Gun", L"Sprites/Weapons/Machine Gun.spr");
	load("Machine Gun Fire", L"Sprites/Weapons/Machine Gun Fire.spr");
	load("Machine Gun Cooldown", L"Sprites/Weapons/Machine Gun Cooldown.spr");
	load("Knife", L"Sprites/Weapons/Knife.spr");
	load("Knife Fire", L"Sprites/Weapons/Knife Fire.spr");
	load("Knife Cooldown", L"Sprites/Weapons/Knife Cooldown.spr");
}