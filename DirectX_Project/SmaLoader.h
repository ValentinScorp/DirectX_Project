#pragma once
#pragma once

#include <string>
#include <fstream>
#include <iterator>
#include <vector>

#include "GameObject.h"


class SmaLoader
{
public:
	SmaLoader();

	GameObject* load(std::string file);

	~SmaLoader();
};

