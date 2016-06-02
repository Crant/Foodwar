#include "SceneGame.h"

#include "IwGx.h"
#include "Resource.h"
#include "Input.h"
#include "Carrot.h"
#include "Cauliflower.h"
#include "Paprika.h"

#include <sstream>
using namespace Iw2DSceneGraph;

//Velocity for sprite movement
#define VELOCITY_SPRITE 50

Game::Game()
{
	srand((unsigned)(time(0)));
	this->zPlayer = 0;
	this->zDeltaTime = 0;
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

	this->zPlayer->Init(playerSprite, VELOCITY_SPRITE *  2.5);

	//Calculate the amount of images that can fit on the screen width (every enemy has the same dimensions)
	float width = RESOURCE_MANAGER->GetCarrot()->GetWidth() * this->zGraphics_ScaleX;
	int amountOfEnemyStartLocations = IwGxGetScreenWidth() / width;

	for (int i = 0; i < amountOfEnemyStartLocations; i++)
	{
		this->zEnemyStartPos.push_back(i * width);
	}
	//Create Enemies
	CSprite* enemySprite = new CSprite();
	enemySprite->SetImage(RESOURCE_MANAGER->GetCarrot());

	int newPos = rand() % this->zEnemyStartPos.size();
	enemySprite->m_X = this->zEnemyStartPos[newPos];

	enemySprite->m_Y = 0;
	enemySprite->m_AnchorX = 0.5f;
	enemySprite->m_AnchorY = 0.5f;
	enemySprite->m_ScaleX = this->zGraphics_ScaleX;
	enemySprite->m_ScaleY = this->zGraphics_ScaleX;

	Enemy* enemy = new Carrot();

	enemy->Init(enemySprite, VELOCITY_SPRITE);

	this->zEnemys.push_back(enemy);

	this->zGameScoreLabel = new CLabel();
	this->zGameScoreLabel->m_X = -20 * this->zGraphics_ScaleX;
	this->zGameScoreLabel->m_Y = 10 * this->zGraphics_ScaleY;
	this->zGameScoreLabel->m_W = FONT_DESIGN_WIDTH;
	this->zGameScoreLabel->m_H = this->zActualFontHeight;

	this->zGameScoreLabel->m_AlignHor = IW_2D_FONT_ALIGN_RIGHT;
	this->zGameScoreLabel->m_AlignVer = IW_2D_FONT_ALIGN_TOP;
	this->zGameScoreLabel->m_Font = RESOURCE_MANAGER->GetFontNormal();
	this->zGameScoreLabel->m_ScaleX = this->zFont_Scale;// / this->zGraphics_Scale;
	this->zGameScoreLabel->m_ScaleY = this->zFont_Scale;// / this->zGraphics_Scale;
	this->zGameScoreLabel->m_Color = CColor(255, 255, 255, 255);

	AddChild(this->zGameScoreLabel);
}

void Game::Update(float pDeltaTime /* = 0.0f */, float pAlphaMul /* = 1.0f */)
{
	if (!this->zIsActive)
		return;

	this->zDeltaTime = pDeltaTime;
	//Update base scene
	Scene::Update(pDeltaTime, pAlphaMul);

	this->UpdateEvents();

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

	this->CheckEvents();

	UpdateScoreText();
}

void Game::UpdateEvents()
{
	for (auto it = this->zEvents.begin(); it != this->zEvents.end(); it++)
	{
		if (EnemyHitEvent* EHE = dynamic_cast<EnemyHitEvent*>(*it))
		{
			if (EHE->zDelayTimer > 0)
			{
				EHE->zDelayTimer -= this->zDeltaTime;
			}
		}
	}
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

void Game::UpdateScoreText()
{
	std::stringstream ss;

	ss << this->zPlayer->GetScore();

	this->zGameScoreLabel->m_Text = ss.str();
}

void Game::CheckBulletCollisions()
{
	std::vector<Bullet*> bullets = this->zPlayer->GetBullets();

	std::vector<Bullet*> bulletsToRemove;
	bool bulletHit = false;
	//Loop Through bullets
	for (auto it = bullets.begin(); it != bullets.end(); it++)
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
					int score = enemy->GetScore();

					this->zPlayer->AddScore(score);
					//Remove Current enemy
					SAFE_DELETE(enemy);

					//Currently replace with a new enemy, TODO add enemies over time instead on on death
					int newPos = rand() % this->zEnemyStartPos.size();
					this->CreateEnemy(enemy, this->zEnemyStartPos[newPos], ENEMY_TYPE_RANDOM);

					this->zEnemys[i] = enemy;
				}
				else
				{
					EnemyHitEvent* enemyHit = new EnemyHitEvent();
					enemyHit->zEnemy = enemy;
					enemyHit->zHit = true;
					enemyHit->zDelayTimer = 0;
					this->zEvents.push_back(enemyHit);
				}
				BulletRemoveEvent* bulletEvent = new BulletRemoveEvent();

				bulletEvent->zBullet = (*it);
				bulletEvent->zBulletOwner = this->zPlayer;

				this->zEvents.push_back(bulletEvent);
			}
		}
	}
}

void Game::CreateEnemy(Enemy*& pEnemy, int pPosition, ENEMY_TYPE pType /*= ENEMY_TYPE_RANDOM*/)
{	
	CSprite* enemySprite = new CSprite();
	if (pType == ENEMY_TYPE_CARROT)
	{
		pEnemy = new Carrot();
		enemySprite->SetImage(RESOURCE_MANAGER->GetCarrot());
	}
	else if (pType == ENEMY_TYPE_CAULIFLOWER)
	{
		pEnemy = new Cauliflower();
		enemySprite->SetImage(RESOURCE_MANAGER->GetCauliflower());
	}
	else if (pType == ENEMY_TYPE_PAPRIKA)
	{
		pEnemy = new Paprika();
		enemySprite->SetImage(RESOURCE_MANAGER->GetPaprika());
	}
	else
	{
		int enemyint = rand() % 300;
		if (enemyint >= 0 && enemyint < 100)
		{
			pEnemy = new Carrot();
			enemySprite->SetImage(RESOURCE_MANAGER->GetCarrot());
		}

		else if (enemyint >= 100 && enemyint < 200)
		{
			pEnemy = new Cauliflower();
			enemySprite->SetImage(RESOURCE_MANAGER->GetCauliflower());
		}
		else
		{
			pEnemy = new Paprika();
			enemySprite->SetImage(RESOURCE_MANAGER->GetPaprika());
		}
	}

	//Set the position and scale of the new enemy
	enemySprite->m_X = pPosition;
	enemySprite->m_Y = 0;
	enemySprite->m_ScaleX = this->zGraphics_ScaleX;
	enemySprite->m_ScaleY = this->zGraphics_ScaleX;

	pEnemy->Init(enemySprite, VELOCITY_SPRITE);
}

void Game::CheckEvents()
{
	for (auto it = this->zEvents.begin(); it != this->zEvents.end(); it++)
	{
		if (EnemyHitEvent* EHE = dynamic_cast<EnemyHitEvent*>(*it))
		{
			if (EHE->zDelayTimer <= 0)
			{
				if (EHE->zHit)
				{
					this->GetGlobalTween()->Tween(0.05f,
						UINT, &EHE->zEnemy->GetEnemySprite()->m_Color.a, 25,
						END);

					EHE->zHit = false;
					EHE->zDelayTimer = 0.05f;
				}
				else
				{
					this->GetGlobalTween()->Tween(0.05f,
						UINT, &EHE->zEnemy->GetEnemySprite()->m_Color.a, 255,
						END);

					SAFE_DELETE(EHE);
					it = this->zEvents.erase(it);
				}
			}

		}
		else if (BulletRemoveEvent* BRE = dynamic_cast<BulletRemoveEvent*>(*it))
		{
			BRE->zBulletOwner->RemoveBullet(BRE->zBullet);

			SAFE_DELETE(BRE);
			it = this->zEvents.erase(it);
		}
	}
}