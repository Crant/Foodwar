#pragma once

#include "Object.h"

class Enemy : public Object
{
protected:
	Iw2DSceneGraph::CSprite* zEnemySprite;
	
public:
	Enemy();
	~Enemy();

	virtual void Init(Iw2DSceneGraph::CSprite* pSprite, float pVel) = 0;
	virtual void Update(float pDt, float pAlphaMul);
	virtual void Render();

	Iw2DSceneGraph::CSprite*& GetEnemySprite() { return this->zEnemySprite; }
};