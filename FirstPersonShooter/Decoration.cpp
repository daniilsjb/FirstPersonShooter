#include "Decoration.h"
#include "EngineFPS.h"

Decoration::~Decoration() {}

Decoration::Decoration(EngineFPS *engine, Sprite *texture) : GameObject(engine)
{
	this->texture = texture;
}