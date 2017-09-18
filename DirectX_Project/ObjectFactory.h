#pragma once

#include "GameObject.h"
#include "Renderer.h"
#include "SmaLoader.h"

class ObjectFactory
{
public:
	ObjectFactory(Renderer *rnd);
	~ObjectFactory();

	GameObject* CreateCube();

	GameObject* LoadObjFile(std::string fileName);
	GameObject* LoadSmaFile(std::string fileName);
	GameObject* LoadSmaFile2(std::string fileName);

private:

	Renderer *renderer = nullptr;
	SmaLoader smaLoader;
};

