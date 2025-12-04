#pragma once
#include "math/vector3.h"

class PointLight
{
public:
	PointLight(int num, Vector3 pos, Vector3 amb, Vector3 diff, Vector3 spec, float constant, float linear,
	           float quadratic, float intensity);
	void SetProperty(int num, const std::string& prop, Vector3 val);
	void SetProperty(int num, const std::string& prop, float val);
};

