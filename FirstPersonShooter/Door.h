#pragma once

#include "Wall.h"

class Door : public Wall
{
public:
	Door(EngineFPS *engine, Sprite* texture);

	void OnInteract() override;
};