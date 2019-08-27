#pragma once

#include "GameObject.h"

struct Decoration : GameObject
{
	Decoration(EngineFPS *engine, Sprite *texture);
	virtual ~Decoration();
};