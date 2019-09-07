#pragma once

#include "GameObject.h"

class Player;

class Item : public GameObject
{
public:
	Item(EngineFPS* engine, float x, float y);
	virtual ~Item();

	bool IsRemoved() const;

	virtual void OnUse(Player& player) = 0;

protected:
	bool removed = false;
};