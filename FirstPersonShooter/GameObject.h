#pragma once

#include "ConsoleGameEngine.h"

class FirstPersonShooter;

struct GameObject
{
	FirstPersonShooter *engine;

	float x, y;
	ConsoleGameEngine::Sprite *sprite = nullptr;

	bool removed = false;

	GameObject(FirstPersonShooter *engine) : engine(engine) {}

	virtual void OnUpdate(float elapsedTime) {}

	virtual void OnSpriteUpdate(float elapsedTime) {}

};