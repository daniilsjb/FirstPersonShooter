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

class Door : public Wall
{
public:
	Door(EngineFPS *engine, Sprite* texture);

	void OnInteract() override;
};