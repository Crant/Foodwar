#pragma once

#include "Iw2DSceneGraph.h"

class Object
{
protected:
	float zHealth;
	float zVelocity;

public:
	virtual void Render() = 0;

	virtual void Update(float pDt, float pAlphaMul) = 0;
	/**
	* @fn    void Object::Init(Iw2DSceneGraph::CSprite* pSprite, float pVel)
	*
	* @brief Inits the class with the sprite and at what velocity it will move with
	*/
	virtual void Init(Iw2DSceneGraph::CSprite* pSprite, float pVel) = 0;

	float GetVelocity() { return this->zVelocity; }
	float GetHealth() { return this->zHealth; }

	void SetVelocity(float pVel) { this->zVelocity = pVel; }
	void SetHealth(float pHealth) { this->zHealth = pHealth; }

};