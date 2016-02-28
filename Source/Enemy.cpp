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