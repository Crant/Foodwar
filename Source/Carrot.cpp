#include "Carrot.h"

void Carrot::Init(Iw2DSceneGraph::CSprite* pSprite, float pVel)
{
	this->zEnemySprite = pSprite;

	this->zVelocity = pVel * 2;

	this->zHealth = 50;
}