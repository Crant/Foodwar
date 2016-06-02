#pragma once

#include "Scene.h"
#include "Player.h"
#include "Enemy.h"
#include "Event.h"

// Constants that are used to fit the game to different screen sizes
#define FONT_HEIGHT             10.0f
#define FONT_DESIGN_WIDTH       320.0f
#define GRAPHIC_DESIGN_WIDTH    768.0f
#define GRAPHIC_DESIGN_HEIGHT   1136.0f

class Game : public Scene
{
private: //Variables
	float zGraphics_ScaleX;
	float zGraphics_ScaleY;
	float zFont_Scale;
	float zActualFontHeight;

	std::vector<Enemy*> zEnemys;
	Player* zPlayer;
	std::vector<int> zEnemyStartPos;
	std::list<Event*> zEvents;

	Iw2DSceneGraph::CLabel* zGameScoreLabel;

	float zDeltaTime;
private: //Functions
	void CheckBulletCollisions();
	
	bool CheckCollission(Iw2DSceneGraph::CSprite* enemy, Iw2DSceneGraph::CSprite* bullet);

	void CreateEnemy(Enemy*& pEnemy, int pPostion, ENEMY_TYPE pType = ENEMY_TYPE_RANDOM);

	void CheckEvents();
	void UpdateEvents();
public:
	Game();
	Game(const char* pHashName) { this->SetName(pHashName); }
	virtual ~Game();

	// Initialize the Game
	void Init();

	//Update the Game
	void Update(float pDeltaTime /* = 0.0f */, float pAlphaMul /* = 1.0f */);

	//Render the game
	void Render();

	//Virtual Functions
	virtual void OnBackKeyPress();

	virtual void OnPause();

	virtual void OnResume();

	virtual void OnSwap();
	void UpdateScoreText();
};