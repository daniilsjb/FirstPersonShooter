#pragma once

#include "GameObject.h"

class Wall : public GameObject
{
public:
	Wall(EngineFPS *engine, Sprite* texture);
	virtual ~Wall();

	bool removed = false;

	virtual void OnInteract();
};

class Door : public Wall
{
public:
	Door(EngineFPS *engine, Sprite* texture);

	void OnInteract() override;
};