#include "Item.h"

Item::Item(EngineFPS* engine) : GameObject(engine) {}

Item::~Item() {}

bool Item::IsRemoved() const
{
	return removed;
}