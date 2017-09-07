#include "ObjectFactory.h"


ObjectFactory::ObjectFactory(Renderer *rnd)
{
	if (rnd == nullptr) {
		throw - 1;
	}
	renderer = rnd;
}

ObjectFactory::~ObjectFactory()
{
}

GameObject * ObjectFactory::CreateCube()
{
	GameObject *go = new GameObject();

	CUSTOMVERTEX vertices[] =
	{
		{ -3.0f, -3.0f, 3.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, },    // side 1
		{ 3.0f, -3.0f, 3.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, },
		{ -3.0f, 3.0f, 3.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, },
		{ 3.0f, 3.0f, 3.0f, 0.0f, 0.0f, 1.0f,  1.0f, 1.0f, },

		{ -3.0f, -3.0f, -3.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, },    // side 2
		{ -3.0f, 3.0f, -3.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, },
		{ 3.0f, -3.0f, -3.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, },
		{ 3.0f, 3.0f, -3.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f, },

		{ -3.0f, 3.0f, -3.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, },    // side 3
		{ -3.0f, 3.0f, 3.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, },
		{ 3.0f, 3.0f, -3.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, },
		{ 3.0f, 3.0f, 3.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, },

		{ -3.0f, -3.0f, -3.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, },    // side 4
		{ 3.0f, -3.0f, -3.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, },
		{ -3.0f, -3.0f, 3.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, },
		{ 3.0f, -3.0f, 3.0f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f, },

		{ 3.0f, -3.0f, -3.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, },    // side 5
		{ 3.0f, 3.0f, -3.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, },
		{ 3.0f, -3.0f, 3.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, },
		{ 3.0f, 3.0f, 3.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, },

		{ -3.0f, -3.0f, -3.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, },    // side 6
		{ -3.0f, -3.0f, 3.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, },
		{ -3.0f, 3.0f, -3.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, },
		{ -3.0f, 3.0f, 3.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f, },
	};

	for (int i = 0; i < 24; i++) {
		VertexData vd;
		vd.position.x = vertices[i].X;
		vd.position.y = vertices[i].Y;
		vd.position.z = vertices[i].Z;
		vd.normal.x = vertices[i].NORMAL.x;
		vd.normal.y = vertices[i].NORMAL.y;
		vd.normal.z = vertices[i].NORMAL.z;
		vd.uv.x = vertices[i].tu;
		vd.uv.y = vertices[i].tv;

		go->AddVertex(vd);
	}
	
	//renderer->AddVertexes(vertices, 24);

	//renderer->AddVertexes((CUSTOMVERTEX*)go->GetVertexes(), 24);
		
	short indices[] =
	{
		0, 1, 2,    // side 1
		2, 1, 3,
		4, 5, 6,    // side 2
		6, 5, 7,
		8, 9, 10,    // side 3
		10, 9, 11,
		12, 13, 14,    // side 4
		14, 13, 15,
		16, 17, 18,    // side 5
		18, 17, 19,
		20, 21, 22,    // side 6
		22, 21, 23,
	};
	
	for (int i = 0; i < 24; i++) {		
		go->AddIndex(indices[i]);
	}

	renderer->AddIndexes(go->GetIndexes(), 36);

	renderer->CreateTexture(L"brown-paper.bmp");

	return go;
}
