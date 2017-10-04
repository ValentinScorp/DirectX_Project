#include "Scene.h"


Scene::Scene()
{
	camera = 0;
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
	

	camera = new Camera(D3DXVECTOR3(0, -10, 15), D3DXVECTOR3(-45, 180, 0));

	SmaLoader smaLoader;

	GameObject *smaMan = smaLoader.load("Cube.002.sma");	
//	GameObject *axis = objectFactory->LoadObjFile("axis.obj");
	GameObject *arrow = smaLoader.load("arrow.sma");

	if (smaMan != nullptr)
		objects.push_back(smaMan);
//	if (axis != nullptr)
//		objects.push_back(axis);
	if (arrow != nullptr)
		objects.push_back(arrow);

	renderer->AttachMesh(smaMan->GetMesh());
	renderer->AttachMesh(arrow->GetMesh());

	renderer->AllocateVideoMemory();

	for (auto o : objects) {
		o->textureId = renderer->CreateTexture(o->texture.c_str());
	}
}

void Scene::OnMessage(Message mes)
{
	if (mes.type == "user_input" && mes.name == "left_mouse_button_down") {
		for (GameObject* go : objects) {
			if (go->name == "Arrow") {

				RayVector camRay = camera->GetVectorRay(mes.x, mes.y);
				D3DXVECTOR3 intersection = terrain->GetTerrainRenderer()->GetTerraneIntersection(camRay);
								
				go->SetCommand(new MoveUnitCommand(go, intersection));
			}
		}
	}
}

void Scene::UpdateUnits(float dt)
{
	for (GameObject *go : objects) {
		go->Update(dt);
	}
}

Camera * Scene::GetActiveCamera()
{
	return camera;
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
	if (camera != 0) {
		delete camera;
	}
}
