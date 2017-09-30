#include "Scene.h"


Scene::Scene()
{
}

void Scene::Initialize(Renderer *r, ObjectFactory *of)
{
	if (of == nullptr) {
		throw - 1;
	}

	terrain = new Terrain(5, 2, 10.0f);
	terrain->CreateGraphics(r->GetTerrainRenderer());

	objectFactory = of;
	renderer = r;

	
	GameObject *smaMan = objectFactory->LoadSmaFile2("Cube.002.sma");	
	GameObject *axis = objectFactory->LoadObjFile("axis.obj");

	if (smaMan != nullptr)
		objects.push_back(smaMan);
	if (axis != nullptr)
		objects.push_back(axis);

	renderer->SendData(objects);
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

	if (terrain != nullptr) {
		terrain->Clear();
		delete terrain;
	}
}


Scene::~Scene()
{
}
