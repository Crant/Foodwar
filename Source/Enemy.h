#pragma once

#include "Object.h"

class Enemy : public Object
{
private:
	Iw2DSceneGraph::CSprite* zEnemySprite;
	
public:
	Enemy();
	~Enemy();

	virtual void Init(Iw2DSceneGraph::CSprite* pSprite, float pVel);
	virtual void Update(float pDt, float pAlphaMul);
	virtual void Render();

	Iw2DSceneGraph::CSprite*& GetEnemySprite() { return this->zEnemySprite; }
};