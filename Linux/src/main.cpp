#include "game/framework/pitfallGameInstance.h"
#include "input/input.h"
#include "renderer/renderer.h"
#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include <GLES3/gl3.h>
#include "engine/util/common.h"
#include "engine/graphics/shader/shader.h"
#include "engine/camera/camera.h"
#include "engine/managers/physics/physicsManager.h"
#include "engine/util/util.h"
#include "filesystem"
#include "glm/gtc/type_ptr.hpp"
#include "chrono"
#include "engine/graphics/model/skeletalModel.h"
#include "game/procGen/randomName.h"
#include "engine/components/mesh/skeletalMeshComponent.h"
#include "engine/graphics/light/pointLight.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

void InitImGUI()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::SetCurrentContext(ImGui::GetCurrentContext());
	ImGuiIO& io = ImGui::GetIO();
	io.DisplaySize.x = SCRWIDTH;
	io.DisplaySize.y = SCRHEIGHT;
	ImGui::StyleColorsDark();
}

void CreateBasicShader()
{
	// Create Shader
	const auto basicShader = new Shader("Linux/shaders/basic.vert", "Linux/shaders/basic.frag");
	Util::SetBasicShader(basicShader);
}

void CreateInstanceShader()
{
	const auto instanceShader = new Shader("Linux/shaders/instance.vert", "Linux/shaders/instance.frag");
	Util::SetInstanceShader(instanceShader);
}

unsigned int CreateUBO()
{
	unsigned int ubo;
	glGenBuffers(1, &ubo);
	glBindBuffer(GL_UNIFORM_BUFFER, ubo);
	glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), nullptr, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	glBindBufferRange(GL_UNIFORM_BUFFER, 0, ubo, 0, 2 * sizeof(glm::mat4));
	return ubo;
}


void UpdatePosition(Vector3& pos, float deltaTime, float& angle, float speed, float radius, Vector3 axis)
{
	const Vector3 offset = Vector3(std::cos(angle) * radius,std::sin(angle) * radius,0.f);
	// Update the position
	pos = axis * offset;

	// Increment the angle based on time and speed
	angle += speed * deltaTime;

	// Wrap the angle to keep it within a full circle
	if (angle > 2.f * static_cast<float>(M_PI))
	{
		angle -= 2.f * static_cast<float>(M_PI);
	}
}

int main()
{
	stbi_set_flip_vertically_on_load(true);

	InitImGUI();
	const auto input = new Input();
	const auto window = new GameWindow("Pi Sucks", SCRWIDTH, SCRHEIGHT);
	window->SetKeyCallbacks([&input](XKeyEvent& keyEvent) {input->KeyPressed(keyEvent); },
		[&input](XKeyEvent& keyEvent) {input->KeyReleased(keyEvent);  });
	window->SetMouseCallbacks([&input](const XButtonEvent& buttonEvent) { input->MousePressed(buttonEvent); },
		[&input](const XButtonEvent& buttonEvent) { input->MouseReleased(buttonEvent); },
		[&input](XMotionEvent& motionEvent) { input->MouseMoved(motionEvent); });

	const auto renderer = new OpenGL(window);
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glClearColor(0.2f, 0.3f, 0.4f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0,0,SCRWIDTH,SCRHEIGHT);
	const auto camera = new Camera(glm::vec3(0, 0, -3));

	const auto ubo = CreateUBO();
	CreateBasicShader();
	CreateInstanceShader();

	const auto gameInstance = new PitfallGameInstance();

	input->SetInput(gameInstance->GetInputManager());
	window->StartHandlingInput();

	ImGui_ImplOpenGL3_Init("#version 300 es");

	std::chrono::steady_clock::time_point prevTime = std::chrono::steady_clock::now();

	srand(static_cast <unsigned> (time(nullptr)));

	glBindBuffer(GL_UNIFORM_BUFFER, ubo);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(Util::GetProjection()));
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	std::vector<Vector3> positions {0.f,0.f,0.f};
	std::vector angle {0.f,0.f,0.f};

	while (window->IsRunning())
	{
		ImGuiIO& io = ImGui::GetIO();
		ImGui_ImplOpenGL3_NewFrame();
		ImGui::NewFrame();

		std::chrono::steady_clock::time_point currentTime = std::chrono::steady_clock::now();
		const float deltaTime = std::chrono::duration<float>(currentTime - prevTime).count();
		io.DeltaTime = deltaTime;
		prevTime = currentTime;

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glBindBuffer(GL_UNIFORM_BUFFER, ubo);
		glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(Util::GetView()));
		glBindBuffer(GL_UNIFORM_BUFFER, 0);

		Util::GetBasicShader()->Bind();
		Util::GetBasicShader()->SetVec3("viewPos", camera->Position);
		Util::GetBasicShader()->SetVec3("light.position", 0,3,0);
		Util::GetBasicShader()->SetVec3("light.ambient", 0.2f, 0.2f, 0.2f);
        Util::GetBasicShader()->SetVec3("light.diffuse", 0.5f, 0.5f, 0.5f);
        Util::GetBasicShader()->SetVec3("light.specular", 1.0f, 1.0f, 1.0f);

		PointLight(0,positions[0],Vector3(0.05f),Vector3(0.8f),Vector3(1.f),1.f,0.09f,0.032f,1.f);
		PointLight(1,positions[1],Vector3(0.5f,0.f,0.5f),Vector3(0.5f,0.f,0.5f),Vector3(0.5f,0.f,0.5f),1.f,
			0.09f,0.032f,2.f);
		PointLight(2, positions[2], Vector3(0.4f, 0.7f, 0.1f), Vector3(0.4f, 0.7f, 0.1f), Vector3(0.4f, 0.7f, 0.1f),
		           1.f, 0.09f, 0.032f, 1.f);

		UpdatePosition(positions[0],deltaTime,angle[0],1.f,3.f,Vector3(0,1,0));
		UpdatePosition(positions[1],deltaTime,angle[1],5.f,5.f,Vector3(1,0,0));
		UpdatePosition(positions[2],deltaTime,angle[2],3.f,4.f,Vector3(0,0,1));

		gameInstance->Tick(deltaTime);
		gameInstance->Render();

		ImGui::Begin("Debug");
		ImGui::Text("FPS: %f", 1.0f / deltaTime);
		ImGui::Text("DeltaTime: %f", deltaTime);
		ImGui::Checkbox("Debug Physics", &Util::GetGameInstance()->GetPhysicsManager()->bDrawDebug);
		ImGui::End();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		renderer->SwapBuffers();

		ImGui::EndFrame();
	}
	delete camera;
	delete gameInstance;
	delete Util::GetBasicShader();
	window->DestroyWindow();
	delete input;
	delete renderer;
	delete window;
}
