#include "Scene.h"


Scene::Scene()
{
	camera = 0;
}

void Scene::Initialize(Renderer *r, ObjectFactory *of)
{
	tmpRun = 0;

	if (of == nullptr) {
		throw - 1;
	}
	
	camera = new Camera(D3DXVECTOR3(0, -10, 15), D3DXVECTOR3(-45, 180, 0));

	terrain = new Terrain(100, 100, 10.0f);
	terrain->CreateGraphics(r->GetTerrainRenderer());

	r->GetTerrainRenderer()->SetCamera(camera);

	objectFactory = of;
	renderer = r;
	
	SmaLoader smaLoader;

	GameObject *man = smaLoader.load("Cube.002.sma");	
	RigidBody* manBody = new RigidBody();
	man->SetRigidBody(manBody);
	
	GameObject *arrow = smaLoader.load("arrow.sma");
	RigidBody* arrowBody = new RigidBody();
	arrow->SetRigidBody(arrowBody);

	if (man != nullptr)
		objects.push_back(man);

	if (arrow != nullptr)
		objects.push_back(arrow);
	
	renderer->AddGameObject(man);
	renderer->AddGameObject(arrow);

	renderer->AllocateVideoMemory();
}

void Scene::OnMessage(Message mes)
{
	if (mes.type == "user_input" && mes.name == "left_mouse_button_down") {
		for (GameObject* go : objects) {
			if (go->name == "Arrow") {

				RayVector camRay = camera->GetVectorRay(mes.x, mes.y);
				D3DXVECTOR3 intersection = terrain->GetTerrainRenderer()->GetTerraneIntersection(camRay);
								
				go->SetPosition(intersection);
			}
			if (go->name == "Cube.002") {
				RayVector camRay = camera->GetVectorRay(mes.x, mes.y);
				D3DXVECTOR3 intersection = terrain->GetTerrainRenderer()->GetTerraneIntersection(camRay);

				go->SetCommand(new MoveUnitCommand(go, intersection));
			}
		}
	}

	if (mes.type == "user_input" && mes.name == "a") {
		tmpRun = !tmpRun;
		GameObject *go = GetGameObject("Cube.002");
		tmpRun ? go->StartAnimation("Walk") : go->StopAnimation();
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

GameObject * Scene::GetGameObject(std::string oname)
{
	for (auto go : objects) {
		if (go->GetName() == oname) {
			return go;
		}
	}
	return nullptr;
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
