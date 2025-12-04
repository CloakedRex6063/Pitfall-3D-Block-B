#pragma once
class Shader;

// Referenced from https://learnopengl.com/Advanced-OpenGL/Cubemaps

class Skybox
{
public:
	Skybox(Shader* skybox);
	~Skybox();
	void Draw() const;

private:
	Shader* skyboxShader;
	unsigned int skyboxVAO;
	unsigned int skyboxVBO;
	unsigned int cubeMapTexture;
};

