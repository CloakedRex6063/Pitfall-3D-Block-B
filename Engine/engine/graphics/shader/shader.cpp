#include "shader.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <GLES3/gl3.h>

Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
	Load(vertexPath,fragmentPath);
}

void Shader::Load(const char* vertexPath, const char* fragmentPath)
{
	std::string vertexCode;
	std::string fragmentCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;

	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try
	{
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);

		std::stringstream vShaderStream;
		std::stringstream fShaderStream;

		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();

		vShaderFile.close();
		fShaderFile.close();

		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();

		CreateShader(vertexCode.c_str(), fragmentCode.c_str());
	}
	catch (std::ifstream::failure& e)
	{
		std::cout << "Shader File not successfully" << e.what() << '\n';
	}
}

void Shader::CreateShader(const char* vertexCode, const char* fragmentCode)
{
	const unsigned int vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex,1,&vertexCode,nullptr);
	glCompileShader(vertex);

	CheckCompileErrors(vertex,"VERTEX");

	const unsigned int fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment,1,&fragmentCode,nullptr);
	glCompileShader(fragment);

	CheckCompileErrors(fragment,"FRAGMENT");

	ID = glCreateProgram();
	glAttachShader(ID,vertex);
	glAttachShader(ID,fragment);
	glLinkProgram(ID);

	CheckCompileErrors(ID,"PROGRAM");

	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

void Shader::CheckCompileErrors(unsigned shader, const std::string& type)
{
	int success;
	char infoLog[1024];

	if (type == "PROGRAM")
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
			std::cout << "Shader Compilation Failed " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << '\n';
		}
	}
	else
	{
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(shader, 1024, nullptr, infoLog);
			std::cout << "Shader Program Linking Failed " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << '\n';
		}
	}
}

Shader::~Shader()
{
	glDeleteProgram(ID);
}

void Shader::Bind() const
{
	glUseProgram(ID);
}

void Shader::Unbind()
{
	glUseProgram(0);
}

void Shader::SetBool(const char* name, bool value) const
{
	glUniform1i(GetUniformLocation(name),value);
}

void Shader::SetInt(const char* name, int value) const
{
	glUniform1i(GetUniformLocation(name),value);
}

void Shader::SetFloat(const char* name, float value) const
{
	glUniform1f(GetUniformLocation(name),value);
}

void Shader::SetVec2(const char* name, float x, float y) const
{
	glUniform2f(GetUniformLocation(name),x,y);
}

void Shader::SetVec2(const char* name, Vector2 value) const
{
	glUniform2f(GetUniformLocation(name),value.x,value.y);
}

void Shader::SetVec3(const char* name, float x, float y, float z) const
{
	glUniform3f(GetUniformLocation(name),x,y,z);
}

void Shader::SetVec3(const char* name, Vector3 value) const
{
	glUniform3f(GetUniformLocation(name),value.x,value.y,value.z);
}

void Shader::SetVec4(const char* name, float x, float y, float z, float w) const
{
	glUniform4f(GetUniformLocation(name),x,y,z,w);
}

void Shader::SetVec4(const char* name, glm::vec4 value) const
{
	glUniform4f(GetUniformLocation(name),value.x,value.y,value.z,value.w);
}

void Shader::SetMat4(const char* name, glm::mat4 value) const
{
	glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &value[0][0]);
}

int Shader::GetUniformLocation(const char* name) const
{
	return glGetUniformLocation(ID,name);
}
