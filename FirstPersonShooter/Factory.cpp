#include "Factory.h"
#include "EngineFPS.h"

#include "Weapon.h"
#include "Pistol.h"
#include "MachineGun.h"

#include "Wall.h"
#include "Door.h"

#include "Decoration.h"

#include "Item.h"
#include "HealthItem.h"
#include "ScoreItem.h"
#include "WeaponItem.h"
#include "AmmoItem.h"
#include "OneUp.h"

#include "DynamicObject.h"
#include "Mob.h"
#include "Player.h"

#include "Guard.h"
#include "SS.h"

Weapon* Factory::MakeWeapon(EngineFPS* engine, short weaponID, Mob* parent)
{
	switch (weaponID)
	{
		case Weapons::PISTOL:
			return new Pistol(engine, parent);
		case Weapons::MACHINE_GUN:
			return new MachineGun(engine, parent);
		default:
			return nullptr;
	}
}

Wall* Factory::MakeWall(EngineFPS* engine, int x, int y, wchar_t type)
{
	if (x < 0 || x >= engine->GetMapWidth() || y < 0 || y >= engine->GetMapHeight())
		return nullptr;

	Wall* wall = nullptr;

	switch (type)
	{
		case '#':
		{
			wall = new Wall(engine, engine->GetSprite("Stone Wall"));
			break;
		}
		case '$':
		{
			wall = new Wall(engine, engine->GetSprite("Stone Wall Eagle"));
			break;
		}
		case '@':
		{
			wall = new Wall(engine, engine->GetSprite("Stone Wall Flag"));
			break;
		}
		case '%':
		{
			wall = new Wall(engine, engine->GetSprite("Blue Wall"));
			break;
		}
		case '^':
		{
			wall = new Wall(engine, engine->GetSprite("Blue Wall Cage"));
			break;
		}
		case '?':
		{
			wall = new Door(engine, engine->GetSprite("Stone Wall Flag"));
			break;
		}
		case '-':
		{
			wall = new Door(engine, engine->GetSprite("Metal Door"));
			break;
		}
		case '&':
		{
			wall = new Door(engine, engine->GetSprite("Next Level"));
			break;
		}
	}

	if (wall != nullptr)
	{
		wall->x = x + 0.5f;
		wall->y = y + 0.5f;
	}
	return wall;
}

Item* Factory::MakeItem(EngineFPS* engine, int x, int y, wchar_t type)
{
	if (x < 0 || x >= engine->GetMapWidth() || y < 0 || y >= engine->GetMapHeight())
		return nullptr;

	Item* item = nullptr;

	switch (type)
	{
		//Health
		case 'D':
		{
			item = new HealthItem(engine, 10, engine->GetSprite("Item Dinner"));
			break;
		}
		case 'M':
		{
			item = new HealthItem(engine, 25, engine->GetSprite("Item Medkit"));
			break;
		}

		//Weapons
		case 'P':
		{
			item = new WeaponItem(engine, Weapons::PISTOL, 2, engine->GetSprite("Item Pistol"));
			break;
		}
		case 'G':
		{
			item = new WeaponItem(engine, Weapons::MACHINE_GUN, 6, engine->GetSprite("Item Machine Gun"));
			break;
		}

		//Ammo
		case 'A':
		{
			item = new AmmoItem(engine, 8, engine->GetSprite("Item Ammo"));
			break;
		}
		case 'B':
		{
			item = new AmmoItem(engine, 4, engine->GetSprite("Item Ammo"));
			break;
		}

		//Score
		case 'S':
		{
			item = new ScoreItem(engine, 100, engine->GetSprite("Item Cross"));
			break;
		}
		case 'U':
		{
			item = new ScoreItem(engine, 500, engine->GetSprite("Item Chalice"));
			break;
		}
		case 'C':
		{
			item = new ScoreItem(engine, 1000, engine->GetSprite("Item Chest"));
			break;
		}
		case 'R':
		{
			item = new ScoreItem(engine, 5000, engine->GetSprite("Item Crown"));
			break;
		}

		//Other
		case 'O':
		{
			item = new OneUp(engine, engine->GetSprite("Item One Up"));
			break;
		}
	}

	if (item != nullptr)
	{
		item->x = x + 0.5f;
		item->y = y + 0.5f;
	}
	return item;
}

Decoration* Factory::MakeDecoration(EngineFPS* engine, int x, int y, wchar_t type)
{
	if (x < 0 || x >= engine->GetMapWidth() || y < 0 || y >= engine->GetMapHeight())
		return nullptr;

	Decoration* decor = nullptr;

	switch (type)
	{
		case 'a':
		{
			decor = new Decoration(engine, engine->GetSprite("Armor"));
			break;
		}
		case 'b':
		{
			decor = new Decoration(engine, engine->GetSprite("Bones"));
			break;
		}
		case 'p':
		{
			decor = new Decoration(engine, engine->GetSprite("Puddle"));
			break;
		}
		case 'g':
		{
			decor = new Decoration(engine, engine->GetSprite("Green Barrel"));
			break;
		}
		case 'e':
		{
			decor = new Decoration(engine, engine->GetSprite("Empty Well"));
			break;
		}
		case 'r':
		{
			decor = new Decoration(engine, engine->GetSprite("Filled Well"));
			break;
		}
		case 's':
		{
			decor = new Decoration(engine, engine->GetSprite("Skeleton"));
			break;
		}
		case 'o':
		{
			decor = new Decoration(engine, engine->GetSprite("Barrel"));
			break;
		}
		case 'f':
		{
			decor = new Decoration(engine, engine->GetSprite("Flag"));
			break;
		}
		case 'j':
		{
			decor = new Decoration(engine, engine->GetSprite("Jug"));
			break;
		}
		case 't':
		{
			decor = new Decoration(engine, engine->GetSprite("Tree"));
			break;
		}
		case 'v':
		{
			decor = new Decoration(engine, engine->GetSprite("Tree In Blue Jug"));
			break;
		}
		case 'l':
		{
			decor = new Decoration(engine, engine->GetSprite("Simple Lamp"));
			break;
		}
		case 'q':
		{
			decor = new Decoration(engine, engine->GetSprite("Golden Lamp"));
			break;
		}
		case 'w':
		{
			decor = new Decoration(engine, engine->GetSprite("Floor Lamp"));
			break;
		}
		case 'y':
		{
			decor = new Decoration(engine, engine->GetSprite("Table"));
			break;
		}
		case 'u':
		{
			decor = new Decoration(engine, engine->GetSprite("Bowl"));
			break;
		}
	}

	if (decor != nullptr)
	{
		decor->x = x + 0.5f;
		decor->y = y + 0.5f;
	}
	return decor;
}

DynamicObject* Factory::MakeDynamicObject(EngineFPS* engine, int x, int y, wchar_t type)
{
	if (x < 0 || x >= engine->GetMapWidth() || y < 0 || y >= engine->GetMapHeight())
		return nullptr;

	DynamicObject* object = nullptr;

	switch (type)
	{
		case '0':
		{
			object = new Player(engine);
			break;
		}
		case '1':
		{
			object = new Guard(engine);
			break;
		}
		case '2':
		{
			object = new SS(engine);
			break;
		}
	}

	if (object != nullptr)
	{
		object->x = x + 0.5f;
		object->y = y + 0.5f;
	}
	return object;
}