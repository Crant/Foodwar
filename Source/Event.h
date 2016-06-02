#pragma once

#include "Bullet.h"
#include "Player.h"
#include "Enemy.h"
class Event
{
public:
	virtual ~Event() {}
};

class BulletRemoveEvent : public Event
{
public:
	virtual ~BulletRemoveEvent() {}
	Player* zBulletOwner;
	Bullet* zBullet;
};

class EnemyHitEvent : public Event
{
public:
	virtual ~EnemyHitEvent() {}
	float zDelayTimer;
	Enemy* zEnemy;
	bool zHit;
};