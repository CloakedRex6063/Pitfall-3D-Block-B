#include "pointLight.h"

#include "graphics/shader/shader.h"
#include "util/util.h"

PointLight::PointLight(int num, Vector3 pos, Vector3 amb, Vector3 diff, Vector3 spec, float constant, float linear,
                       float quadratic, float intensity)
{
	SetProperty(num,"position",pos);
	SetProperty(num,"ambient",amb * intensity);
	SetProperty(num,"diffuse",diff * intensity);
	SetProperty(num,"specular",spec * intensity);
	SetProperty(num,"constant",constant);
	SetProperty(num,"linear",linear);
	SetProperty(num,"quadratic",quadratic);
}

void PointLight::SetProperty(int num, const std::string& prop, Vector3 val)
{
	const std::string name = "pointLights[" + std::to_string(num) + "]." + prop;
    Util::GetBasicShader()->SetVec3(name.c_str(),val);
}

void PointLight::SetProperty(int num, const std::string& prop, float val)
{
	const std::string name = "pointLights[" + std::to_string(num) + "]." + prop;
	Util::GetBasicShader()->SetFloat(name.c_str(), val);
}

