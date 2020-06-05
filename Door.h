#pragma once

#include "Wall.h"

class Door : public Wall
{
public:
	Door(EngineFPS* engine, float x, float y, Sprite* spr);

	void OnInteract() override;
};