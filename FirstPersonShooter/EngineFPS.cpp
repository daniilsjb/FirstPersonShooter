#include "EngineFPS.h"
#include "Wall.h"
#include "Decoration.h"
#include "DynamicObject.h"
#include "Mob.h"
#include "Item.h"
#include "Player.h"
#include "Enemy.h"
#include "Node.h"

bool EngineFPS::OnStart()
{
	SetApplicationTitle(L"First Person Shooter");

	LoadAudio();
	LoadSprites();

	//Walls and Items, by their nature, are static. We want to be able to access them based on some arbitrary map coordinates,
	//as it may save performance a lot in exchange for a bit of memory.
	walls.resize(mapWidth * mapHeight, nullptr);
	items.resize(mapWidth * mapHeight, nullptr);

	//Special Characters - Walls
	//Lowercase Letters  - Decorations
	//Uppercase Letters  - Items
	//Numbers            - Dynamic Objects
	map.append(L"##########@#############$#####$####@#####$######");
	map.append(L"#...P.#......#.......#....................f...t#");
	map.append(L"#..0..-......#....U..#.j...#..l..#....j........#");
	map.append(L"#f....#......-..Sl...$.....#..1..#..........K..#");
	map.append(L"###$#@#......#.......#.....##@##$#..l.....l....#");
	map.append(L"#............###@#####......................M..#");
	map.append(L"#.f......l...#.S..M..#.t...l......1..1......M..#");
	map.append(L"#............-...l...#.........................#");
	map.append(L"###$###......#..U....$.....l....####$###$##H##@#");
	map.append(L"#.CCC.#..l...###$##@##..........#....#.........#");
	map.append(L"#.RUU.#......#.......#.....1....#....#..M...M..#");
	map.append(L"#.USS.#......-..1l.R.#..........#....#.........#");
	map.append(L"###?###..l...#.......#...##@##########.f.......#");
	map.append(L"#............#########...#.M.....K...#.....1...#");
	map.append(L"#.U......1...........#...#...AA..BB..#.........#");
	map.append(L"#...$....l..l.1L..l..H...#...........#.........#");
	map.append(L"#....................#...#@#####?###@#......l..#");
	map.append(L"###@####$#####@#######.........................#");
	map.append(L"#...........................l....l..........l..#");
	map.append(L"#..f...................................1.......#");
	map.append(L"#............###@#####$#####$#####@####@###....#");
	map.append(L"#...#........#............................#....#");
	map.append(L"#...#.1..l...$............................#....#");
	map.append(L"#...#........#............................#....#");
	map.append(L"#...@###$#####......###@####@####$###.....#....#");
	map.append(L"#...#...............#...............#.....#....#");
	map.append(L"#...$...............#..t......1...t.###@###....#");
	map.append(L"#...######@######@###..........................#");
	map.append(L"#..............................................#");
	map.append(L"#................1.............................#");
	map.append(L"#######-###@####-##$#####-#########$##.........#");
	map.append(L"#..........#.S.....U.#........#......#....l...f#");
	map.append(L"#.S..l..1..#.1..l....#...l....#...l..#.........#");
	map.append(L"#.U.......M#.......U.#.K....M.#.1....#.........#");
	map.append(L"######?##$######@########$#####......#....l...f#");
	map.append(L"#..........#.1.......f.......f.......#.........#");
	map.append(L"#..M.UU..M.#.........................#.........#");
	map.append(L"#A..RSSR..A#........K....K...........#....l...f#");
	map.append(L"######@#####...................................#");
	map.append(L"#..............................................#");
	map.append(L"#....................###?###..............l...f#");
	map.append(L"######?####....1.....#S...S#......1............#");
	map.append(L"#A.R...U..#..........#..MM.#...................#");
	map.append(L"#..UU.L.U.#..........#######......1.......l...f#");
	map.append(L"###?###..B#...1................................#");
	map.append(L"#RUUUC#.KC#....................................#");
	map.append(L"#RSSSC#M..#........f...........f..f..f..f..f..j#");
	map.append(L"############@####$####@####$####################");

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
	if (player->GetHealth() <= 0)
	{
		gameOverTimer += elapsedTime;
		if (gameOverTimer >= 3.0f)
			return false;

		return true;
	}

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
		if (wall != nullptr && wall->IsRemoved())
		{
			delete wall;
			wall = nullptr;
		}
	}

	//Same applies to item
	for (auto &item : items)
	{
		if (item != nullptr && item->IsRemoved())
		{
			delete item;
			item = nullptr;
		}
	}

	//Dynamic objects, however, are not that easy to control, so we want to just remove it from the list completely
	dynamicObjects.remove_if([](const DynamicObject* object)
	{
		if (object->IsRemoved())
			delete object;

		return object->IsRemoved();
	});

	//Reset depth buffer
	for (int i = 0; i < GetScreenWidth() * GetScreenHeight(); i++)
		depthBuffer[i] = FLT_MAX;

	//Draw the walls, floor, and ceiling
	for (int x = 0; x < GetScreenWidth(); x++)
	{
		//Calculate ray angle depending on the column we're filling in
		float rayOffset = (float)x / (float)GetScreenWidth();
		float rayAngle = (player->GetAngle() - FoV * 0.5f) + rayOffset * FoV;

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
		if (ObjectWithinFoV(player->x, player->y, player->GetAngle(), decoration->x, decoration->y, objectAngle, distance))
		{
			DrawObject2D(decoration->texture, objectAngle, distance);
		}
	}

	//Draw items
	for (auto &item : items)
	{
		float objectAngle, distance;
		if (item != nullptr && ObjectWithinFoV(player->x, player->y, player->GetAngle(), item->x, item->y, objectAngle, distance))
		{
			DrawObject2D(item->texture, objectAngle, distance);
		}
	}

	//Draw dynamic objects
	for (auto &object : dynamicObjects)
	{
		float objectAngle, distance;
		if (object->texture != nullptr && ObjectWithinFoV(player->x, player->y, player->GetAngle(), object->x, object->y, objectAngle, distance))
		{
			DrawObject2D(object->texture, objectAngle, distance);
		}
	}

	//Draw the weapon in the bottom-middle of the screen
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

	//Draw the map (for debug purposes mainly)
	/*for (float i = 0.0f; i < mapWidth; i++)
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
	DrawPoint((int)player->x, (int)player->y, ' ', BG_GREEN);*/

	//Get player stats
	int hp = player->GetHealth();
	int maxhp = player->GetMaxHealth();

	int ammo = 0;
	int capacity = 0;
	if (player->HasWeapon())
	{
		ammo = player->GetWeaponAmmo();
		capacity = player->GetWeaponCapacity();
	}

	int score = player->GetScore();

	//Display player stats
	wchar_t title[256];
	swprintf_s(title, 256, L"First Person Shooter - Health: %d / %d - Ammo: %d / %d - Score: %d", hp, maxhp, ammo, capacity, score);
	SetApplicationTitle(title);

	return true;
}

bool EngineFPS::OnDestroy()
{
	for (auto &spr : sprites)
	{
		delete spr.second;
		spr.second = nullptr;
	}	

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

int EngineFPS::GetMapWidth() const
{
	return mapWidth;
}

int EngineFPS::GetMapHeight() const
{
	return mapHeight;
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

Weapon* EngineFPS::CreateWeapon(short weaponID, Mob* parent)
{
	switch (weaponID)
	{
		case Weapons::PISTOL:
			return new Pistol(this, parent);
		case Weapons::MACHINE_GUN:
			return new MachineGun(this, parent);
		default:
			return nullptr;
	}
}

void EngineFPS::AddWall(int x, int y, wchar_t type)
{
	if (x < 0 || x >= mapWidth || y < 0 || y >= mapHeight)
		return;

	Wall* wall = nullptr;

	switch (type)
	{
		case '#':
		{
			wall = new Wall(this, sprites["Stone Wall"]);
			break;
		}
		case '$':
		{
			wall = new Wall(this, sprites["Stone Wall Eagle"]);
			break;
		}
		case '@':
		{
			wall = new Wall(this, sprites["Stone Wall Flag"]);
			break;
		}
		case '?':
		{
			wall = new Door(this, sprites["Stone Wall Flag"]);
			break;
		}
		case '-':
		{
			wall = new Door(this, sprites["Metal Door"]);
			break;
		}
	}

	if (wall != nullptr)
	{
		wall->x = x + 0.5f;
		wall->y = y + 0.5f;
		walls[mapWidth * y + x] = wall;
	}
}

void EngineFPS::AddItem(int x, int y, wchar_t type)
{
	if (x < 0 || x >= mapWidth || y < 0 || y >= mapHeight)
		return;

	Item* item = nullptr;

	switch (type)
	{
		//Health
		case 'M':
		{
			item = new Medkit(this, 10, sprites["Item Medpack"]);
			break;
		}
		case 'K':
		{
			item = new Medkit(this, 40, sprites["Item Medkit"]);
			break;
		}

		//Weapons
		case 'P':
		{
			item = new WeaponItem(this, Weapons::PISTOL, sprites["Item Pistol"]);
			break;
		}
		case 'G':
		{
			item = new WeaponItem(this, Weapons::MACHINE_GUN, sprites["Item Machine Gun"]);
			break;
		}

		//Ammo
		case 'A':
		{
			item = new AmmoItem(this, Weapons::PISTOL, 5, sprites["Item Pistol Ammo"]);
			break;
		}
		case 'B':
		{
			item = new AmmoItem(this, Weapons::MACHINE_GUN, 50, sprites["Item Machine Gun Ammo"]);
			break;
		}

		//Score
		case 'S':
		{
			item = new ScoreItem(this, 10, sprites["Item Cross"]);
			break;
		}
		case 'C':
		{
			item = new ScoreItem(this, 35, sprites["Item Chest"]);
			break;
		}
		case 'R':
		{
			item = new ScoreItem(this, 60, sprites["Item Crown"]);
			break;
		}
		case 'U':
		{
			item = new ScoreItem(this, 5, sprites["Item Cup"]);
			break;
		}
	}

	if (item != nullptr)
	{
		item->x = x + 0.5f;
		item->y = y + 0.5f;
		items[mapWidth * y + x] = item;
	}
}

void EngineFPS::AddDecoration(int x, int y, wchar_t type)
{
	if (x < 0 || x >= mapWidth || y < 0 || y >= mapHeight)
		return;

	Decoration* decor = nullptr;

	switch (type)
	{
		case 'f':
		{
			decor = new Decoration(this, sprites["Flag"]);
			break;
		}
		case 'j':
		{
			decor = new Decoration(this, sprites["Jug"]);
			break;
		}
		case 't':
		{
			decor = new Decoration(this, sprites["Tree"]);
			break;
		}
		case 'l':
		{
			decor = new Decoration(this, sprites["Lamp"]);
			break;
		}
	}

	if (decor != nullptr)
	{
		decor->x = x + 0.5f;
		decor->y = y + 0.5f;
		decorations.push_back(decor);
	}
}

void EngineFPS::AddDynamicObject(int x, int y, wchar_t type)
{
	if (x < 0 || x >= mapWidth || y < 0 || y >= mapHeight)
		return;

	DynamicObject* object = nullptr;

	switch (type)
	{
		case '0':
		{
			player = new Player(this);
			object = player;
			break;
		}
		case '1':
		{
			object = new Guard(this);
			break;
		}
	}

	if (object != nullptr)
	{
		object->x = x + 0.5f;
		object->y = y + 0.5f;
		dynamicObjects.push_back(object);
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
		if (ObjectsCollide(decor->x, decor->y, x, y))
			return decor;
	}
	return nullptr;
}

DynamicObject* EngineFPS::GetDynamicObject(float x, float y) const
{
	for (auto &object : dynamicObjects)
	{
		if (ObjectsCollide(object->x, object->y, x, y))
			return object;
	}
	return nullptr;
}

Sprite* EngineFPS::GetSprite(std::string spriteName) const
{
	return sprites.at(spriteName);
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

bool EngineFPS::DynamicObjectVisible(DynamicObject *eye, DynamicObject *object) const
{
	for (int x = 0; x < 4; x++)
	{
		float rayOffset = (float)x / 4;
		float rayAngle = (eye->GetAngle() - FoV * 0.5f) + rayOffset * FoV;

		float rayX, rayY;
		float distance;

		if (CastRay(eye->x, eye->y, rayAngle, rayX, rayY, distance, false, true, eye))
		{
			if (GetDynamicObject(rayX, rayY) == object)
				return true;
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

			if (ObjectsCollide((float)i, (float)j, start->x, start->y) || ObjectsCollide((float)i, (float)j, finish->x, finish->y))
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

void EngineFPS::LoadSprites()
{
	auto load = [&](std::string spriteName, std::wstring fileName)
	{
		Sprite *spr = new Sprite(fileName);
		sprites[spriteName] = spr;
	};

	//Textures
	load("Brick Wall", L"Sprites/Brick Wall.spr");
	load("Stone Wall", L"Sprites/Stone Wall.spr");
	load("Stone Wall Eagle", L"Sprites/Stone Wall Eagle.spr");
	load("Stone Wall Flag", L"Sprites/Stone Wall Flag.spr");
	load("Metal Door", L"Sprites/Metal Door.spr");

	//Decorations
	load("Flag", L"Sprites/Flag.spr");
	load("Jug", L"Sprites/Jug.spr");
	load("Tree", L"Sprites/Tree.spr");
	load("Lamp", L"Sprites/Lamp.spr");

	//Enemies
	load("Guard Back", L"Sprites/Guard Back.spr");
	load("Guard Front", L"Sprites/Guard Front.spr");
	load("Guard Left", L"Sprites/Guard Left.spr");
	load("Guard Right", L"Sprites/Guard Right.spr");
	load("Guard Reload", L"Sprites/Guard Reload.spr");
	load("Guard Fire", L"Sprites/Guard Fire.spr");

	//Weapons
	load("Pistol", L"Sprites/Weapon Pistol.spr");
	load("Pistol Fire", L"Sprites/Weapon Pistol Fire.spr");
	load("Machine Gun", L"Sprites/Weapon Machine Gun.spr");
	load("Machine Gun Fire", L"Sprites/Weapon Machine Gun Fire.spr");

	//Items
	load("Item Pistol", L"Sprites/Item Pistol.spr");
	load("Item Machine Gun", L"Sprites/Item Machine Gun.spr");
	load("Item Pistol Ammo", L"Sprites/Item Pistol Ammo.spr");
	load("Item Machine Gun Ammo", L"Sprites/Item Machine Gun Ammo.spr");
	load("Item Medpack", L"Sprites/Item Medpack.spr");
	load("Item Medkit", L"Sprites/Item Medkit.spr");
	load("Item Chest", L"Sprites/Item Chest.spr");
	load("Item Cross", L"Sprites/Item Cross.spr");
	load("Item Crown", L"Sprites/Item Crown.spr");
	load("Item Cup", L"Sprites/Item Cup.spr");
}