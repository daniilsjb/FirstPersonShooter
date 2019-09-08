#pragma once

#include "Wall.h"

class ExitDoor : public Wall
{
public:
	ExitDoor(EngineFPS* engine, float x, float y, Sprite* spr);

	void OnInteract() override;
};
