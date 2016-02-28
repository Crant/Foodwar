#pragma once

#include "Enemy.h"

class Carrot : public Enemy
{
public:
	virtual void Init(Iw2DSceneGraph::CSprite* pSprite, float pVel);
};