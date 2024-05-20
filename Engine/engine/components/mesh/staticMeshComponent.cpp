#include "staticMeshComponent.h"
#include "glm/ext/matrix_transform.hpp"
#include "glm/gtc/quaternion.hpp"
#include "engine/actor/actor.h"
#include "engine/components/physics/physicsComponent.h"
#include "engine/framework/gameInstance.h"
#include "engine/graphics/model/staticModel.h"
#include "engine/math/vector3.h"
#include "engine/util/util.h"
#include "glm/gtx/euler_angles.hpp"

StaticMeshComponent::StaticMeshComponent(Actor* owner) : Component(owner)
{

}

StaticMeshComponent::~StaticMeshComponent()
{
}

void StaticMeshComponent::Init()
{
	physicsComponent = GetOwner()->GetComponent<PhysicsComponent>();
}

void StaticMeshComponent::Tick(float deltaTime)
{
}

void StaticMeshComponent::Render()
{
	if (physicsComponent)
	{
		const auto transform = physicsComponent->GetTransform();

		const auto position = Vector3(transform.getOrigin()); //+ model->offset;
		const auto rotation = transform.getRotation();

		modelMat = glm::mat4(1.0f);
		modelMat = glm::translate(modelMat, glm::vec3(position));
		const auto rotationMat = glm::mat4_cast(glm::quat(rotation.getW(), rotation.getX(), rotation.getY(), rotation.getZ()));
		modelMat *= rotationMat;
		const auto additionalRotationMat = glm::eulerAngleYXZ(rot.z, rot.y, rot.x);
		modelMat *= additionalRotationMat;
		modelMat = glm::scale(modelMat, glm::vec3(1,1,1));

		if (bInstanced)
		{
			const auto modelManager = Util::GetGameInstance()->GetModelManager();
			for (const auto& modelMap : modelManager->models)
			{
				if (modelMap.second == model)
				{
					modelManager->modelMatrices[modelMap.first].push_back(modelMat);
				}
			}
		}
		else
		{
			Util::GetBasicShader()->SetMat4("model", modelMat);
			model->Draw(*Util::GetBasicShader());
		}
	}
}

void StaticMeshComponent::SetModel(StaticModel* newModel, Vector3 pos)
{
	model = newModel;
	if (physicsComponent)
	{
		physicsComponent->CreateRigidBody(model->GetBoxCollider(),pos);
	}
}

glm::mat4 StaticMeshComponent::GetModelMatrix() const
{
	return modelMat;
}
