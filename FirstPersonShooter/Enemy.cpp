#include "Enemy.h"

Enemy::Enemy(EngineFPS *engine) : Mob(engine)
{
	directionSprites.resize(DIRECTIONS_COUNT, nullptr);
}

Sprite* Enemy::ChooseDirectionSprite(DynamicObject *relativeObject)
{
	//Find direction vector of the enemy
	float eyeX = cosf(angle);
	float eyeY = sinf(angle);

	//Find direction vector of the relative object
	float objEyeX = cosf(relativeObject->angle);
	float objEyeY = sinf(relativeObject->angle);

	//Both vectors are normalized, dot product is a value between -1 to 1 that indicates the similarity between their directions
	float dot = (eyeX * objEyeX) + (eyeY * objEyeY);

	//We also need to know the side of the relative object. We can just rotate the enemy by half-pi radians and do another dot product
	//to find the side
	float rotatedEyeX = cosf(angle + 3.14159f * 0.5f);
	float rotatedEyeY = sinf(angle + 3.14159f * 0.5f);

	//Find vector from enemy towards relative object
	float dirX = relativeObject->x - x;
	float dirY = relativeObject->y - y;

	//Normalize the vector
	float magnitude = sqrtf(dirX * dirX + dirY * dirY);
	float magInv = 1.0f / magnitude;

	dirX *= magInv;
	dirY *= magInv;

	//Find the side (positive is right, negative is left)
	float side = (rotatedEyeX * dirX) + (rotatedEyeY * dirY);

	//The less the dot product is, the more both objects look towards each other
	if (dot < -0.5f)
		return directionSprites[FRONT];
	else if (dot >= -0.5f && dot < 0.5f)
	{
		if (side >= 0.0f)
			return directionSprites[RIGHT];
		else
			return directionSprites[LEFT];
	}
	else
		return directionSprites[BACK];
}

Guard::Guard(EngineFPS *engine) : Enemy(engine)
{
	directionSprites[BACK] = engine->sprites["guard back"];
	directionSprites[RIGHT] = engine->sprites["guard right"];
	directionSprites[LEFT] = engine->sprites["guard front"];
	directionSprites[FRONT] = engine->sprites["guard left"];

	currentHealth = maxHealth = 50;

	angle = 3.14159f;
}

void Guard::OnUpdate(float elapsedTime)
{
	//SUPER AI

	//UPDATE LOOK SMILE
	texture = ChooseDirectionSprite((DynamicObject*)engine->player);
}

void Guard::OnHit(int damage)
{
	Damage(damage);
	if (currentHealth <= 0)
		removed = true;
}