#include "Scene.h"

#include "IwGx.h"
#include "Input.h"

//
//
// Scene class
//
//

Scene::Scene()
{
	m_X = -(float)IwGxGetScreenWidth();
	this->zIsActive = false;
	this->zIsInputActive = false;
}

Scene::~Scene()
{

}

void Scene::SetName( const char* pName )
{
	this->zNameHash = IwHashString(pName);
}

void Scene::Init()
{

}

void Scene::Update( float pDeltaTime /*= 0.0f*/, float pAlphaMul /*= 1.0f*/ )
{
	if(!this->zIsActive)
		return;

	this->zTweener.Update(pDeltaTime);

	CNode::Update(pDeltaTime, pAlphaMul);
}

void Scene::Render()
{
	CNode::Render();
}
//
//
// SceneManager class
//
//

CDECLARE_SINGLETONS(SceneManager)

void SceneManager::SwitchTo( Scene* pScene )
{
	this->zNextScene = pScene;
	
	if(this->zCurrentScene == 0) // No scene exist
	{
		this->zCurrentScene = this->zNextScene;
		this->zCurrentScene->m_X = 0;
		this->zCurrentScene->SetActive(true);
		this->zCurrentScene->SetInputActive(true);
		this->zCurrentScene->OnSwap();
		this->zNextScene = 0;
	}
	else
	{
		this->zCurrentScene->SetInputActive(false);
		this->zNextScene->OnSwap();
		this->zNextScene->SetActive(true);
		this->zNextScene->m_X = -(float)IwGxGetScreenWidth();
		this->zNextScene->GetGlobalTween()->Tween(0.5f, 
			FLOAT, &this->zNextScene->m_X, 0.0f,
			FLOAT, &this->zCurrentScene->m_X, (float)IwGxGetScreenWidth(), 
			EASING, Ease::sineIn, 
			ONCOMPLETE, OnSwitchComplete, 
			END);
	}
}

void SceneManager::Render()
{
	for (std::list<Scene*>::iterator it = this->zScenes.begin(); it != this->zScenes.end(); it++)
		(*it)->Render();
}

void SceneManager::Update( float pDeltaTime /*= 0.0f*/ )
{
	for (std::list<Scene*>::iterator it = this->zScenes.begin(); it != this->zScenes.end(); it++)
	{
		(*it)->Update(pDeltaTime);
	}
}

void SceneManager::Add( Scene* pScene )
{
	this->zScenes.push_back(pScene);
	pScene->SetManager(this);
}

void SceneManager::Remove( Scene* pScene )
{
	this->zScenes.remove(pScene);
}

Scene* SceneManager::Find( const char* pName )
{
	unsigned int nameHash = IwHashString(pName);
	
	for (std::list<Scene*>::iterator it = this->zScenes.begin(); it != this->zScenes.end(); it++)
	{
		if((*it)->GetNameHash() == nameHash)
			return *it;
	}

	return 0;
}

void SceneManager::FinishSwitch()
{
	this->zNextScene->SetInputActive(true);
	this->zNextScene->SetActive(true);
	
	this->zPrevScene = this->zCurrentScene;

	this->zCurrentScene->Update(0);
	this->zCurrentScene->SetActive(false);
	this->zCurrentScene = this->zNextScene;
	this->zNextScene = 0;
}

void SceneManager::OnSwitchComplete( CTween* pTween )
{
	SCENE_MANAGER->FinishSwitch();
}

void SceneManager::OnBackButtonPressed()
{
	for (std::list<Scene*>::iterator it = this->zScenes.begin(); it != this->zScenes.end(); it++)
	{
		if( (*it) == this->zCurrentScene)
			(*it)->OnBackKeyPress();
	}
}

void SceneManager::OnPause()
{
	for (std::list<Scene*>::iterator it = this->zScenes.begin(); it != this->zScenes.end(); it++)
	{
		if( (*it) == this->zCurrentScene)
			(*it)->OnPause();
	}
}

void SceneManager::OnResume()
{
	for (std::list<Scene*>::iterator it = this->zScenes.begin(); it != this->zScenes.end(); it++)
	{
		if( (*it) == this->zCurrentScene)
			(*it)->OnResume();
	}
}

void SceneManager::Init()
{
	this->zQuitRequest = false;
	this->zCurrentScene = 0;
	this->zNextScene = 0;
	this->zPrevScene = 0;
}

void SceneManager::Release()
{
	for(std::list<Scene*>::iterator it = this->zScenes.begin(); it != this->zScenes.end(); it++)
		delete (*it);
}