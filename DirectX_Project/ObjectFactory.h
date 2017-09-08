#pragma once

#include "GameObject.h"
#include "Renderer.h"

class ObjectFactory
{
public:
	ObjectFactory(Renderer *rnd);
	~ObjectFactory();

	GameObject* CreateCube();

	GameObject* LoadObjFile(std::string fileName);

private:

	Renderer *renderer = nullptr;
};

