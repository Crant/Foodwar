#pragma once

#include "ClassUtil.h"
#include <list>
#include "Iw2DSceneGraph.h"
#include "IwTween.h"

using namespace Iw2DSceneGraphCore;
using namespace IwTween;

#define SCENE_MANAGER SceneManager::GetInstance()

class SceneManager;

class Scene : public CNode
{
protected:
	unsigned int	zNameHash;		// Hash string representation of scene name
	bool			zIsActive;		// Active state
	bool			zIsInputActive;	// Input active state
	SceneManager*	zSceneManager;	// Scene manager
	CTweenManager	zTweener;		// Scene local tween manager
	CTweenManager*	zGlobalTweener;	// Global Tween manager
public:
	Scene();
	~Scene();

	bool			IsActive() const						{return this->zIsActive;}
	void			SetActive(bool pActive)					{this->zIsActive = pActive;}
	void			SetName(const char* pName);
	unsigned int	GetNameHash() const						{return this->zNameHash;}
	void			SetManager(SceneManager* pManager)		{this->zSceneManager = pManager;}
	void			SetInputActive(bool pActive)			{this->zIsInputActive = pActive;}
	CTweenManager&	GetTweener()							{return this->zTweener;}
	void			SetGlobalTween(CTweenManager* pTween)	{this->zGlobalTweener = pTween;}
	CTweenManager* GetGlobalTween()							{return this->zGlobalTweener;}

	virtual void OnBackKeyPress() = 0;
	virtual void OnPause() = 0;
	virtual void OnResume() = 0;
	virtual void OnSwap() = 0;
public:
	/**
     * @fn    virtual void Scene::Init();
     *
     * @brief Initialize the scene.
     */
    virtual void    Init();
    /**
     * @fn    virtual void Scene::Update(float pDeltaTime = 0.0f, float pAlphaMul = 1.0f);
     *
     * @brief Update the scene.
     *
     * @brief Updates this scene
     *
     * @param pDeltaTime   Number of seconds that has passed since Update() was last called.
     * @param pAlphaMul    The alpha value being passed down to this node. Default value is 1.
     */
    virtual void    Update(float pDeltaTime = 0.0f, float pAlphaMul = 1.0f);
    /**
     * @fn    virtual void Scene::Render();
     *
     * @brief Render the scene.
     */
    virtual void    Render();
};

class SceneManager
{
private:
	CDEFINE_SINGLETONS(SceneManager)
protected:
	bool				zQuitRequest;
	Scene*				zCurrentScene;	// Currently active scene
	Scene*				zNextScene;		// Next scene (that is being switched to)
	Scene*				zPrevScene;		// Previous scene (that was switched from)
	std::list<Scene*>	zScenes;		// Scenes list
public:
	void Init();
	void Release();

	Scene* GetCurrentScene()	{return this->zCurrentScene;}
	Scene* GetPreviousScene()	{return this->zPrevScene;}

	/**
     * @fn    void SceneManager::SwitchTo(Scene* pScene);
     *
     * @brief Switch to a new scene.
     *
     * @param pScene   Thew new scene.
     */
	 void	SwitchTo(Scene* pScene);

	/**
     * @fn    void SceneManager::Update(float pDeltaTime = 0.0f);
     *
     * @brief Updates all scenes managed by the manager
     *
     * @param pDeltaTime   Number of seconds that has passed since Update() was last called.
     */
	void	Update(float pDeltaTime = 0.0f);
	
	/**
     * @fn    void SceneManager::Render();
     *
     * @brief Renders all scenes managed by the manager
     */
    void    Render();

	/**
     * @fn    void SceneManager::Add(Scene* pScene);
     *
     * @brief Adds a scene to the manager.
     *
     * @param pScene   If non-null, the scene to add.
     */
    void    Add(Scene* pScene);

    /**
     * @fn    void SceneManager::Remove(Scene* pScene);
     *
     * @brief Removes the given scene from the scene manager.
     *
     * @param pScene   If non-null, the scene to remove.
     */
    void    Remove(Scene* pScene);

    /**
     * @fn    Scene* SceneManager::Find(const char* pName);
     *
     * @brief Searches for the named scene.
     *
     * @param pName    The name of the scene to find.
     *
     * @return    null if it fails, else the scene.
     */
    Scene*  Find(const char* pName);

	void OnBackButtonPressed();
	void OnPause();
	void OnResume();

	void QuitGame() {this->zQuitRequest = true;}

	bool QuitRequested() {return this->zQuitRequest;}
	//Internal
	static void OnSwitchComplete(CTween* pTween);
	void	FinishSwitch();
};