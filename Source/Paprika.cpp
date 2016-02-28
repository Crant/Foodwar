#include "Paprika.h"

void Paprika::Init(Iw2DSceneGraph::CSprite* pSprite, float pVel)
{
	this->zEnemySprite = pSprite;

	this->zVelocity = pVel * 0.8f;

	this->zHealth = 100;
}

