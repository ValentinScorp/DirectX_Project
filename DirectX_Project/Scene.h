#pragma once

#include <vector>

#include "GameObject.h"
#include "ObjectFactory.h"
#include "Terrain.h"

class Scene : public MessageReceiver
{
public:
	Scene();

	void Initialize(Renderer *r, ObjectFactory *of);	
	
	void OnMessage(Message mes);

	void UpdateUnits(float dt);
	Camera* GetActiveCamera();

	void Destroy();

	~Scene();

private:
	Renderer *renderer = nullptr;
	ObjectFactory *objectFactory = nullptr;
	std::vector <GameObject*> objects;

	Terrain *terrain = nullptr;

	Camera *camera;
};

