#include "Scene.h"


Scene::Scene()
{
}

void Scene::Initialize(Renderer *r, ObjectFactory *of)
{
	if (of == nullptr) {
		throw - 1;
	}

	objectFactory = of;
	renderer = r;

	GameObject *cube = objectFactory->CreateCube();
	GameObject *cubeObj = objectFactory->CreateCube();
	
	objects.push_back(cube);
	objects.push_back(cubeObj);
	
	cube->position.x = 3.0f;
	cubeObj->position.x = -3.0f;

	renderer->SendData(objects);

	//renderer->AddVertexes((CUSTOMVERTEX*)cube->GetVertexes(), 24);
	//renderer->AddIndexes(cube->GetIndexes(), 24);
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
