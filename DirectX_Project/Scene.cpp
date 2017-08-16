#include "Scene.h"


Scene::Scene()
{
}

void Scene::Initialize(ObjectFactory *of)
{
	if (of == nullptr) {
		throw - 1;
	}

	GameObject *cube = new GameObject();

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
