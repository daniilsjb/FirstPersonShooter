#pragma once

#include "GameObject.h"

class Wall : public GameObject
{
public:
	Wall(EngineFPS* engine, Sprite* spr);
	virtual ~Wall();

	bool IsRemoved() const;
	
	virtual void OnInteract();

protected:
	bool removed = false;
};