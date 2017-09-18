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

	GameObject *cubeSma = objectFactory->LoadSmaFile("cube.sma");
	
	GameObject *smaMan = objectFactory->LoadSmaFile2("Cube.002.sma");	

	roman->texture = L"roman_tex_high.png";
	cubeSma->texture = L"roman_tex_high.png";
	
	if (cubeObj != nullptr)
		objects.push_back(cubeObj);
	if (roman != nullptr)
		objects.push_back(roman);	
	if (cubeSma != nullptr)
		objects.push_back(cubeSma);
	if (smaMan != nullptr)
		objects.push_back(smaMan);

	cubeObj->position.x = -3.0f;
	roman->position.z = 10.0f;
	cubeSma->position.z = 12.0f;
	cubeSma->position.x = 3.0f;

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
