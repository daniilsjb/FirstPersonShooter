#pragma once

#include "Wall.h"

class SecretEntrance : public Wall
{
public:
	SecretEntrance(EngineFPS* engine, float x, float y, Sprite* spr);

	void OnInteract() override;
};