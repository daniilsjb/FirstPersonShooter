#pragma once

class Sprite;
class FirstPersonShooter;

struct GameObject
{
	FirstPersonShooter *engine;

	float x, y;
	Sprite *sprite = nullptr;

	bool removed = false;

	GameObject(FirstPersonShooter *engine);

	virtual void OnUpdate(float elapsedTime) {};

	virtual void OnSpriteUpdate(float elapsedTime) {};

	virtual void OnInteract() {};
};

struct Guard : public GameObject
{
	Guard(FirstPersonShooter *engine);

	void OnUpdate(float elapsedTime) override;

	void OnSpriteUpdate(float elapsedTime) override;

	void OnInteract() override;

};