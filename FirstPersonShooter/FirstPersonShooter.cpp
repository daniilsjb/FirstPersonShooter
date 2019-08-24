#include "FirstPersonShooter.h"

bool FirstPersonShooter::OnStart()
{
	SetApplicationTitle(L"First Person Shooter");

	map.append(L"#$@#$@#$@#$@#$@#");
	map.append(L"#..............#");
	map.append(L"#......P.......#");
	map.append(L"#..............#");
	map.append(L"#........G.....#");
	map.append(L"#...F..........#");
	map.append(L"#...F..........#");
	map.append(L"#.......$......#");
	map.append(L"#..............#");
	map.append(L"#..............#");
	map.append(L"#..J......F....#");
	map.append(L"#..............#");
	map.append(L"#...T..........#");
	map.append(L"#..........F...#");
	map.append(L"#..............#");
	map.append(L"################");

	LoadSprites();

	for (int x = 0; x < mapWidth; x++)
	{
		for (int y = 0; y < mapHeight; y++)
		{
			wchar_t c = map[mapWidth * y + x];

			if (c == 'P')
			{
				playerX = (float)x;
				playerY = (float)y;

				continue;
			}

			if (IsWall(c))
				continue;

			GameObject *object = nullptr;

			switch (c)
			{
				case 'F':
				{
					object = new GameObject(this);
					object->sprite = sprites["flag"];
					break;
				}
				case 'J':
				{
					object = new GameObject(this);
					object->sprite = sprites["jug"];
					break;
				}
				case 'T':
				{
					object = new GameObject(this);
					object->sprite = sprites["tree"];
					break;
				}
				case 'G':
				{
					object = new Guard(this);
					break;
				}
			}

			if (object != nullptr)
			{
				object->x = (float)x + 0.5f;
				object->y = (float)y + 0.5f;
				objects.push_back(object);
			}
		}
	}

	depthBuffer = new float[GetScreenWidth() * GetScreenHeight()];

	return true;
}

bool FirstPersonShooter::OnUpdate(float elapsedTime)
{
	if (GetKey('W').held)
	{
		playerX += cosf(playerA) * speed * elapsedTime;
		playerY += sinf(playerA) * speed * elapsedTime;
		if (IsWall(map[mapWidth * (int)playerY + (int)playerX])) {
			playerX -= cosf(playerA) * speed * elapsedTime;
			playerY -= sinf(playerA) * speed * elapsedTime;
		}
	}

	if (GetKey('S').held)
	{
		playerX -= cosf(playerA) * speed * elapsedTime;
		playerY -= sinf(playerA) * speed * elapsedTime;
		if (IsWall(map[mapWidth * (int)playerY + (int)playerX])) {
			playerX += cosf(playerA) * speed * elapsedTime;
			playerY += sinf(playerA) * speed * elapsedTime;
		}
	}

	if (GetKey('Q').held)
	{
		playerX += sinf(playerA) * speed * elapsedTime;
		playerY -= cosf(playerA) * speed * elapsedTime;
		if (IsWall(map[mapWidth * (int)playerY + (int)playerX])) {
			playerX -= sinf(playerA) * speed * elapsedTime;
			playerY += cosf(playerA) * speed * elapsedTime;
		}
	}

	if (GetKey('E').held)
	{
		playerX -= sinf(playerA) * speed * elapsedTime;
		playerY += cosf(playerA) * speed * elapsedTime;
		if (IsWall(map[mapWidth * (int)playerY + (int)playerX])) {
			playerX += sinf(playerA) * speed * elapsedTime;
			playerY -= cosf(playerA) * speed * elapsedTime;
		}
	}

	if (GetKey('A').held)
		playerA -= (speed * 0.75f) * elapsedTime;

	if (GetKey('D').held)
		playerA += (speed * 0.75f) * elapsedTime;

	if (GetKey(VK_LBUTTON).released)
	{
		//Shoot by casting a ray forward from the player and checking if it hits an enemy
		float step = 0.01f;
		float distance = 0.0f;

		bool hitEnemy = false;

		float eyeX = cosf(playerA);
		float eyeY = sinf(playerA);

		while (!hitEnemy && distance < depth)
		{
			distance += step;

			int rayX = (int)(playerX + eyeX * distance);
			int rayY = (int)(playerY + eyeY * distance);

			if (rayX < 0 || rayX >= mapWidth || rayY < 0 || rayY >= mapHeight)
			{
				distance = depth;
			}
			else
			{
				if (map[mapWidth * rayY + rayX] == 'G')
				{
					auto guard = std::find_if(objects.begin(), objects.end(), [rayX, rayY](const GameObject* obj)
					{
						return (int)obj->x == rayX && (int)obj->y == rayY;
					});

					(*guard)->OnInteract();
					hitEnemy = true;
				}
			}
		}
	}

	for (int i = 0; i < GetScreenWidth() * GetScreenHeight(); i++)
		depthBuffer[i] = FLT_MAX;

	for (int x = 0; x < GetScreenWidth(); x++)
	{
		float rayOffset = (float)x / (float)GetScreenWidth();
		float rayAngle = (playerA - FoV * 0.5f) + rayOffset * FoV;

		float step = 0.01f;
		float distanceToWall = 0.0f;

		bool hitWall = false;

		float eyeX = cosf(rayAngle);
		float eyeY = sinf(rayAngle);

		float sampleX = 0.0f;

		wchar_t wall;

		while (!hitWall && distanceToWall < depth)
		{
			distanceToWall += step;

			int rayX = (int)(playerX + eyeX * distanceToWall);
			int rayY = (int)(playerY + eyeY * distanceToWall);

			if (rayX < 0 || rayX >= mapWidth || rayY < 0 || rayY >= mapHeight)
			{
				hitWall = true;
				distanceToWall = depth;
			}
			else
			{
				if (IsWall(map[mapWidth * rayY + rayX]))
				{
					hitWall = true;

					wall = map[mapWidth * rayY + rayX];

					float blockMidX = (float)rayX + 0.5f;
					float blockMidY = (float)rayY + 0.5f;

					float testPointX = playerX + (eyeX * distanceToWall);
					float testPointY = playerY + (eyeY * distanceToWall);

					float testAngle = atan2f(testPointY - blockMidY, testPointX - blockMidX);

					//Right
					if (testAngle >= -3.14159f * 0.25f && testAngle < 3.14159f * 0.25f)
						sampleX = 1.0f - (testPointY - (int)rayY);
					//Top
					else if (testAngle >= 3.14159f * 0.25f && testAngle < 3.14159f * 0.75f)
						sampleX = testPointX - (int)rayX;
					//Left
					else if (testAngle >= 3.14159f * 0.75f || testAngle < -3.14159f * 0.75f)
						sampleX = testPointY - (int)rayY;
					//Bottom
					else if (testAngle >= -3.14159f * 0.75f && testAngle < -3.14159f * 0.25f)
						sampleX = 1.0f - (testPointX - (int)rayX);
				}
			}
		}

		int ceiling = (int)((float)GetScreenHeight() * 0.5f - (float)GetScreenHeight() / (float)distanceToWall);
		int floor = GetScreenHeight() - ceiling;

		for (int y = 0; y < GetScreenHeight(); y++)
		{
			if (y <= ceiling)
			{
				DrawPoint(x, y, ' ', BG_DARK_GRAY);
			}
			else if (y > ceiling && y <= floor)
			{
				depthBuffer[GetScreenWidth() * y + x] = distanceToWall;

				if (distanceToWall < depth)
				{
					Sprite* texture = GetWallTexture(wall);
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

	objects.remove_if([](const GameObject* obj) { return obj->removed; });

	for (auto &object : objects)
	{
		float objDirX = object->x - playerX;
		float objDirY = object->y - playerY;
		float distance = sqrtf(objDirX*objDirX + objDirY * objDirY);

		float objectAngle = atan2f(objDirY, objDirX) - playerA;

		while (objectAngle < -3.14159f)
			objectAngle += 2.0f * 3.14159f;

		while (objectAngle > 3.14159f)
			objectAngle -= 2.0f * 3.14159f;

		bool withinFoV = fabs(objectAngle) < (FoV * 0.5f);

		if (withinFoV && distance >= 0.5f && distance < depth)
		{
			float objectTop = ((float)GetScreenHeight() * 0.5f) - ((float)GetScreenHeight() / (float)distance);
			float objectBottom = (float)GetScreenHeight() - objectTop;
			float objectHeight = objectBottom - objectTop;

			float objectAspectRatio = (float)object->sprite->GetHeight() / (float)object->sprite->GetWidth();
			float objectWidth = objectHeight / objectAspectRatio;

			float objectMid = (0.5f * (objectAngle / (FoV * 0.5f)) + 0.5f) * (float)GetScreenWidth();

			for (float x = 0.0f; x < objectWidth; x++)
			{
				for (float y = 0.0f; y < objectHeight; y++)
				{
					float sampleX = x / objectWidth;
					float sampleY = y / objectHeight;

					short symbol = object->sprite->SampleCharacter(sampleX, sampleY);
					short color = object->sprite->SampleColor(sampleX, sampleY);

					int objectColumn = (int)(objectMid + x - (objectWidth * 0.5f));
					int objectRow = (int)(objectTop + y);

					if (objectColumn >= 0 && objectColumn < GetScreenWidth() && objectRow >= 0 && objectRow < GetScreenHeight())
					{
						if (color != BG_DARK_PINK && depthBuffer[GetScreenWidth() * objectRow + objectColumn] >= distance)
						{
							DrawPoint(objectColumn, objectRow, symbol, color);
							depthBuffer[GetScreenWidth() * objectRow + objectColumn] = distance;
						}
					}
				}
			}
		}
	}

	for (int i = 0; i < mapWidth; i++)
	{
		for (int j = 0; j < mapHeight; j++)
		{
			DrawPoint(i, j, map[mapWidth * j + i], FG_GRAY);
		}
	}
	DrawPoint((int)playerX, (int)playerY, ' ', BG_GREEN);

	return true;
}

bool FirstPersonShooter::OnDestroy()
{
	for (auto &spr : sprites)
		delete spr.second;

	for (auto &obj : objects)
		delete obj;

	return true;
}

void FirstPersonShooter::LoadSprites()
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
	load("flag", L"Sprites/flag.spr");
	load("jug", L"Sprites/jug.spr");
	load("tree", L"Sprites/tree.spr");
	load("guard back", L"Sprites/guard_back.spr");
	load("guard front", L"Sprites/guard_front.spr");
	load("guard reload", L"Sprites/guard_reload.spr");
	load("guard fire", L"Sprites/guard_fire.spr");
}

bool FirstPersonShooter::IsWall(wchar_t character)
{
		return (character == '#' || character == '$' || character == '@');
}

Sprite* FirstPersonShooter::GetWallTexture(wchar_t wall)
{
	switch (wall)
	{
		case '#':
			return sprites["stone wall"];
		case '$':
			return sprites["stone wall eagle"];
		case '@':
			return sprites["stone wall flag"];
		default:
			return sprites["stone wall"];
	}
}

int main()
{
	FirstPersonShooter game;
	if (game.ConstructScreen(320, 240, 4, 4))
		game.Start();

	return 0;

}