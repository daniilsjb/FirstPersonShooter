#include "EngineFPS.h"
#include "Wall.h"
#include "Decoration.h"
#include "DynamicObject.h"
#include "Item.h"
#include "Player.h"

bool EngineFPS::OnStart()
{
	SetApplicationTitle(L"First Person Shooter");

	LoadSprites();

	//Walls and Items, by their nature, are static. We want to be able to access them based on some arbitrary map coordinates,
	//as it may save performance a lot in exchange for a bit of memory.
	walls.resize(mapWidth * mapHeight, nullptr);
	items.resize(mapWidth * mapHeight, nullptr);

	map.append(L"#$@#$@#$@#$@#$@#");
	map.append(L"#..............#");
	map.append(L"#......P.......#");
	map.append(L"#...........M..#");
	map.append(L"#..F...........#");
	map.append(L"#...........M..#");
	map.append(L"#..............#");
	map.append(L"#...####H###...#");
	map.append(L"#...$......$...#");
	map.append(L"#...@......@...#");
	map.append(L"#...$...K..$...#");
	map.append(L"#...@......@...#");
	map.append(L"#...########...#");
	map.append(L"#..............#");
	map.append(L"#..............#");
	map.append(L"################");

	ParseMap();

	//Weapon sprite itself is relatively small, so it must be scaled up depending on the screen resolution
	weaponWidth = (int)(GetScreenWidth() * 0.5f);
	weaponHeight = (int)(GetScreenHeight() * 0.5f);

	depthBuffer = new float[GetScreenWidth() * GetScreenHeight()];

	return true;
}

bool EngineFPS::OnUpdate(float elapsedTime)
{
	//Dynamic objects is where the entire game logic is stored; they must be updated first
	for (auto &object : dynamicObjects)
		object->OnUpdate(elapsedTime);
		
	//Check if there is an item under player's feet
	Item* item = GetItem(player->x, player->y);
	if (item != nullptr)
		item->OnUse(player);

	//Since everything's been updated now, we may garbage collect marked objects
	//We don't want to remove destroyed walls from the vector, so we just deallocate memory
	for (auto &wall : walls)
	{
		if (wall != nullptr && wall->removed)
		{
			delete wall;
			wall = nullptr;
		}
	}

	//Same applies to item
	for (auto &item : items)
	{
		if (item != nullptr && item->removed)
		{
			delete item;
			item = nullptr;
		}
	}

	//Dynamic objects, however, are not that easy to control, so we want to just remove it from the list completely
	dynamicObjects.remove_if([](const DynamicObject* object)
	{
		if (object->removed)
			delete object;
		return object->removed;
	});

	//Reset depth buffer
	for (int i = 0; i < GetScreenWidth() * GetScreenHeight(); i++)
		depthBuffer[i] = FLT_MAX;

	//Draw the walls, floor, and ceiling
	for (int x = 0; x < GetScreenWidth(); x++)
	{
		//Calculate ray angle depending on the column we're filling in
		float rayOffset = (float)x / (float)GetScreenWidth();
		float rayAngle = (player->angle - FoV * 0.5f) + rayOffset * FoV;

		float rayX, rayY;
		float distance;

		//Texture information about the wall we may have hit with the ray
		float sampleX = 0.0f;
		Wall* wall = nullptr;

		if (CastRay(player->x, player->y, rayAngle, rayX, rayY, distance, true, false))
		{
			wall = GetWall(rayX, rayY);

			float blockMidX = rayX + 0.5f;
			float blockMidY = rayY + 0.5f;

			float collisionX = player->x + (cosf(rayAngle) * distance);
			float collisionY = player->y + (sinf(rayAngle) * distance);

			float collisionAngle = atan2f(collisionY - blockMidY, collisionX - blockMidX);

			//Our collision angle is a value between -pi and pi, which allows us to determine the quadrant of the angle in a wall.
			//However, we need to determine the face that we've hit, which we can achieve by "rotating" the quadrant by pi/4 radians.
			//After that we can find the sample coordinate based on the decimal value of the ray's hitting point.

			//Right face
			if (collisionAngle >= -3.14159f * 0.25f && collisionAngle < 3.14159f * 0.25f)
				sampleX = 1.0f - (collisionY - (int)rayY);
			//Top face
			else if (collisionAngle >= 3.14159f * 0.25f && collisionAngle < 3.14159f * 0.75f)
				sampleX = collisionX - (int)rayX;
			//Left face
			else if (collisionAngle >= 3.14159f * 0.75f || collisionAngle < -3.14159f * 0.75f)
				sampleX = collisionY - (int)rayY;
			//Bottom face
			else if (collisionAngle >= -3.14159f * 0.75f && collisionAngle < -3.14159f * 0.25f)
				sampleX = 1.0f - (collisionX - (int)rayX);
		}

		//Ceiling is whatever is above the wall, and floor is the rest of the screen
		int ceiling = (int)((float)GetScreenHeight() * 0.5f - (float)GetScreenHeight() / (float)distance);
		int floor = GetScreenHeight() - ceiling;

		//Now we may draw the column
		for (int y = 0; y < GetScreenHeight(); y++)
		{
			if (y <= ceiling)
			{
				DrawPoint(x, y, ' ', BG_DARK_GRAY);
			}
			else if (y > ceiling && y <= floor)
			{
				depthBuffer[GetScreenWidth() * y + x] = distance;

				if (distance < depth && wall != nullptr)
				{
					Sprite* texture = wall->texture;
					float sampleY = ((float)y - (float)ceiling) / ((float)floor - (float)ceiling);
					DrawPoint(x, y, texture->SampleCharacter(sampleX, sampleY), texture->SampleColor(sampleX, sampleY));
				}
				else
					DrawPoint(x, y, ' ', BG_DARK_GRAY);
			}
			else
			{
				DrawPoint(x, y, ' ', BG_DARK_GRAY);
			}
		}

	}

	//Draw decorations
	for (auto &decoration : decorations)
	{
		float objectAngle, distance;
		if (ObjectWithinFoV(player->x, player->y, player->angle, decoration->x, decoration->y, objectAngle, distance))
		{
			DrawObject2D(decoration->texture, objectAngle, distance);
		}
	}

	//Draw items
	for (auto &item : items)
	{
		float objectAngle, distance;
		if (item != nullptr && ObjectWithinFoV(player->x, player->y, player->angle, item->x, item->y, objectAngle, distance))
		{
			DrawObject2D(item->texture, objectAngle, distance);
		}
	}

	//Draw dynamic objects
	for (auto &object : dynamicObjects)
	{
		float objectAngle, distance;
		if (object->texture != nullptr && ObjectWithinFoV(player->x, player->y, player->angle, object->x, object->y, objectAngle, distance))
		{
			DrawObject2D(object->texture, objectAngle, distance);
		}
	}

	//Draw the weapon in the bottom-middle of the screen
	/*for (int x = 0; x < weaponWidth; x++)
	{
		for (int y = 0; y < weaponHeight; y++)
		{
			float sampleX = (float)x / (float)weaponWidth;
			float sampleY = (float)y / (float)weaponHeight;

			short symbol = player->weaponSpr->SampleCharacter(sampleX, sampleY);
			short color = player->weaponSpr->SampleColor(sampleX, sampleY);
			
			if (color != BG_DARK_PINK)
			{
				int gunX = (int)((GetScreenWidth() * 0.5f) - (weaponWidth * 0.5f) + x);
				int gunY = (int)(GetScreenHeight() - weaponHeight + y);

				DrawPoint(gunX, gunY, symbol, color);
			}
		}
	}*/

	//Draw the map (for debug purposes mainly)
	for (int i = 0; i < mapWidth; i++)
	{
		for (int j = 0; j < mapHeight; j++)
		{
			DrawPoint(i, j, map[mapWidth * j + i], FG_GRAY);
		}
	}
	DrawPoint((int)player->x, (int)player->y, ' ', BG_GREEN);

	//Display player stats
	wchar_t title[256];
	swprintf_s(title, 256, L"First Person Shooter - Health: %d / %d", player->GetHealth(), player->GetMaxHealth());
	SetConsoleTitle(title);

	return true;
}

bool EngineFPS::OnDestroy()
{
	for (auto &spr : sprites)
		delete spr.second;

	for (auto &object : dynamicObjects)
		delete object;

	for (auto &wall : walls)
		delete wall;

	for (auto &item : items)
		delete item;

	for (auto &decoration : decorations)
		delete decoration;

	return true;
}

bool EngineFPS::CastRay(float x, float y, float angle, float &hitX, float &hitY, float &distance, bool againstWalls, bool againstDynamicObjects)
{
	float step = 0.01f;
	float distanceSoFar = 0.0f;

	float eyeX = cosf(angle);
	float eyeY = sinf(angle);

	while (distanceSoFar < depth)
	{
		distanceSoFar += step;

		int rayX = (int)(x + eyeX * distanceSoFar);
		int rayY = (int)(y + eyeY * distanceSoFar);

		if (rayX < 0 || rayX >= mapWidth || rayY < 0 || rayY >= mapHeight)
		{
			distanceSoFar = depth;
			return false;
		}
		else
		{
			if (againstWalls && GetWall((float)rayX, (float)rayY) != nullptr)
			{
				hitX = (float)rayX;
				hitY = (float)rayY;
				distance = distanceSoFar;
				return true;
			}

			if (againstDynamicObjects && GetDynamicObject((float)rayX, (float)rayY) != nullptr)
			{
				hitX = (float)rayX;
				hitY = (float)rayY;
				distance = distanceSoFar;
				return true;
			}
		}
	}

	return false;
}

bool EngineFPS::ObjectWithinFoV(float x0, float y0, float angle, float x1, float y1, float &objectAngle, float &distance)
{
	//Find direction and distance from one object to the other
	float objDirX = x1 - x0;
	float objDirY = y1 - y0;
	distance = sqrtf(objDirX*objDirX + objDirY*objDirY);

	//Find the angle between two objects
	objectAngle = atan2f(objDirY, objDirX) - angle;

	//Since angles may have gotten accumulated by rotations, the resulting angle between objects may exceed its -pi to pi radians range,
	//so we must revert full rotations until it's in the acceptable range
	while (objectAngle < -3.14159f)
		objectAngle += 2.0f * 3.14159f;

	while (objectAngle > 3.14159f)
		objectAngle -= 2.0f * 3.14159f;

	//We just need to check the absolute value against half of FoV to account for negative, mirrored angles
	bool withinFoV = fabs(objectAngle) < (FoV * 0.5f);

	//The object must be within field of view and within acceptable distance in order to be visible
	return (withinFoV && distance >= 0.5f && distance < depth);
}

void EngineFPS::DrawObject2D(Sprite* spr, float angle, float distance)
{
	//Find object's dimensions based on the distance
	float objectTop = ((float)GetScreenHeight() * 0.5f) - ((float)GetScreenHeight() / (float)distance);
	float objectBottom = (float)GetScreenHeight() - objectTop;
	float objectHeight = objectBottom - objectTop;

	//Width is easy to find knowing the height and the h/w aspect ratio
	float objectAspectRatio = (float)spr->GetHeight() / (float)spr->GetWidth();
	float objectWidth = objectHeight / objectAspectRatio;

	//The object is drawn relative to its middle
	float objectMid = (0.5f * (angle / (FoV * 0.5f)) + 0.5f) * (float)GetScreenWidth();

	//Draw the object
	for (float x = 0.0f; x < objectWidth; x++)
	{
		for (float y = 0.0f; y < objectHeight; y++)
		{
			float sampleX = x / objectWidth;
			float sampleY = y / objectHeight;

			short symbol = spr->SampleCharacter(sampleX, sampleY);
			short color = spr->SampleColor(sampleX, sampleY);

			int objectColumn = (int)(objectMid + x - (objectWidth * 0.5f));
			int objectRow = (int)(objectTop + y);

			if (objectColumn >= 0 && objectColumn < GetScreenWidth() && objectRow >= 0 && objectRow < GetScreenHeight())
			{
				//Dark pink is the transparency color for this game
				//We also only want to draw this pixel if there is nothing in front of it already
				if (color != BG_DARK_PINK && depthBuffer[GetScreenWidth() * objectRow + objectColumn] >= distance)
				{
					DrawPoint(objectColumn, objectRow, symbol, color);
					depthBuffer[GetScreenWidth() * objectRow + objectColumn] = distance;
				}
			}
		}
	}
}

bool EngineFPS::IsObstacle(float x, float y, GameObject* ignored)
{
	Wall *wall = GetWall(x, y);
	bool isWall = false;
	if (wall != nullptr && wall != ignored)
		isWall = true;	

	DynamicObject *object = GetDynamicObject(x, y);
	bool isObject = false;
	if (object != nullptr && object != ignored)
		isObject = true;

	return isWall || isObject;
}

Wall* EngineFPS::GetWall(float x, float y)
{
	return walls[mapWidth * (int)y + (int)x];
}

Item* EngineFPS::GetItem(float x, float y)
{
	return items[mapWidth * (int)y + (int)x];
}

Decoration* EngineFPS::GetDecoration(float x, float y)
{
	for (auto &decor : decorations)
	{
		if (ObjectsCollide(decor->x, decor->y, x, y))
			return decor;
	}
	return nullptr;
}

DynamicObject* EngineFPS::GetDynamicObject(float x, float y)
{
	for (auto &object : dynamicObjects)
	{
		if (ObjectsCollide(object->x, object->y, x, y))
			return object;
	}
	return nullptr;
}

bool EngineFPS::ObjectsCollide(float x0, float y0, float x1, float y1)
{
	return ((int)x0 == (int)x1 && (int)y0 == (int)y1);
}

void EngineFPS::LoadSprites()
{
	auto load = [&](std::string spriteName, std::wstring fileName)
	{
		Sprite *spr = new Sprite(fileName);
		sprites[spriteName] = spr;
	};

	load("brick wall", L"Sprites/brick_wall.spr");
	load("stone wall", L"Sprites/stone_wall.spr");
	load("stone wall eagle", L"Sprites/stone_wall_eagle.spr");
	load("stone wall flag", L"Sprites/stone_wall_flag.spr");
	load("metal door", L"Sprites/metal_door.spr");
	load("flag", L"Sprites/flag.spr");
	load("jug", L"Sprites/jug.spr");
	load("tree", L"Sprites/tree.spr");
	load("guard back", L"Sprites/guard_back.spr");
	load("guard front", L"Sprites/guard_front.spr");
	load("guard reload", L"Sprites/guard_reload.spr");
	load("guard fire", L"Sprites/guard_fire.spr");
	load("gun", L"Sprites/gun.spr");
	load("gun up", L"Sprites/gun_up.spr");
	load("gun fire", L"Sprites/gun_fire.spr");
	load("item gun", L"Sprites/item_gun.spr");
	load("item medpack", L"Sprites/item_medpack.spr");
	load("item medkit", L"Sprites/item_medkit.spr");
}

void EngineFPS::ParseMap()
{
	for (float x = 0.0f; x < (float)mapWidth; x++)
	{
		for (float y = 0.0f; y < (float)mapHeight; y++)
		{
			wchar_t c = map[mapWidth * (int)y + (int)x];

			switch (c)
			{
				case 'P':
				{
					player = new Player(this);
					player->x = x + 0.5f;
					player->y = y + 0.5f;
					dynamicObjects.push_front(player);
					break;
				}
				case '#':
				{
					Wall *wall = new Wall(this, sprites["stone wall"]);
					wall->x = x;
					wall->y = y;
					walls[mapWidth * (int)y + (int)x] = wall;
					break;
				}
				case '$':
				{
					Wall *wall = new Wall(this, sprites["stone wall eagle"]);
					wall->x = x;
					wall->y = y;
					walls[mapWidth * (int)y + (int)x] = wall;
					break;
				}
				case '@':
				{
					Wall *wall = new Wall(this, sprites["stone wall flag"]);
					wall->x = x;
					wall->y = y;
					walls[mapWidth * (int)y + (int)x] = wall;
					break;
				}
				case '?':
				{
					Wall *wall = new Door(this, sprites["stone wall flag"]);
					wall->x = x;
					wall->y = y;
					walls[mapWidth * (int)y + (int)x] = wall;
					break;
				}
				case 'H':
				{
					Wall *wall = new Door(this, sprites["metal door"]);
					wall->x = x;
					wall->y = y;
					walls[mapWidth * (int)y + (int)x] = wall;
					break;
				}
				case 'M':
				{
					Item *item = new Medpack(this);
					item->x = x + 0.5f;
					item->y = y + 0.5f;
					items[mapWidth * (int)y + (int)x] = item;
					break;
				}
				case 'K':
				{
					Item *item = new Medkit(this);
					item->x = x + 0.5f;
					item->y = y + 0.5f;
					items[mapWidth * (int)y + (int)x] = item;
					break;
				}
				case 'F':
				{
					Decoration *decor = new Decoration(this, sprites["flag"]);
					decor->x = x + 0.5f;
					decor->y = y + 0.5f;
					decorations.push_back(decor);
					break;
				}
				case 'J':
				{
					Decoration *decor = new Decoration(this, sprites["jug"]);
					decor->x = x + 0.5f;
					decor->y = y + 0.5f;
					decorations.push_back(decor);
					break;
				}
				case 'T':
				{
					Decoration *decor = new Decoration(this, sprites["tree"]);
					decor->x = x + 0.5f;
					decor->y = y + 0.5f;
					decorations.push_back(decor);
					break;
				}
			}
		}
	}
}

int EngineFPS::GetMapWidth()
{
	return mapWidth;
}

int EngineFPS::GetMapHeight()
{
	return mapHeight;
}

int main()
{
	EngineFPS game;
	if (game.ConstructScreen(320, 240, 4, 4))
		game.Start();

	return 0;

}