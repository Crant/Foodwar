#include "Player.h"
#include "Input.h"
#include "Resource.h"
#include "IwGx.h"

Player::Player()
{
	this->zPlayerSprite = 0;
}

Player::~Player()
{
	SAFE_DELETE(this->zPlayerSprite);

	for (int i = 0; i < this->zBullets.size(); i++)
	{
		SAFE_DELETE(this->zBullets[i]);
	}
}

void Player::Init(Iw2DSceneGraph::CSprite* pSprite, float pVel)
{
	this->zPlayerSprite = pSprite;

	this->zVelocity = pVel;

	this->zHealth = 100;

	this->zBulletTimer = 0;
}

void Player::Update(float pDt, float pAlphaMul)
{
	static const float BULLET_DELAY = 0.66f;

	this->zBulletTimer += pDt;

	this->zPlayerSprite->Update(pDt, pAlphaMul);

	for (auto it = this->zBullets.begin(); it != this->zBullets.end(); it++)
	{
		(*it)->Update(pDt, pAlphaMul);
	}

	//Delete bullets that have gone outside the screen
	for (int i = 0; i < this->zBullets.size(); i++)
	{
		Iw2DSceneGraph::CSprite* bulletSprite = this->zBullets[i]->GetSprite();
		if (bulletSprite->m_Y <= 0)
		{
			Bullet* temp = this->zBullets[i];

			//Move the bullet last and remove it
			this->zBullets[i] = this->zBullets[this->zBullets.size() - 1];
			this->zBullets[this->zBullets.size() - 1] = temp;

			//temp->cleanup();
			this->zBullets.pop_back();
			SAFE_DELETE(temp);
			i--;
		}
	}

	// Move bullets up
	for (auto it = this->zBullets.begin(); it != this->zBullets.end(); it++)
	{
		Iw2DSceneGraph::CSprite* bulletSprite = (*it)->GetSprite();
		bulletSprite->m_Y -= this->zVelocity * pDt;
	}

	//Check if it's time to shoot another bullet
	if (this->zBulletTimer >= BULLET_DELAY)
	{
		this->zBulletTimer = 0;

		
		Iw2DSceneGraph::CSprite* bulletSprite = new Iw2DSceneGraph::CSprite();

		bulletSprite->SetImage(RESOURCE_MANAGER->GetTomato());
		//Place the bullet above the player
		bulletSprite->m_X = this->zPlayerSprite->m_X;
		bulletSprite->m_Y = this->zPlayerSprite->m_Y;
		//Scale it to fit the screen
		bulletSprite->m_ScaleX = this->zPlayerSprite->m_ScaleX * 0.25f;
		bulletSprite->m_ScaleY = this->zPlayerSprite->m_ScaleY * 0.25f;
		
		Bullet* bullet = new Bullet(bulletSprite, 25);
		this->zBullets.push_back(bullet);
	}
}

void Player::Render()
{
	for (auto it = this->zBullets.begin(); it != this->zBullets.end(); it++)
	{
		(*it)->GetSprite()->Render();
	}

	this->zPlayerSprite->Render();
}

void Player::UpdatePlayerPos(float dt)
{
	//Move Player With Input
	float x = INPUT_MANAGER->GetX_Position() - this->zPlayerSprite->m_X;
	float y = INPUT_MANAGER->GetY_Position() - this->zPlayerSprite->m_Y;

	if (x > 0)
	{
		this->zPlayerSprite->m_X += this->zVelocity * dt;
	}
	else
	{
		this->zPlayerSprite->m_X -= this->zVelocity * dt;
	}

	if (y > 0)
	{
		this->zPlayerSprite->m_Y += this->zVelocity * dt;
	}
	else
	{
		this->zPlayerSprite->m_Y -= this->zVelocity * dt;
	}
	//Check if player is outside the screen
	//Sprite Anchor is at the center so add half width * scale to offset
	if (this->zPlayerSprite->m_X - (this->zPlayerSprite->m_W * 0.5f * this->zPlayerSprite->m_ScaleX) <= 0)
	{
		this->zPlayerSprite->m_X = this->zPlayerSprite->m_W * 0.5f * this->zPlayerSprite->m_ScaleX;
	}
	else if (this->zPlayerSprite->m_X + (this->zPlayerSprite->m_W * 0.5f * this->zPlayerSprite->m_ScaleX) >= IwGxGetScreenWidth())
	{
		this->zPlayerSprite->m_X = IwGxGetScreenWidth() - this->zPlayerSprite->m_W * 0.5f * this->zPlayerSprite->m_ScaleX;
	}

	//Sprite Anchor is at the center so add half height * scale to offset
	if (this->zPlayerSprite->m_Y - (this->zPlayerSprite->m_H * 0.5f * this->zPlayerSprite->m_ScaleY) <= 0)
	{
		this->zPlayerSprite->m_Y = this->zPlayerSprite->m_H * 0.5f * this->zPlayerSprite->m_ScaleY;
	}
	else if (this->zPlayerSprite->m_Y + (this->zPlayerSprite->m_H * 0.5f * this->zPlayerSprite->m_ScaleY) >= IwGxGetScreenHeight())
	{
		this->zPlayerSprite->m_Y = IwGxGetScreenHeight() - this->zPlayerSprite->m_H * 0.5f * this->zPlayerSprite->m_ScaleY;
	}
}

void Player::RemoveBullet(Bullet* bullet)
{
	bool found = false;

	for (int i = 0; i < this->zBullets.size(); i++)
	{
		if (this->zBullets[i] == bullet)
		{
			Bullet* temp = this->zBullets[i];
			this->zBullets[i] = this->zBullets[this->zBullets.size() - 1];
			this->zBullets[this->zBullets.size() - 1] = temp;
			this->zBullets.pop_back();
			found = true;

			SAFE_DELETE(temp);
		}
	}
}
