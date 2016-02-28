#include "Bullet.h"
#include "ClassUtil.h"

Bullet::Bullet(Iw2DSceneGraph::CSprite* pSprite, float pDamage)
{
	this->zBulletSprite = pSprite;

	this->zDamage = pDamage;
}

Bullet::~Bullet()
{
	SAFE_DELETE(this->zBulletSprite);
}

void Bullet::Update(float pDt, float pAlphaMul)
{
	this->zBulletSprite->Update(pDt, pAlphaMul);
}
