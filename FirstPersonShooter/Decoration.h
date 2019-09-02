#pragma once

#include "GameObject.h"

class Decoration : public GameObject
{
public:
	Decoration(EngineFPS *engine, Sprite *texture);
	virtual ~Decoration();
};