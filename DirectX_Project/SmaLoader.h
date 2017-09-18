#pragma once
#pragma once

#include <string>
#include <fstream>
#include <iterator>
#include <vector>

#include "Vector3D.h"
#include "GameObject.h"


class SmaLoader
{
public:
	SmaLoader();

	GameObject* load(std::string file);

	~SmaLoader();
};

