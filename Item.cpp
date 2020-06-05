#include "Item.h"

Item::Item(EngineFPS* engine, float x, float y) : GameObject(engine, x, y) {}

Item::~Item() {}

bool Item::IsRemoved() const
{
	return removed;
}