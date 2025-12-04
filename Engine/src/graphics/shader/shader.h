#pragma once
#include <unordered_map>
#include "math/vector3.h"
#include "glm/glm.hpp"
class Vector2;

// Referenced from LearnOpenGL.com

class Shader
{
public:
	Shader(const char* vertexPath, const char* fragmentPath);
	void Load(const char* vertexPath, const char* fragmentPath);
	void CreateShader(const char* vertexCode, const char* fragmentCode);
	static void CheckCompileErrors(unsigned shader, const std::string& type);
	~Shader();

	void Bind() const;
	static void Unbind();
	void SetBool(const char* name, bool value) const;
	void SetInt(const char* name, int value) const;
	void SetFloat(const char* name, float value) const;
	void SetVec2(const char* name, float x, float y) const;
	void SetVec2(const char* name, Vector2 value) const;
	void SetVec3(const char* name, float x, float y, float z) const;
	void SetVec3(const char* name, Vector3 value) const;
	void SetVec4(const char* name, float x, float y, float z, float w) const;
	void SetVec4(const char* name, glm::vec4 value) const;
	void SetMat4(const char* name, glm::mat4 value) const;
	int GetUniformLocation(const char* name) const;

private:
	unsigned int ID = 0;
};

