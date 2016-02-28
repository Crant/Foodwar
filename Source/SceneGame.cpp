#include "SceneGame.h"

#include "IwGx.h"
#include "Resource.h"
#include "Input.h"
using namespace Iw2DSceneGraph;

//Velocity for sprite movement
#define VELOCITY_SPRITE 50

Game::Game()
{
	srand((unsigned)(time(0)));
	this->zPlayer = 0;
	this->zEnemy = 0;
}

Game::~Game()
{
	SAFE_DELETE(this->zPlayer);
	SAFE_DELETE(this->zEnemy);
}

void Game::Init()
{
	Scene::Init();

	this->zGraphics_ScaleX = (float)IwGxGetScreenWidth() / GRAPHIC_DESIGN_WIDTH;
	this->zGraphics_ScaleY = (float)IwGxGetScreenHeight() / GRAPHIC_DESIGN_HEIGHT;

	this->zFont_Scale = (float)IwGxGetScreenWidth() / FONT_DESIGN_WIDTH;
	this->zActualFontHeight = FONT_HEIGHT * this->zFont_Scale;
	
	CSprite* playerSprite = new CSprite();
	playerSprite->SetImage(RESOURCE_MANAGER->GetTomato());
	playerSprite->m_X = IwGxGetScreenWidth() / 2;
	playerSprite->m_Y = IwGxGetScreenHeight() - playerSprite->GetImage()->GetHeight() * this->zGraphics_ScaleX;
	playerSprite->m_ScaleX = this->zGraphics_ScaleX;
	playerSprite->m_ScaleY = this->zGraphics_ScaleX;
	playerSprite->m_AnchorX = 0.5f;
	playerSprite->m_AnchorY = 0.5f;

	this->zPlayer = new Player();

	this->zPlayer->Init(playerSprite, VELOCITY_SPRITE);

	float width = RESOURCE_MANAGER->GetTomato()->GetWidth() * this->zGraphics_ScaleX;
	int amountOfEnemyStartLocations = IwGxGetScreenWidth() / width;

	for (int i = 0; i < amountOfEnemyStartLocations - 1; i++)
	{
		this->zEnemyStartPos.push_back((i + 1) * RESOURCE_MANAGER->GetTomato()->GetWidth()* this->zGraphics_ScaleX);
	}

	CSprite* enemySprite = new CSprite();
	enemySprite->SetImage(RESOURCE_MANAGER->GetTomato());
	enemySprite->m_X = 100 * this->zGraphics_ScaleX;
	enemySprite->m_Y = enemySprite->GetImage()->GetHeight() * this->zGraphics_ScaleY;
	enemySprite->m_ScaleX = this->zGraphics_ScaleX;
	enemySprite->m_ScaleY = this->zGraphics_ScaleX;

	this->zEnemy = new Enemy();

	this->zEnemy->Init(enemySprite, VELOCITY_SPRITE);
}

void Game::Update(float pDeltaTime /* = 0.0f */, float pAlphaMul /* = 1.0f */)
{
	if (!this->zIsActive)
		return;

	//Update base scene
	Scene::Update(pDeltaTime, pAlphaMul);

	this->zPlayer->Update(pDeltaTime, pAlphaMul);
	this->zEnemy->Update(pDeltaTime, pAlphaMul);

	if (INPUT_MANAGER->GetTouchedStatus() && !INPUT_MANAGER->GetPrevTouchedStatus())
	{
		INPUT_MANAGER->Reset();

		this->zPlayer->UpdatePlayerPos(pDeltaTime);
	}

	this->CheckBulletCollisions();
}

void Game::Render()
{
	if (!this->zIsActive)
		return;

	Scene::Render();

	this->zPlayer->Render();

	this->zEnemy->Render();
}

void Game::OnBackKeyPress()
{
	//Swap screen
}

void Game::OnPause()
{
	this->zIsActive = false;
}

void Game::OnResume()
{
	this->zIsActive = true;
}

void Game::OnSwap()
{

}

void Game::CheckBulletCollisions()
{
	CSprite* enemy = this->zEnemy->GetEnemySprite();

	std::vector<Bullet*> bullets = this->zPlayer->GetBullets();

	bool bulletHit = false;
	for (auto it = bullets.begin(); it != bullets.end() && !bulletHit; it++)
	{
		CSprite* bulletSprite = (*it)->GetSprite();
		if(enemy->HitTest(bulletSprite->m_X, bulletSprite->m_Y))
		{
			bulletHit = true;

			this->zEnemy->SetHealth(this->zEnemy->GetHealth() - (*it)->GetDamage());
			if (this->zEnemy->GetHealth() <= 0)
			{
				enemy->m_Y = 0;
				int newPos = rand() % this->zEnemyStartPos.size();

				enemy->m_X = this->zEnemyStartPos[newPos];

				this->zEnemy->SetHealth(100);
			}
			
			this->zPlayer->RemoveBullet(*it);
		}
	}
}
