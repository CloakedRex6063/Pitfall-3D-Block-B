#include "modelManager.h"

#include "imgui.h"
#include "math/vector3.h"
#include "util/util.h"

void ModelManager::AddModel(const std::string& name, const std::string& path, Vector3 scale, Vector3 offset, Vector3 rot, Vector2 texScale)
{
	models[name] = new StaticModel(path,scale,offset,rot,texScale);
}

StaticModel* ModelManager::GetModel(const std::string& name)
{
	return models[name];
}

void ShowMat4Vector(const char* label, const std::vector<glm::mat4>& matrices)
{
    ImGui::Text("%s", label);

    for (size_t i = 0; i < matrices.size(); ++i) 
    {
        ImGui::Text("Matrix %zu:", i + 1);
        for (int row = 0; row < 4; ++row) 
        {
            for (int col = 0; col < 4; ++col) 
            {
                ImGui::SameLine();
                ImGui::Text("%.2f", matrices[i][row][col]);
            }
            ImGui::NewLine();
        }
    }
}

void ModelManager::Render()
{
	Util::GetInstanceShader()->Bind();

    for (auto& modelEntry : modelMatrices) 
    {
        const std::string& modelName = modelEntry.first;
        std::vector<glm::mat4>& matrices = modelEntry.second;

        for (auto& modelMatrix: matrices)
        {
            modelMatrix = glm::translate(modelMatrix, glm::vec3(models[modelName]->offset));
            const float roll = glm::radians(models[modelName]->rot.x);
            const float pitch = glm::radians(models[modelName]->rot.y);
            const float yaw = glm::radians(models[modelName]->rot.z);
            modelMatrix = glm::rotate(modelMatrix, yaw, glm::vec3(0.0f, 0.0f, 1.0f)); 
            modelMatrix = glm::rotate(modelMatrix, pitch, glm::vec3(0.0f, 1.0f, 0.0f)); 
            modelMatrix = glm::rotate(modelMatrix, roll, glm::vec3(1.0f, 0.0f, 0.0f)); 
            modelMatrix = glm::scale(modelMatrix, glm::vec3(models[modelName]->scale));
        }
    	// Check if the model exists
        if (models.find(modelName) != models.end()) 
        {
            // Setup instanced mesh and draw
            models[modelName]->SetupInstancedMesh(matrices);
            models[modelName]->DrawInstanced(matrices.size());

            // Clear matrices for the current model
            modelMatrices[modelName].clear();
        }
    }
}
