#include "Decoration.h"
#include "EngineFPS.h"

Decoration::~Decoration() {}

Decoration::Decoration(EngineFPS* engine, float x, float y, Sprite* spr) : GameObject(engine, x, y)
{
	texture = spr;
}