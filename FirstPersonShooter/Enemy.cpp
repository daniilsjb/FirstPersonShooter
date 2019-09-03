#include "Enemy.h"
#include "Player.h"
#include "Item.h"

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
	float objEyeX = cosf(relativeObject->GetAngle());
	float objEyeY = sinf(relativeObject->GetAngle());

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

void Enemy::OnUpdate(float elapsedTime)
{
	DynamicObject* player = engine->player;

	bool playerVisible = false;

	float objectAngle, distance;
	if (engine->ObjectWithinFoV(x, y, angle, player->x, player->y, objectAngle, distance))
	{
		float rayX, rayY, rayDistance;
		if (engine->CastRay(x, y, angle + objectAngle, rayX, rayY, rayDistance, true, true, this))
		{
			if (engine->GetDynamicObject(rayX, rayY) == player)
			{
				playerVisible = true;
				playerDetected = true;
			}
		}
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
					texture = reloadingSpr;
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
						texture = reloadingSpr;
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
	}

	weapon->OnUpdate(elapsedTime);
}

Guard::Guard(EngineFPS *engine) : Enemy(engine)
{
	directionSprites[BACK] = engine->GetSprite("Guard Back");
	directionSprites[RIGHT] = engine->GetSprite("Guard Right");
	directionSprites[FRONT] = engine->GetSprite("Guard Front");
	directionSprites[LEFT] = engine->GetSprite("Guard Left");

	reloadingSpr = engine->GetSprite("Guard Reload");
	shootingSpr = engine->GetSprite("Guard Fire");

	currentHealth = maxHealth = 50;

	state = PATROL;

	angle = 3.14159f;

	speed = 1.5f;

	weapon = engine->CreateWeapon(Weapons::MACHINE_GUN, this);
}

void Guard::OnHit(int damage)
{
	playerDetected = true;

	Damage(damage);
	if (currentHealth <= 0)
	{
		removed = true;
		engine->AddItem((int)x, (int)y, 'G');
		engine->PlayAudio("Enemy Death 1");
	}

	if (rand() % 5 == 0)
		engine->PlayAudio("Enemy Pain");
}