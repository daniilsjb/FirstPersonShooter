#include "ScoreItem.h"
#include "EngineFPS.h"
#include "Player.h"

ScoreItem::ScoreItem(EngineFPS* engine, int worth, Sprite* spr) : Item(engine), worth(worth)
{
	texture = spr;
}

void ScoreItem::OnUse(Player* player)
{
	player->AddScore(worth);
	removed = true;
	engine->PlayAudio("Pickup");
}