#include "Enemy.h"
#include "Input.h"
#include "Resource.h"
#include "IwGx.h"

Enemy::Enemy()
{
	this->zEnemySprite = 0;
}

Enemy::~Enemy()
{
	SAFE_DELETE(this->zEnemySprite);
}

void Enemy::Init(Iw2DSceneGraph::CSprite* pSprite, float pVel)
{
	this->zEnemySprite = pSprite;

	this->zVelocity = pVel;

	this->zHealth = 100;
}

void Enemy::Update(float pDt, float pAlphaMul)
{
	this->zEnemySprite->Update(pDt, pAlphaMul);

	//Move Enemy
	this->zEnemySprite->m_Y += this->zVelocity * pDt;
	if (this->zEnemySprite->m_Y > IwGxGetScreenHeight())
	{
		this->zEnemySprite->m_Y = 0;
	}
}

void Enemy::Render()
{
	this->zEnemySprite->Render();
}