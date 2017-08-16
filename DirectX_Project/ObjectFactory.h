#pragma once

#include "GameObject.h"
#include "Renderer.h"

class ObjectFactory
{
public:
	ObjectFactory(Renderer *rnd);
	~ObjectFactory();

	GameObject* CreateCube();

private:

	Renderer *renderer = nullptr;
};

