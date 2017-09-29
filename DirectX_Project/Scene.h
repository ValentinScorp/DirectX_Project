#pragma once

#include <vector>

#include "GameObject.h"
#include "ObjectFactory.h"
#include "Terrain.h"

class Scene
{
public:
	Scene();

	void Initialize(Renderer *r, ObjectFactory *of);	
	
	void Destroy();

	~Scene();

private:
	Renderer *renderer = nullptr;
	ObjectFactory *objectFactory = nullptr;
	std::vector <GameObject*> objects;

	Terrain *terrain = nullptr;
};

