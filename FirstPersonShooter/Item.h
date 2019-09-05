#pragma once

#include "GameObject.h"

class Player;

class Item : public GameObject
{
public:
	Item(EngineFPS* engine);
	virtual ~Item();

	bool IsRemoved() const;

	virtual void OnUse(Player& player) = 0;

protected:
	bool removed = false;
};