#include "GameObject.h"

GameObject::GameObject()
{
}

GameObject::~GameObject()
{
}

void GameObject::AddVertex(VertexData vd)
{
	vertexes.push_back(vd);
}

void GameObject::AddIndex(int i)
{
	indexes.push_back(i);
}

