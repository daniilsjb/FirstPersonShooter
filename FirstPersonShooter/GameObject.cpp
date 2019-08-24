#include "GameObject.h"
#include "FirstPersonShooter.h"

GameObject::GameObject(FirstPersonShooter *engine) : engine(engine) {}

Guard::Guard(FirstPersonShooter *engine) : GameObject(engine)
{
	sprite = engine->sprites["guard front"];
}

void Guard::OnUpdate(float elapsedTime)
{

}

void Guard::OnSpriteUpdate(float elapsedTime)
{

}

void Guard::OnInteract()
{
	removed = true;
}