#pragma once

#include "ConsoleGameEngine.h"
#include "GameObject.h"

#include <map>

class FirstPersonShooter : public ConsoleGameEngine
{
public:
	float playerX = 8.0f;
	float playerY = 8.0f;

	float playerA = 3.14159f / 2.0f;

	float speed = 5.0f;

	float FoV = 3.14159f / 4.0f;
	float depth = 20.0f;

	int mapWidth = 16;
	int mapHeight = 16;

	std::wstring map;

	std::map<std::string, Sprite*> sprites;

	std::list<GameObject*> objects;

	float *depthBuffer = nullptr;

	bool OnStart() override;
	bool OnUpdate(float elapsedTime) override;
	bool OnDestroy() override;

	void LoadSprites();
	bool IsWall(wchar_t character);
	Sprite* GetWallTexture(wchar_t wall);
};