#include "Enemy.h"
#include "EngineFPS.h"
#include "Player.h"
#include "Weapon.h"

Enemy::Enemy(EngineFPS* engine, float x, float y) : Mob(engine, x, y)
{
	directionSprites.resize(DIRECTION_COUNT, nullptr);
}

void Enemy::OnUpdate(float elapsedTime)
{
	DynamicObject* player = engine->player;

	bool playerVisible = false;

	float objectAngle, objectDistance;
	if (engine->DynamicObjectVisible(this, engine->player, objectAngle, objectDistance))
	{
		playerVisible = true;
		playerDetected = true;
	}

	switch (state)
	{
		case PATROL:
		{
			if (playerDetected)
			{
				state = COMBAT;
				engine->PlayAudio("Achtung");
			}

			texture = ChooseDirectionSprite(player);
			break;
		}
		case COMBAT:
		{
			if (!playerVisible)
			{
				state = FOLLOW;
				shootingDelayed = true;
				shootingDelay = (float)(rand() / (float)(RAND_MAX / (1.0f - 0.25f))) + 0.25f;
				delayTimer = 0.0f;
				texture = ChooseDirectionSprite(player);
				break;
			}

			angle += objectAngle;
			if (weapon->Ready())
			{
				if (shootingDelayed)
				{
					delayTimer += elapsedTime;
					if (delayTimer >= shootingDelay)
					{
						delayTimer = 0.0f;
						shootingDelayed = false;
					}
					texture = aimingSpr;
				}
				else
				{
					if (weapon->GetAmmo() > 0)
					{
						weapon->Fire();
						shootingDelayed = true;
						shootingDelay = (float)(rand() / (float)(RAND_MAX / (1.0f - 0.25f))) + 0.25f;
						texture = shootingSpr;
					}
					else
					{
						texture = aimingSpr;
					}
				}
			}
			break;
		}
		case FOLLOW:
		{
			if (playerVisible)
			{
				state = COMBAT;
				angle += objectAngle;
				texture = ChooseDirectionSprite(player);
				break;
			}

			float moveX, moveY;
			if (engine->FindMove(this, player, moveX, moveY))
			{
				angle += objectAngle;

				float dirX = (moveX + 0.5f) - x;
				float dirY = (moveY + 0.5f) - y;

				float magInv = 1.0f / sqrt(dirX * dirX + dirY * dirY);
				dirX *= magInv;
				dirY *= magInv;

				float prevX = x;
				float prevY = y;

				x += dirX * speed * elapsedTime;
				y += dirY * speed * elapsedTime;

				if (engine->IsObstacle(x, y, this))
				{
					x = prevX;
					y = prevY;
				}
			}
			texture = ChooseDirectionSprite(player);
			break;
		}
		case PAIN:
		{
			painTimer += elapsedTime;
			if (painTimer >= pain)
			{
				painTimer = 0.0f;
				state = COMBAT;
			}
			texture = painSpr;
			break;
		}
	}

	weapon->OnUpdate(elapsedTime);
}

Sprite* Enemy::ChooseDirectionSprite(DynamicObject *relativeObject) const
{
	//Find direction vector of the enemy
	float eyeX = cosf(angle);
	float eyeY = sinf(angle);

	//Find direction vector of the relative object
	float objEyeX = cosf(relativeObject->GetAngle());
	float objEyeY = sinf(relativeObject->GetAngle());

	//Both vectors are normalized, dot product is a value between -1 to 1 that indicates the similarity between their directions
	float dot = (eyeX * objEyeX) + (eyeY * objEyeY);

	//We also need to know the side of the relative object. We can just rotate the enemy by half-pi radians and do another dot product
	//to find the side
	float rotatedEyeX = cosf(angle + 3.14159f * 0.5f);
	float rotatedEyeY = sinf(angle + 3.14159f * 0.5f);

	//Find vector from enemy towards relative object
	float dirX = relativeObject->GetX() - x;
	float dirY = relativeObject->GetY() - y;

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