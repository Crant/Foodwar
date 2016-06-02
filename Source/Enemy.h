#pragma once

#include "Object.h"

enum ENEMY_TYPE
{
	ENEMY_TYPE_RANDOM,
	ENEMY_TYPE_CARROT,
	ENEMY_TYPE_CAULIFLOWER,
	ENEMY_TYPE_PAPRIKA
};

class Enemy : public Object
{
protected:
	Iw2DSceneGraph::CSprite* zEnemySprite;
	int zScoreValue;
public:
	Enemy();
	~Enemy();

	virtual void Init(Iw2DSceneGraph::CSprite* pSprite, float pVel) = 0;
	virtual void Update(float pDt, float pAlphaMul);
	virtual void Render();

	int GetScore() { return this->zScoreValue; }
	Iw2DSceneGraph::CSprite*& GetEnemySprite() { return this->zEnemySprite; }
};