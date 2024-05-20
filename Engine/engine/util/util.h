#pragma once
#include <assimp/scene.h>

#include "glm/glm.hpp"
#include "string"
#include "vector"
#include "engine/math/vector2.h"

class Shader;
class GameInstance;
class Camera;

class Util
{
public:
	static Shader* GetBasicShader();
	static void SetBasicShader(Shader* newShader);

	static Shader* GetInstanceShader();
	static void SetInstanceShader(Shader* newShader);

private:
	static Shader* basicShader;
	static Shader* instanceShader;

public:
	static GameInstance* GetGameInstance();
	static void SetGameInstance(GameInstance* newGameInstance);

private:
	static GameInstance* gameInstance;

public:
	static Camera* GetCamera();
	static void SetCamera(Camera* newCamera);

	static glm::mat4 GetProjection();
	static glm::mat4 GetView();

private:
	static Camera* camera;

public:
	static unsigned int TextureFromFile(const char *path, const std::string &directory, Vector2 scale = Vector2(1,1));
	static unsigned int LoadCubemap(const std::vector<std::string>& faces);
};

