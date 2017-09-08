#pragma once

class Vector3Dd
{
public:
	Vector3Dd()
	{

	}
	~Vector3Dd()
	{

	}

	double x = 0;
	double y = 0;
	double z = 0;
};

class Vector3Df
{
public:
	Vector3Df()
	{

	}
	Vector3Df(float ix, float iy, float iz) {
		x = ix; y = iy; z = iz;
	}
	~Vector3Df()
	{

	}

	float x = 0;
	float y = 0;
	float z = 0;
};

class Vector3Di
{
public:
	Vector3Di()
	{

	}
	~Vector3Di()
	{

	}

	int x = 0;
	int y = 0;
	int z = 0;
};

class Vector2Df
{
public:
	Vector2Df()
	{

	}
	Vector2Df(float ix, float iy) {
		x = ix; y = iy;
	}
	~Vector2Df()
	{

	}

	float x = 0;
	float y = 0;	
};