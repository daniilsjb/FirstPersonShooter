#pragma once

#include "ConsoleGameEngine.h"
#include "Pathfinder.h"

#include <map>

constexpr int WEAPON_COUNT = 2;

class GameObject;
class Wall;
class Item;
class Decoration;
class DynamicObject;
class Player;

class EngineFPS : public ConsoleGameEngine
{
public:
	Player *player = nullptr;

	std::list<DynamicObject*> dynamicObjects;
	std::vector<Wall*> walls;
	std::vector<Item*> items;
	std::list<Decoration*> decorations;

	bool OnStart() override;
	bool OnUpdate(float elapsedTime) override;
	bool OnDestroy() override;

	bool CastRay(float x, float y, float angle, float &hitX, float &hitY, float &distance, bool againstWalls, bool againstDynamicObjects, GameObject *ignored = nullptr);
	bool ObjectsCollide(float x0, float y0, float x1, float y1);
	bool IsObstacle(float x, float y, GameObject* ignored = nullptr);

	bool ObjectWithinFoV(float x0, float y0, float angle, float x1, float y1, float &objectAngle, float &distance);
	bool DynamicObjectVisible(DynamicObject *eye, DynamicObject *object);

	bool FindMove(GameObject *start, GameObject *finish, float &x, float &y);
	void DrawObject2D(Sprite* spr, float angle, float distance);

	Wall* GetWall(float x, float y);
	Item* GetItem(float x, float y);
	Decoration* GetDecoration(float x, float y);
	DynamicObject* GetDynamicObject(float x, float y);

	int GetMapWidth();
	int GetMapHeight();

	Sprite* GetSprite(std::string spriteName);
	void PlayAudio(std::string audioName, bool loop = false);

private:
	Pathfinder pathfinder;

	float FoV = 3.14159f / 4.0f;
	float depth = 20.0f;

	float *depthBuffer = nullptr;

	int weaponWidth;
	int weaponHeight;

	int mapWidth = 16;
	int mapHeight = 16;

	std::wstring map;

	void ParseMap();

	std::map<std::string, Sprite*> sprites;
	std::map<std::string, int> audio;

	void LoadAudio();
	void LoadSprites();
};