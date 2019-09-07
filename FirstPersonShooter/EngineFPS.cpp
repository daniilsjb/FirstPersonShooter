#include "EngineFPS.h"
#include "Factory.h"
#include "Node.h"

#include "Weapon.h"
#include "Pistol.h"
#include "MachineGun.h"

#include "GameObject.h"

#include "Wall.h"

#include "Decoration.h"

#include "Item.h"

#include "DynamicObject.h"
#include "Player.h"

#include "Enemy.h"

bool EngineFPS::OnStart()
{
	assetManager.LoadSprites();
	LoadAudio();

	//Walls and Items, by their nature, are static. We want to be able to access them based on some arbitrary map coordinates,
	//as it may save performance a lot in exchange for a bit of memory.
	walls.resize(mapWidth * mapHeight, nullptr);
	items.resize(mapWidth * mapHeight, nullptr);

	//Special Characters - Walls
	//Lowercase Letters  - Decorations
	//Uppercase Letters  - Items
	//Numbers            - Dynamic Objects
	map.append(L"################################################################");
	map.append(L"#..............................................................#");
	map.append(L"#..............................................................#");
	map.append(L"#..............................................................#");
	map.append(L"#..............................................................#");
	map.append(L"#..............................................................#");
	map.append(L"#...........................#############......................#");
	map.append(L"#....######.................#j.........j#......................#");
	map.append(L"#....#AASS##.....############..y.1y..y..#####..................#");
	map.append(L"#....#AACS##.....#..........#...........#SSU#..................#");
	map.append(L"#....#AACC.#.....#.l....l...-.....q....1-.1S#..................#");
	map.append(L"#....#AAC..#.....#..........#...........#UUU#..................#");
	map.append(L"#....#####?#####.#...########...........#####..................#");
	map.append(L"#....#.SUt.tU..#.#...#......#...........#######................#");
	map.append(L"#....#.....S..1###...###....######-######......................#");
	map.append(L"#....#f.1......#...1...#........#...#..........................#");
	map.append(L"#....#....y.1..-...l...#........#.1.#..........................#");
	map.append(L"#....#.........#.....1.#........#.l.#..........................#");
	map.append(L"######C.1......#########........#...#..........................#");
	map.append(L"#.Mo##CC......1#................#...#..........................#");
	map.append(L"#.l.######-#####...........######...###........................#");
	map.append(L"#...#...#...#..............#..?1..l..a#........................#");
	map.append(L"##-##...#.l.#..............#A.###...###........................#");
	map.append(L"#...#####...#..............#AM#.#...#..........................#");
	map.append(L"#.l.#..D#...#..............#GM#.#...#..........................#");
	map.append(L"#...-.l1#.l.#..............####.#.l.#..........................#");
	map.append(L"#...#..U#.1.#..................##...##.........................#");
	map.append(L"#...######-#####...............###-###...............%%%%%%%%%%#");
	map.append(L"#.l.##1........#..........######v...v######..........%D...pooo%#");
	map.append(L"#...##.v.....v.#..........#v.1...........v#..........%.......o%#");
	map.append(L"#...##..w...w..#..........#...............#%%%%%%%%%%%.....1..%#");
	map.append(L"#....#.........#..........#................%.........%........%#");
	map.append(L"#....-.1.......#..........#..q....q...1q...-.........-........^#");
	map.append(L"#....#.........#..........#................%.........%........%#");
	map.append(L"#...##..w...w..#..........#...............#%%%....%%%%..yD.y.1%#");
	map.append(L"#...##.v.....v.#..........#v.............v#..%%..%%..%........%#");
	map.append(L"#...##.......1.#..........######.....######...%..%...%......D.%#");
	map.append(L"#.l.######-#####...............%%%-%%%........%..%...%%%r%%e%%%#");
	map.append(L"#...#####...#...................%...%.........%..%.....%%%%%%..#");
	map.append(L"#...#####...#...................%.l.%.........%.1%%%%%%%%%%%...#");
	map.append(L"#...#1..#...#...................%...%.........%..%%%.%.%.%u%%..#");
	map.append(L"#...-...#.l.#...................%...%.........%...%.........%%.#");
	map.append(L"#...#...#...#...................%...%.........%...-..........^.#");
	map.append(L"#.l.#####...#...................%.l.%.........%.1.%........u%%.#");
	map.append(L"#.1.#####.1.##############......%.1.%.........%%%%%%.%.%.%.%%..#");
	map.append(L"#................#.....1###.....%...%.........%%%%%%%%%%%%%%...#");
	map.append(L"#.....l.......l..-...1..&.#.....%...%..........................#");
	map.append(L"#................#.....1###.....%.l.%..........................#");
	map.append(L"##################?#######......%...%..........................#");
	map.append(L"#........###..#..?.M#......%%%%%%%-%%%%%%%.....................#");
	map.append(L"#........#.&..#..#.##......%bD..%...%....%.....................#");
	map.append(L"#........###.....###.......%....-.l.-.s..%.....................#");
	map.append(L"#.........###?#####........%....%...%....%.....................#");
	map.append(L"#...........#..#...........%....%...%....%.....................#");
	map.append(L"#...........#.O#...........%%%%%%...%%%%%%.....................#");
	map.append(L"#...........####...........%...P%...%....%.....................#");
	map.append(L"#..........................%.0..-.l.-D..b%.....................#");
	map.append(L"#..........................%....%...%....%.....................#");
	map.append(L"#..........................%%%%%%...%%%%%%.....................#");
	map.append(L"#..........................%.............%.....................#");
	map.append(L"#..........................%..l...l...l1A%.....................#");
	map.append(L"#..........................%1.....D......%.....................#");
	map.append(L"#..........................%^%%^%%^%%^%%^%.....................#");
	map.append(L"################################################################");

	ParseMap();

	//Weapon sprite itself is relatively small, so it must be scaled up depending on the screen resolution
	weaponWidth = (int)(GetScreenWidth() * 0.5f);
	weaponHeight = (int)(GetScreenHeight() * 0.5f);

	depthBuffer = new float[GetScreenWidth() * GetScreenHeight()];

	srand(time(0));

	StartAudio();
	PlayAudioClip(audio["Get Them Before They Get You"], true);

	return true;
}

bool EngineFPS::OnUpdate(float elapsedTime)
{
	if (player->IsDead())
	{
		gameOverTimer += elapsedTime;
		if (gameOverTimer >= 3.0f)
			return false;

		return true;
	}

	for (auto &object : dynamicObjects)
		object->OnUpdate(elapsedTime);

	UseItemUnderPlayer();

	CleanWalls();
	CleanItems();
	CleanDynamicObjects();
	CleanDecorations();

	ResetDepthBuffer();

	Render3DScene();

	DrawDynamicObjects();
	DrawItems();
	DrawDecorations();

	DrawPlayerWeapon();

	//Map may be drawn for the debug purposes
	//DrawMap();

	DisplayPlayerStats();

	return true;
}

bool EngineFPS::OnDestroy()
{
	for (auto &object : dynamicObjects)
	{
		delete object;
		object = nullptr;
	}
		
	for (auto &wall : walls)
	{
		delete wall;
		wall = nullptr;
	}

	for (auto &item : items)
	{
		delete item;
		item = nullptr;
	}

	for (auto &decoration : decorations)
	{
		delete decoration;
		decoration = nullptr;
	}

	return true;
}

void EngineFPS::UseItemUnderPlayer()
{
	Item* item = GetItem(player->GetX(), player->GetY());
	if (item != nullptr)
		item->OnUse(*player);
}

void EngineFPS::CleanWalls()
{
	for (auto &wall : walls)
	{
		if (wall != nullptr && wall->IsRemoved())
		{
			delete wall;
			wall = nullptr;
		}
	}
}

void EngineFPS::CleanDynamicObjects()
{
	dynamicObjects.remove_if([](const DynamicObject* object)
	{
		if (object->IsRemoved())
			delete object;

		return object->IsRemoved();
	});
}

void EngineFPS::CleanItems()
{
	for (auto &item : items)
	{
		if (item != nullptr && item->IsRemoved())
		{
			delete item;
			item = nullptr;
		}
	}
}

void EngineFPS::CleanDecorations()
{

}

void EngineFPS::Render3DScene()
{
	//Render each column
	for (int x = 0; x < GetScreenWidth(); x++)
	{
		//Calculate ray angle depending on the column we're filling in
		float rayOffset = (float)x / (float)GetScreenWidth();
		float rayAngle = (player->GetAngle() - FoV * 0.5f) + rayOffset * FoV;

		RenderColumn(x, rayAngle);
	}
}

void EngineFPS::RenderColumn(int col, float rayAngle)
{
	float rayX, rayY;
	float distance;

	//Texture information about the wall we may have hit with the ray
	float sampleX = 0.0f;
	Wall* wall = nullptr;

	if (CastRay(player->GetX(), player->GetY(), rayAngle, rayX, rayY, distance, true, false))
	{
		wall = GetWall(rayX, rayY);

		float blockMidX = rayX + 0.5f;
		float blockMidY = rayY + 0.5f;

		float collisionX = player->GetX() + (cosf(rayAngle) * distance);
		float collisionY = player->GetY() + (sinf(rayAngle) * distance);

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
			DrawPoint(col, y, ' ', BG_DARK_GRAY);
		}
		else if (y > ceiling && y <= floor)
		{
			depthBuffer[GetScreenWidth() * y + col] = distance;

			if (distance < depth && wall != nullptr)
			{
				Sprite* texture = wall->texture;
				float sampleY = ((float)y - (float)ceiling) / ((float)floor - (float)ceiling);
				DrawPoint(col, y, texture->SampleCharacter(sampleX, sampleY), texture->SampleColor(sampleX, sampleY));
			}
			else
				DrawPoint(col, y, ' ', BG_DARK_GRAY);
		}
		else
		{
			DrawPoint(col, y, ' ', BG_DARK_GRAY);
		}
	}
}

void EngineFPS::DrawDynamicObjects()
{
	for (auto &decoration : decorations)
	{
		float objectAngle, distance;
		if (ObjectWithinFoV(player->GetX(), player->GetY(), player->GetAngle(), decoration->GetX(), decoration->GetY(), objectAngle, distance))
		{
			DrawObject2D(decoration->texture, objectAngle, distance);
		}
	}
}

void EngineFPS::DrawItems()
{
	for (auto &item : items)
	{
		float objectAngle, distance;
		if (item != nullptr && ObjectWithinFoV(player->GetX(), player->GetY(), player->GetAngle(), item->GetX(), item->GetY(), objectAngle, distance))
		{
			DrawObject2D(item->texture, objectAngle, distance);
		}
	}
}

void EngineFPS::DrawDecorations()
{
	for (auto &object : dynamicObjects)
	{
		float objectAngle, distance;
		if (object->texture != nullptr && ObjectWithinFoV(player->GetX(), player->GetY(), player->GetAngle(), object->GetX(), object->GetY(), objectAngle, distance))
		{
			DrawObject2D(object->texture, objectAngle, distance);
		}
	}
}

void EngineFPS::ResetDepthBuffer()
{
	for (int i = 0; i < GetScreenWidth() * GetScreenHeight(); i++)
		depthBuffer[i] = FLT_MAX;
}

void EngineFPS::DrawMap()
{
	for (float i = 0.0f; i < mapWidth; i++)
	{
		for (float j = 0.0f; j < mapHeight; j++)
		{
			if (GetWall(i, j) != nullptr)
				DrawPoint((int)i, (int)j, ' ', BG_YELLOW);
			else if (GetDynamicObject(i, j) != nullptr)
				DrawPoint((int)i, (int)j, ' ', BG_RED);
			else if (GetItem(i, j) != nullptr)
				DrawPoint((int)i, (int)j, ' ', BG_PINK);
			else if (GetDecoration(i, j) != nullptr)
				DrawPoint((int)i, (int)j, ' ', BG_CYAN);
			else
				DrawPoint((int)i, (int)j, ' ', BG_BLACK);
		}
	}
	DrawPoint((int)player->GetX(), (int)player->GetY(), ' ', BG_GREEN);
}

void EngineFPS::DrawPlayerWeapon()
{
	if (player->HasWeapon())
	{
		for (int x = 0; x < weaponWidth; x++)
		{
			for (int y = 0; y < weaponHeight; y++)
			{
				float sampleX = (float)x / (float)weaponWidth;
				float sampleY = (float)y / (float)weaponHeight;

				short symbol = player->GetWeaponSprite()->SampleCharacter(sampleX, sampleY);
				short color = player->GetWeaponSprite()->SampleColor(sampleX, sampleY);

				if (color != BG_DARK_PINK)
				{
					int gunX = (int)((GetScreenWidth() * 0.5f) - (weaponWidth * 0.5f) + x);
					int gunY = (int)(GetScreenHeight() - weaponHeight + y);

					DrawPoint(gunX, gunY, symbol, color);
				}
			}
		}
	}
}

void EngineFPS::DisplayPlayerStats()
{
	//Get player stats
	int hp = player->GetHealth();
	int maxhp = player->GetMaxHealth();

	int ammo = 0;
	int capacity = 0;
	if (player->HasWeapon())
	{
		ammo = player->GetWeaponAmmo();
		capacity = Weapons::CAPACITY;
	}

	int score = player->GetScore();

	//Display player stats
	wchar_t title[256];
	swprintf_s(title, 256, L"First Person Shooter - Health: %d / %d - Ammo: %d / %d - Score: %d", hp, maxhp, ammo, capacity, score);
	SetApplicationTitle(title);
}

int EngineFPS::GetMapWidth() const
{
	return mapWidth;
}

int EngineFPS::GetMapHeight() const
{
	return mapHeight;
}

Sprite* EngineFPS::GetSprite(std::string spriteName) const
{
	return assetManager.GetSprite(spriteName);
}

Weapon* EngineFPS::CreateWeapon(short weaponID, Mob* parent)
{
	return Factory::MakeWeapon(this, weaponID, parent);
}

void EngineFPS::ParseMap()
{
	for (int x = 0; x < mapWidth; x++)
	{
		for (int y = 0; y < mapHeight; y++)
		{
			wchar_t type = map[mapWidth * y + x];

			if (type == '.')
			{
				continue;
			}
			if (isdigit(type))
			{
				AddDynamicObject(x, y, type);
			}
			else if (islower(type))
			{
				AddDecoration(x, y, type);
			}
			else if (isupper(type))
			{
				AddItem(x, y, type);
			}
			else
			{
				AddWall(x, y, type);
			}
		}
	}
}

void EngineFPS::AddWall(int x, int y, wchar_t type)
{
	Wall* wall = Factory::MakeWall(this, x, y, type);
	if (wall != nullptr)
		walls[mapWidth * y + x] = wall;
}

void EngineFPS::AddItem(int x, int y, wchar_t type)
{
	Item* item = Factory::MakeItem(this, x, y, type);
	if (item != nullptr)
		items[mapWidth * y + x] = item;
}

void EngineFPS::AddDecoration(int x, int y, wchar_t type)
{
	Decoration* decor = Factory::MakeDecoration(this, x, y, type);
	if (decor != nullptr)
		decorations.push_back(decor);
}

void EngineFPS::AddDynamicObject(int x, int y, wchar_t type)
{
	DynamicObject* object = Factory::MakeDynamicObject(this, x, y, type);
	if (object != nullptr)
	{
		dynamicObjects.push_back(object);

		if (type == '0')
			player = (Player*)object;
	}
}

Wall* EngineFPS::GetWall(float x, float y) const
{
	return walls[mapWidth * (int)y + (int)x];
}

Item* EngineFPS::GetItem(float x, float y) const
{
	return items[mapWidth * (int)y + (int)x];
}

Decoration* EngineFPS::GetDecoration(float x, float y) const
{
	for (auto &decor : decorations)
	{
		if (ObjectsCollide(decor->GetX(), decor->GetY(), x, y))
			return decor;
	}
	return nullptr;
}

DynamicObject* EngineFPS::GetDynamicObject(float x, float y) const
{
	for (auto &object : dynamicObjects)
	{
		if (ObjectsCollide(object->GetX(), object->GetY(), x, y))
			return object;
	}
	return nullptr;
}

void EngineFPS::PlayAudio(std::string audioName, bool loop)
{
	PlayAudioClip(audio[audioName], loop);
}

bool EngineFPS::IsObstacle(float x, float y, GameObject* ignored) const
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

bool EngineFPS::ObjectsCollide(float x0, float y0, float x1, float y1) const
{
	return ((int)x0 == (int)x1 && (int)y0 == (int)y1);
}

bool EngineFPS::CastRay(float x, float y, float angle, float &hitX, float &hitY, float &distance, bool againstWalls, bool againstDynamicObjects, GameObject *ignored) const
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
			if (againstWalls)
			{
				Wall *wall = GetWall((float)rayX, (float)rayY);
				if (wall != nullptr && wall != ignored)
				{
					hitX = (float)rayX;
					hitY = (float)rayY;
					distance = distanceSoFar;
					return true;
				}
			}

			if (againstDynamicObjects)
			{
				DynamicObject *object = GetDynamicObject((float)rayX, (float)rayY);
				if (object != nullptr && object != ignored)
				{
					hitX = (float)rayX;
					hitY = (float)rayY;
					distance = distanceSoFar;
					return true;
				}
			}
		}
	}

	return false;
}

bool EngineFPS::ObjectWithinFoV(float x0, float y0, float angle, float x1, float y1, float &objectAngle, float &distance) const
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

bool EngineFPS::DynamicObjectVisible(DynamicObject *eye, DynamicObject *object, float &angle, float &distance) const
{
	float objectAngle, objectDistance;
	if (ObjectWithinFoV(eye->GetX(), eye->GetY(), eye->GetAngle(), object->GetX(), object->GetY(), objectAngle, objectDistance))
	{
		float rayX, rayY, rayDistance;
		if (CastRay(eye->GetX(), eye->GetY(), eye->GetAngle() + objectAngle, rayX, rayY, rayDistance, true, true, eye))
		{
			if (GetDynamicObject(rayX, rayY) == object)
			{
				angle = objectAngle;
				distance = objectDistance;
				return true;
			}
		}
	}
	return false;
}

bool EngineFPS::FindMove(GameObject *start, GameObject *finish, float &x, float &y)
{
	//Build the graph
	Node *graph = new Node[mapWidth * mapHeight];

	for (int i = 0; i < GetMapWidth(); i++)
	{
		for (int j = 0; j < GetMapHeight(); j++)
		{
			Node *node = &graph[GetMapWidth() * j + i];

			node->x = i;
			node->y = j;

			if (ObjectsCollide((float)i, (float)j, start->GetX(), start->GetY()) || ObjectsCollide((float)i, (float)j, finish->GetX(), finish->GetY()))
				node->obstacle = false;
			else
				node->obstacle = IsObstacle((float)i, (float)j);

			if (i + 1 < GetMapWidth())
				node->neighbors.push_back(&graph[GetMapWidth() * j + (i + 1)]);

			if (j - 1 >= 0)
				node->neighbors.push_back(&graph[GetMapWidth() * (j - 1) + i]);

			if (i - 1 >= 0)
				node->neighbors.push_back(&graph[GetMapWidth() * j + (i - 1)]);

			if (j + 1 < GetMapHeight())
				node->neighbors.push_back(&graph[GetMapWidth() * (j + 1) + i]);
		}
	}

	//Pass the graph to the pathfinder
	bool result = pathfinder.FindMove(graph, mapWidth, mapHeight, start, finish, x, y);

	delete[] graph;

	return result;
}

void EngineFPS::DrawObject2D(Sprite* spr, float angle, float distance)
{
	//We should just make sure the sprite is actually there
	if (spr == nullptr) return;

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

void EngineFPS::LoadAudio()
{
	auto load = [&](std::string audioName, std::wstring fileName)
	{
		audio[audioName] = LoadAudioClip(fileName);
	};

	//OST
	load("Get Them Before They Get You", L"Audio/OST/Get Them Before They Get You.wav");

	//SFX
	load("Health", L"Audio/SFX/Health.wav");
	load("Pistol", L"Audio/SFX/Pistol.wav");
	load("Machine Gun", L"Audio/SFX/Machine Gun.wav");
	load("Ammo", L"Audio/SFX/Ammo.wav");
	load("Door", L"Audio/SFX/Door.wav");
	load("Achtung", L"Audio/SFX/Achtung.wav");
	load("Enemy Pain", L"Audio/SFX/Enemy Pain.wav");
	load("Enemy Death 1", L"Audio/SFX/Death 1.wav");
	load("Enemy Death 2", L"Audio/SFX/Death 2.wav");
	load("Player Pain 1", L"Audio/SFX/Player Pain 1.wav");
	load("Player Pain 2", L"Audio/SFX/Player Pain 2.wav");
	load("Player Death", L"Audio/SFX/Player Dies.wav");
	load("Pickup", L"Audio/SFX/Pickup.wav");
	load("Secret Entrance", L"Audio/SFX/Secret Entrance.wav");
}