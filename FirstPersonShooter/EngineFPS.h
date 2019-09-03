#pragma once

#include "ConsoleGameEngine.h"
#include "Pathfinder.h"

#include <map>

class GameObject;
class Wall;
class Item;
class Decoration;
class DynamicObject;
class Player;
class Weapon;
class Mob;

class EngineFPS : public ConsoleGameEngine
{
public:
	Player *player = nullptr;

	bool OnStart() override;
	bool OnUpdate(float elapsedTime) override;
	bool OnDestroy() override;

	bool CastRay(float x, float y, float angle, float &hitX, float &hitY, float &distance, bool againstWalls, bool againstDynamicObjects, GameObject *ignored = nullptr) const;
	bool ObjectsCollide(float x0, float y0, float x1, float y1) const;
	bool IsObstacle(float x, float y, GameObject* ignored = nullptr) const;

	bool ObjectWithinFoV(float x0, float y0, float angle, float x1, float y1, float &objectAngle, float &distance) const;
	bool DynamicObjectVisible(DynamicObject *eye, DynamicObject *object) const;

	bool FindMove(GameObject *start, GameObject *finish, float &x, float &y);
	void DrawObject2D(Sprite* spr, float angle, float distance);

	Weapon* CreateWeapon(short weaponID, Mob* parent);

	void AddWall(int x, int y, wchar_t type);
	void AddItem(int x, int y, wchar_t type);
	void AddDecoration(int x, int y, wchar_t type);
	void AddDynamicObject(int x, int y, wchar_t type);

	Wall* GetWall(float x, float y) const;
	Item* GetItem(float x, float y) const;
	Decoration* GetDecoration(float x, float y) const;
	DynamicObject* GetDynamicObject(float x, float y) const;

	int GetMapWidth() const;
	int GetMapHeight() const;

	Sprite* GetSprite(std::string spriteName) const;
	void PlayAudio(std::string audioName, bool loop = false);

private:
	Pathfinder pathfinder;

	float FoV = 3.14159f / 4.0f;
	float depth = 20.0f;

	float *depthBuffer = nullptr;

	int weaponWidth;
	int weaponHeight;

	std::list<DynamicObject*> dynamicObjects;
	std::vector<Wall*> walls;
	std::vector<Item*> items;
	std::list<Decoration*> decorations;

	int mapWidth = 48;
	int mapHeight = 48;

	std::wstring map;

	void ParseMap();

	std::map<std::string, Sprite*> sprites;
	std::map<std::string, int> audio;

	float gameOverTimer = 0.0f;

	void LoadAudio();
	void LoadSprites();
};