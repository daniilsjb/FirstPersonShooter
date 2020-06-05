#include "Factory.h"
#include "EngineFPS.h"

#include "Weapon.h"
#include "Pistol.h"
#include "MachineGun.h"
#include "Knife.h"

#include "Wall.h"
#include "Door.h"
#include "SecretEntrance.h"
#include "ExitDoor.h"

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
		case Weapons::KNIFE:
			return new Knife(engine, parent);
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

	float wallX = x + 0.5f;
	float wallY = y + 0.5f;

	switch (type)
	{
		case '#':
		{
			wall = new Wall(engine, wallX, wallY, engine->GetSprite("Stone Wall"));
			break;
		}
		case '$':
		{
			wall = new Wall(engine, wallX, wallY, engine->GetSprite("Stone Wall Eagle"));
			break;
		}
		case '@':
		{
			wall = new Wall(engine, wallX, wallY, engine->GetSprite("Stone Wall Flag"));
			break;
		}
		case '%':
		{
			wall = new Wall(engine, wallX, wallY, engine->GetSprite("Blue Wall"));
			break;
		}
		case '^':
		{
			wall = new Wall(engine, wallX, wallY, engine->GetSprite("Blue Wall Cage"));
			break;
		}
		case '?':
		{
			wall = new SecretEntrance(engine, wallX, wallY, engine->GetSprite("Stone Wall Flag"));
			break;
		}
		case '~':
		{
			wall = new SecretEntrance(engine, wallX, wallY, engine->GetSprite("Stone Wall Eagle"));
			break;
		}
		case '*':
		{
			wall = new SecretEntrance(engine, wallX, wallY, engine->GetSprite("Stone Wall"));
			break;
		}
		case '-':
		{
			wall = new Door(engine, wallX, wallY, engine->GetSprite("Metal Door"));
			break;
		}
		case '&':
		{
			wall = new ExitDoor(engine, wallX, wallY, engine->GetSprite("Next Level"));
			break;
		}
	}

	return wall;
}

Item* Factory::MakeItem(EngineFPS* engine, int x, int y, wchar_t type)
{
	if (x < 0 || x >= engine->GetMapWidth() || y < 0 || y >= engine->GetMapHeight())
		return nullptr;

	Item* item = nullptr;

	float itemX = x + 0.5f;
	float itemY = y + 0.5f;

	switch (type)
	{
		//Health
		case 'D':
		{
			item = new HealthItem(engine, itemX, itemY, 10, engine->GetSprite("Item Dinner"));
			break;
		}
		case 'M':
		{
			item = new HealthItem(engine, itemX, itemY, 25, engine->GetSprite("Item Medkit"));
			break;
		}

		//Weapons
		case 'P':
		{
			item = new WeaponItem(engine, itemX, itemY, Weapons::PISTOL, 2, engine->GetSprite("Item Pistol"));
			break;
		}
		case 'G':
		{
			item = new WeaponItem(engine, itemX, itemY, Weapons::MACHINE_GUN, 6, engine->GetSprite("Item Machine Gun"));
			break;
		}

		//Ammo
		case 'A':
		{
			item = new AmmoItem(engine, itemX, itemY, 8, engine->GetSprite("Item Ammo"));
			break;
		}
		case 'B':
		{
			item = new AmmoItem(engine, itemX, itemY, 4, engine->GetSprite("Item Ammo"));
			break;
		}

		//Score
		case 'S':
		{
			item = new ScoreItem(engine, itemX, itemY, 100, engine->GetSprite("Item Cross"));
			break;
		}
		case 'U':
		{
			item = new ScoreItem(engine, itemX, itemY, 500, engine->GetSprite("Item Chalice"));
			break;
		}
		case 'C':
		{
			item = new ScoreItem(engine, itemX, itemY, 1000, engine->GetSprite("Item Chest"));
			break;
		}
		case 'R':
		{
			item = new ScoreItem(engine, itemX, itemY, 5000, engine->GetSprite("Item Crown"));
			break;
		}

		//Other
		case 'O':
		{
			item = new OneUp(engine, itemX, itemY, engine->GetSprite("Item One Up"));
			break;
		}
	}

	return item;
}

Decoration* Factory::MakeDecoration(EngineFPS* engine, int x, int y, wchar_t type)
{
	if (x < 0 || x >= engine->GetMapWidth() || y < 0 || y >= engine->GetMapHeight())
		return nullptr;

	Decoration* decor = nullptr;

	float decorX = x + 0.5f;
	float decorY = y + 0.5f;

	switch (type)
	{
		case 'a':
		{
			decor = new Decoration(engine, decorX, decorY, engine->GetSprite("Armor"));
			break;
		}
		case 'b':
		{
			decor = new Decoration(engine, decorX, decorY, engine->GetSprite("Bones"));
			break;
		}
		case 'p':
		{
			decor = new Decoration(engine, decorX, decorY, engine->GetSprite("Puddle"));
			break;
		}
		case 'g':
		{
			decor = new Decoration(engine, decorX, decorY, engine->GetSprite("Green Barrel"));
			break;
		}
		case 'e':
		{
			decor = new Decoration(engine, decorX, decorY, engine->GetSprite("Empty Well"));
			break;
		}
		case 'r':
		{
			decor = new Decoration(engine, decorX, decorY, engine->GetSprite("Filled Well"));
			break;
		}
		case 's':
		{
			decor = new Decoration(engine, decorX, decorY, engine->GetSprite("Skeleton"));
			break;
		}
		case 'o':
		{
			decor = new Decoration(engine, decorX, decorY, engine->GetSprite("Barrel"));
			break;
		}
		case 'f':
		{
			decor = new Decoration(engine, decorX, decorY, engine->GetSprite("Flag"));
			break;
		}
		case 'j':
		{
			decor = new Decoration(engine, decorX, decorY, engine->GetSprite("Jug"));
			break;
		}
		case 't':
		{
			decor = new Decoration(engine, decorX, decorY, engine->GetSprite("Tree"));
			break;
		}
		case 'v':
		{
			decor = new Decoration(engine, decorX, decorY, engine->GetSprite("Tree In Blue Jug"));
			break;
		}
		case 'l':
		{
			decor = new Decoration(engine, decorX, decorY, engine->GetSprite("Simple Lamp"));
			break;
		}
		case 'q':
		{
			decor = new Decoration(engine, decorX, decorY, engine->GetSprite("Golden Lamp"));
			break;
		}
		case 'w':
		{
			decor = new Decoration(engine, decorX, decorY, engine->GetSprite("Floor Lamp"));
			break;
		}
		case 'y':
		{
			decor = new Decoration(engine, decorX, decorY, engine->GetSprite("Table"));
			break;
		}
		case 'u':
		{
			decor = new Decoration(engine, decorX, decorY, engine->GetSprite("Bowl"));
			break;
		}
	}

	return decor;
}

DynamicObject* Factory::MakeDynamicObject(EngineFPS* engine, int x, int y, wchar_t type)
{
	if (x < 0 || x >= engine->GetMapWidth() || y < 0 || y >= engine->GetMapHeight())
		return nullptr;

	DynamicObject* object = nullptr;

	float objX = x + 0.5f;
	float objY = y + 0.5f;

	switch (type)
	{
		case '0':
		{
			object = new Player(engine, objX, objY);
			break;
		}
		case '1':
		{
			object = new Guard(engine, objX, objY);
			break;
		}
		case '2':
		{
			object = new SS(engine, objX, objY);
			break;
		}
	}

	return object;
}