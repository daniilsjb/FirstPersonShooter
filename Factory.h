#pragma once

class EngineFPS;
class Wall;
class Item;
class Decoration;
class DynamicObject;
class Mob;
class Weapon;

namespace Factory
{
	Weapon* MakeWeapon(EngineFPS* engine, short weaponID, Mob* parent);

	Wall* MakeWall(EngineFPS* engine, int x, int y, wchar_t type);
	Item* MakeItem(EngineFPS* engine, int x, int y, wchar_t type);
	Decoration* MakeDecoration(EngineFPS* engine, int x, int y, wchar_t type);
	DynamicObject* MakeDynamicObject(EngineFPS* engine, int x, int y, wchar_t type);
}