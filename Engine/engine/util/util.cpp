#include "engine/graphics/shader/shader.h"
#include "engine/camera/camera.h"
#include "engine/framework/gameInstance.h"
#include "util.h"

#include <functional>
#include <stb_image.h>
#include <GLES3/gl3.h>

#include "common.h"

Shader* Util::basicShader = nullptr;
Shader* Util::instanceShader = nullptr;
GameInstance* Util::gameInstance = nullptr;
Camera* Util::camera = nullptr;

Shader* Util::GetBasicShader()
{
	return basicShader;
}

void Util::SetBasicShader(Shader* newShader)
{
	basicShader = newShader;
}

Shader* Util::GetInstanceShader()
{
	return instanceShader;
}

void Util::SetInstanceShader(Shader* newShader)
{
	instanceShader = newShader;
}

GameInstance* Util::GetGameInstance()
{
	return gameInstance;
}

void Util::SetGameInstance(GameInstance* newGameInstance)
{
	gameInstance = newGameInstance;
}

Camera* Util::GetCamera()
{
	return camera;
}

void Util::SetCamera(Camera* newCamera)
{
	camera = newCamera;
}

glm::mat4 Util::GetProjection()
{
	return glm::perspective(glm::radians(camera->Zoom), SCRWIDTH / SCRHEIGHT, 0.1f, 100.0f);
}

glm::mat4 Util::GetView()
{
	return camera->GetViewMatrix();
}

unsigned Util::TextureFromFile(const char* path, const std::string& directory, Vector2 scale)
{
	std::string filename = std::string(path);
	filename = directory + '/' + filename;

	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
	width = static_cast<int>(scale.x * static_cast<float>(width));
	height = static_cast<int>(scale.y * static_cast<float>(height));
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 2)
			format = GL_RG;
		else if (nrComponents == 3)
			format = GL_RGB;
		else
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << '\n';
		stbi_image_free(data);
	}

	return textureID;
}

unsigned Util::LoadCubemap(const std::vector<std::string>& faces)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	stbi_set_flip_vertically_on_load(false);

	int width, height, nrChannels;
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Cubemap texture failed to load at path: " << faces[i] << '\n';
			stbi_image_free(data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	stbi_set_flip_vertically_on_load(true);

	return textureID;
}

