#include "Scene.h"


Scene::Scene()
{
}

void Scene::Initialize(ObjectFactory *of)
{
	if (of == nullptr) {
		throw - 1;
	}

	objectFactory = of;

	GameObject *cube = objectFactory->CreateCube();

	objects.push_back(cube);
}

void Scene::Destroy()
{
	for (GameObject *object : objects) {
		if (object != nullptr) {
			delete object;
		}
		object = nullptr;
	}
	objects.clear();
}


Scene::~Scene()
{
}
