#include "Cauliflower.h"

void Cauliflower::Init(Iw2DSceneGraph::CSprite* pSprite, float pVel)
{
	this->zEnemySprite = pSprite;

	this->zVelocity = pVel * 0.66f;

	this->zHealth = 125;
}