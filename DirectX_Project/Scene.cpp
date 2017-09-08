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

	//GameObject *cube = objectFactory->CreateCube();
	GameObject *cubeObj = objectFactory->LoadObjFile("cube.obj");
	GameObject *roman = objectFactory->LoadObjFile("roman02.obj");
	roman->texture = L"roman_tex_high.png";
	
	objects.push_back(cubeObj);
	objects.push_back(roman);
	
	
	cubeObj->position.x = -3.0f;

	roman->position.z = 10.0f;

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
}


Scene::~Scene()
{
}
