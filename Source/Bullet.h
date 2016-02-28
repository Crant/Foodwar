#pragma once

#include "Iw2DSceneGraph.h"

class Bullet
{
private:
	Iw2DSceneGraph::CSprite* zBulletSprite;
	float zDamage;

public:
	Bullet(Iw2DSceneGraph::CSprite* pSprite, float pDamage);
	~Bullet();

	void Update(float pDt, float pAlphaMul);

	float GetDamage() { return this->zDamage; }
	Iw2DSceneGraph::CSprite* GetSprite() { return this->zBulletSprite; }
};