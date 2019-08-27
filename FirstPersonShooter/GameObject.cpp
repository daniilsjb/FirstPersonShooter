#include "GameObject.h"
#include "EngineFPS.h"

GameObject::~GameObject() {}

GameObject::GameObject(EngineFPS *engine) : engine(engine) {}