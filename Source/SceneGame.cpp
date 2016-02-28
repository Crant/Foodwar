#include "SceneGame.h"

#include "IwGx.h"
#include "Resource.h"
#include "Input.h"
#include "Carrot.h"
#include "Cauliflower.h"
#include "Paprika.h"
using namespace Iw2DSceneGraph;

//Velocity for sprite movement
#define VELOCITY_SPRITE 50

Game::Game()
{
	srand((unsigned)(time(0)));
	this->zPlayer = 0;
}

Game::~Game()
{
	SAFE_DELETE(this->zPlayer);
	for (auto it = this->zEnemys.begin(); it != this->zEnemys.end(); it++)
	{
		SAFE_DELETE(*it);
	}
	
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

	this->zPlayer->Init(playerSprite, VELOCITY_SPRITE *  2);

	float width = RESOURCE_MANAGER->GetTomato()->GetWidth() * this->zGraphics_ScaleX;
	int amountOfEnemyStartLocations = IwGxGetScreenWidth() / width;

	for (int i = 0; i < amountOfEnemyStartLocations - 1; i++)
	{
		this->zEnemyStartPos.push_back(i * RESOURCE_MANAGER->GetCarrot()->GetWidth()* this->zGraphics_ScaleX);
	}

	CSprite* enemySprite = new CSprite();
	enemySprite->SetImage(RESOURCE_MANAGER->GetCarrot());

	int newPos = rand() % this->zEnemyStartPos.size();
	enemySprite->m_X = this->zEnemyStartPos[newPos];

	enemySprite->m_Y = 0;
	enemySprite->m_ScaleX = this->zGraphics_ScaleX;
	enemySprite->m_ScaleY = this->zGraphics_ScaleX;

	Enemy* enemy = new Carrot();

	enemy->Init(enemySprite, VELOCITY_SPRITE);

	this->zEnemys.push_back(enemy);
}

void Game::Update(float pDeltaTime /* = 0.0f */, float pAlphaMul /* = 1.0f */)
{
	if (!this->zIsActive)
		return;

	//Update base scene
	Scene::Update(pDeltaTime, pAlphaMul);

	this->zPlayer->Update(pDeltaTime, pAlphaMul);
	for (auto itEnemy = this->zEnemys.begin(); itEnemy != this->zEnemys.end(); itEnemy++)
	{
		(*itEnemy)->Update(pDeltaTime, pAlphaMul);
	}
	

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

	for (auto itEnemy = this->zEnemys.begin(); itEnemy != this->zEnemys.end(); itEnemy++)
	{
		(*itEnemy)->Render();
	}
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
	std::vector<Bullet*> bullets = this->zPlayer->GetBullets();

	bool bulletHit = false;
	for (auto it = bullets.begin(); it != bullets.end() && !bulletHit; it++)
	{
		CSprite* bulletSprite = (*it)->GetSprite();

		for (int i = 0; i < this->zEnemys.size(); i++)
		{
			Enemy* enemy = this->zEnemys[i];
			if (enemy->GetEnemySprite()->HitTest(bulletSprite->m_X, bulletSprite->m_Y))
			{
				bulletHit = true;

				enemy->SetHealth(enemy->GetHealth() - (*it)->GetDamage());
				if (enemy->GetHealth() <= 0)
				{
					//if (this->zEnemys.size() > 1)
					//{
					//	if (i < this->zEnemys.size() - 1)
					//	{
					//		this->zEnemys[i] = this->zEnemys[this->zEnemys.size() - 1];
					//		this->zEnemys[this->zEnemys.size() - 1] = enemy;
					//	}
					//	i--;
					//}
					//this->zEnemys.pop_back();
					SAFE_DELETE(enemy);
					int newPos = rand() % this->zEnemyStartPos.size();

					CSprite* enemySprite = new CSprite();
					
					Enemy* newEnemy;

					int enemyint = rand() % 300;
					if (enemyint >= 0 && enemyint < 100)
					{
						newEnemy = new Carrot();
						enemySprite->SetImage(RESOURCE_MANAGER->GetCarrot());
					}
						
					else if (enemyint >= 100 && enemyint < 200)
					{
						newEnemy = new Cauliflower();
						enemySprite->SetImage(RESOURCE_MANAGER->GetCauliflower());
					}	
					else
					{
						newEnemy = new Paprika();
						enemySprite->SetImage(RESOURCE_MANAGER->GetPaprika());
					}
						
					enemySprite->m_X = this->zEnemyStartPos[newPos];
					enemySprite->m_Y = 0;
					enemySprite->m_ScaleX = this->zGraphics_ScaleX;
					enemySprite->m_ScaleY = this->zGraphics_ScaleX;
					
					
					newEnemy->Init(enemySprite, VELOCITY_SPRITE);
					this->zEnemys[i] = newEnemy;
					//this->zEnemys.push_back(newEnemy);
				}

				this->zPlayer->RemoveBullet(*it);
			}
		}
		
	}
}
