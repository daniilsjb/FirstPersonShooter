#include "Decoration.h"
#include "EngineFPS.h"

Decoration::~Decoration() {}

Decoration::Decoration(EngineFPS *engine, Sprite *spr) : GameObject(engine)
{
	texture = spr;
}