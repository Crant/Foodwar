#pragma once

#include "Object.h"
#include "Bullet.h"

class Player : public Object
{
private:
	Iw2DSceneGraph::CSprite* zPlayerSprite;
	std::vector<Bullet*> zBullets;
	//cocos2d::CCSprite* zPlayerSprite;
	//std::vector<cocos2d::CCSprite*> zBullets;
	float zBulletTimer;
public:
	Player();
	~Player();

	virtual void Render();

	virtual void Update(float pDt, float pAlphaMul);
	virtual void Init(Iw2DSceneGraph::CSprite* pSprite, float pVel) override;

	void UpdatePlayerPos(float dt);

	Iw2DSceneGraph::CSprite*& GetPlayerSprite() { return this->zPlayerSprite; }
	std::vector<Bullet*>& GetBullets() { return this->zBullets; }
	void RemoveBullet(Bullet* bullet);
};