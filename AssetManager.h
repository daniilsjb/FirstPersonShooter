#pragma once

#include <map>

class Sprite;

class AssetManager
{
public:
	AssetManager();
	~AssetManager();

	void LoadSprites();
	Sprite* GetSprite(std::string spriteName) const;

private:
	std::map<std::string, Sprite*> sprites;
};