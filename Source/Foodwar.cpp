#include "Foodwar.h"

#include "Resource.h"
#include "Input.h"
#include "s3eFlurry.h"
#include "Scene.h"
#include "SceneGame.h"

// Replace this with your own Flurry API key
const char* FLURRY_API_KEY = NULL;

Foodwar::Foodwar()
{
	this->zGlobalTweener = 0;
}

Foodwar::~Foodwar()
{
	if (s3eFlurryAvailable())
		s3eFlurryEndSession();

	INPUT_MANAGER->Release();
	Input::Destroy();

	SCENE_MANAGER->Release();
	SceneManager::Destroy();

	RESOURCE_MANAGER->Release();
	Resource::Destroy();

	SAFE_DELETE(this->zGlobalTweener);

	s3eDeviceUnRegister(S3E_DEVICE_PAUSE, (s3eCallback)PauseCallback);
	s3eDeviceUnRegister(S3E_DEVICE_UNPAUSE, (s3eCallback)ResumeCallback);
}

void Foodwar::Init()
{
	//Set up Input System
	Input::Create();
	INPUT_MANAGER->Init();
	//Set up Scene Manager
	SceneManager::Create();
	SCENE_MANAGER->Init();
	//Set up Resources
	Resource::Create();
	RESOURCE_MANAGER->Init();

	if (!s3eFlurryAvailable())
	{
		IwTrace(S3EFLURRY, ("Flurry extension not found"));
	}
	else if (!FLURRY_API_KEY)
	{
		IwTrace(S3EFLURRY, ("Please supply a valid Flurry Api ID for FLURRY_API_KEY"));
	}
	else
	{
		s3eFlurryStartSession(FLURRY_API_KEY);
	}

	this->zGlobalTweener = new CTweenManager();

	Game* game = new Game();
	game->SetName("game");
	game->Init();
	game->SetGlobalTween(this->zGlobalTweener);
	SCENE_MANAGER->Add(game);

	SCENE_MANAGER->SwitchTo(game);
	//S3EFLURRY TIP:
	//Add this code wherever you want to log an event to flurry
	//    if (s3eFlurryAvailable())
	//        s3eFlurryLogEvent("<Event>");

	s3eDeviceUnRegister(S3E_DEVICE_PAUSE, (s3eCallback)PauseCallback);
	s3eDeviceUnRegister(S3E_DEVICE_UNPAUSE, (s3eCallback)ResumeCallback);
}

void Foodwar::Run()
{
	uint32 timer = (uint32)s3eTimerGetMs();

	while (!s3eDeviceCheckQuitRequest() &&
		!SCENE_MANAGER->QuitRequested())
	{

		INPUT_MANAGER->Update();
		if (INPUT_MANAGER->GetBackKeyStatus())
		{
			SCENE_MANAGER->OnBackButtonPressed();
		}
		if (INPUT_MANAGER->GetHomeKeyStatus())
		{
			SCENE_MANAGER->QuitGame();
		}

		int deltaTime = uint32(s3eTimerGetMs()) - timer;
		timer += deltaTime;

		//Make sure the delta time is safe
		if (deltaTime < 0)
			deltaTime = 0;
		if (deltaTime > 100)
			deltaTime = 100;

		Update(deltaTime);

		// Clear the drawing surface
		Iw2DSurfaceClear(0xff000000);

		SCENE_MANAGER->Render();

		// Show the drawing surface
		Iw2DSurfaceShow();

		// Yield to OS
		s3eDeviceYield(0);
	}
}

void Foodwar::Update(int delta)
{
	this->zGlobalTweener->Update(delta / 1000.0f);

	SCENE_MANAGER->Update(delta / 1000.0f);
}

void Foodwar::PauseCallback(void* systemData, void* userData)
{
	SCENE_MANAGER->OnPause();
}

void Foodwar::ResumeCallback(void* systemData, void* userData)
{
	SCENE_MANAGER->OnResume();
}