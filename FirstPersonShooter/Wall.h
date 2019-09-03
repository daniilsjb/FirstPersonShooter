#pragma once

#include "GameObject.h"

class Wall : public GameObject
{
public:
	Wall(EngineFPS *engine, Sprite* texture);
	virtual ~Wall();

	bool IsRemoved() const;
	
	virtual void OnInteract();

protected:
	bool removed = false;
};