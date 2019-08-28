#pragma once

#include "ConsoleGameEngine.h"
#include <map>

#define WEAPON_COUNT 3

struct GameObject;
struct Wall;
struct Item;
struct Decoration;
struct DynamicObject;
struct Player;

class EngineFPS : public ConsoleGameEngine
{
public:
	float FoV = 3.14159f / 4.0f;
	float depth = 20.0f;

	std::map<std::string, Sprite*> sprites;

	std::list<DynamicObject*> dynamicObjects;
	std::vector<Wall*> walls;
	std::vector<Item*> items;
	std::list<Decoration*> decorations;

	Player *player = nullptr;

	int weaponWidth;
	int weaponHeight;

	float *depthBuffer = nullptr;

	bool OnStart() override;
	bool OnUpdate(float elapsedTime) override;
	bool OnDestroy() override;

	bool CastRay(float x, float y, float angle, float &hitX, float &hitY, float &distance, bool againstWalls, bool againstDynamicObjects, GameObject *ignored = nullptr);
	bool ObjectWithinFoV(float x0, float y0, float angle, float x1, float y1, float &objectAngle, float &distance);

	void DrawObject2D(Sprite* spr, float angle, float distance);

	bool IsObstacle(float x, float y, GameObject* ignored = nullptr);

	Wall* GetWall(float x, float y);
	Item* GetItem(float x, float y);
	Decoration* GetDecoration(float x, float y);
	DynamicObject* GetDynamicObject(float x, float y);

	bool ObjectsCollide(float x0, float y0, float x1, float y1);

	int GetMapWidth();
	int GetMapHeight();

private:
	int mapWidth = 16;
	int mapHeight = 16;

	std::wstring map;

	void ParseMap();
	void LoadSprites();
};