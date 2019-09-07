#pragma once

#include "GameObject.h"

class Decoration : public GameObject
{
public:
	Decoration(EngineFPS* engine, float x, float y, Sprite* spr);
	virtual ~Decoration();
};