#pragma once

#include <vector>

#include "GameObject.h"
#include "ObjectFactory.h"

class Scene
{
public:
	Scene();

	void Initialize(ObjectFactory *of);
	
	
	void Destroy();

	~Scene();

private:
	ObjectFactory *objectFactory = nullptr;
	std::vector <GameObject*> objects;
};

