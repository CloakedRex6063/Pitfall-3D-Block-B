#include "skeletalMeshComponent.h"
#include "actor/actor.h"
#include "components/controller/controllerComponent.h"
#include "components/physics/physicsComponent.h"
#include "graphics/animator/animator.h"
#include "graphics/model/skeletalModel.h"
#include "util/util.h"

SkeletalMeshComponent::SkeletalMeshComponent(Actor* owner) : Component(owner) 
{

}

SkeletalMeshComponent::~SkeletalMeshComponent()
{
}

void SkeletalMeshComponent::Init()
{
	animator = new Animator();
	physicsComponent = GetOwner()->GetComponent<PhysicsComponent>();
}

void SkeletalMeshComponent::Tick(float deltaTime)
{
	animator->UpdateAnimation(deltaTime);
}

void SkeletalMeshComponent::Render()
{
	const auto transforms = animator->GetFinalBoneMatrices();
	for (unsigned i = 0; i < transforms.size(); ++i)
	{
		auto name = "finalBonesMatrices[" + std::to_string(i) + "]";
		Util::GetBasicShader()->SetMat4(name.c_str(), transforms[i]);
	}
	if (model)
	{
		if (physicsComponent)
		{
			const auto transform = physicsComponent->GetTransform();

			const auto position = Vector3(transform.getOrigin()) + offset;
			const auto rotation = transform.getRotation();

			auto modelMat = glm::mat4(1.0f);
			modelMat = glm::translate(modelMat, glm::vec3(position));
			const auto rotationMat = glm::mat4_cast(glm::quat(rotation.getW(), rotation.getX(), rotation.getY(), rotation.getZ()));
			modelMat *= rotationMat;
			modelMat = glm::scale(modelMat, glm::vec3(1.f, 1.f, 1.f));	
			Util::GetBasicShader()->SetMat4("model", modelMat);
			model->Draw();
		}
		else if(controllerComponent)
		{
			const auto transform = controllerComponent->GetTransform();

			const auto position = Vector3(transform.getOrigin()) + offset;
			const auto rotation = transform.getRotation();

			auto modelMat = glm::mat4(1.0f);
			modelMat = glm::translate(modelMat, glm::vec3(position));
			const auto rotationMat = glm::mat4_cast(glm::quat(rotation.getW(), rotation.getX(), rotation.getY(), rotation.getZ()));
			modelMat *= rotationMat;
			modelMat = glm::scale(modelMat, glm::vec3(1.f, 1.f, 1.f));
			Util::GetBasicShader()->SetMat4("model", modelMat);
			model->Draw();
		}
	}
}

void SkeletalMeshComponent::AddAnimation(const std::string& path, const std::string& name) const
{
	const auto anim = new Animation(path,GetModel());
	animator->AddAnimation(anim, name);
}

void SkeletalMeshComponent::PlayAnimation(const std::string& name, const bool bNewLoop, float animSpeed, bool bYLock, float newOffset) const
{
	animator->PlayAnimation(name, bNewLoop, animSpeed, bYLock, newOffset);
}

std::string SkeletalMeshComponent::GetCurrentAnimation() const
{
	return animator->GetCurrentAnimation();
}

void SkeletalMeshComponent::SetModel(SkeletalModel* newModel)
{
	model = newModel;
	if (physicsComponent)
	{
		physicsComponent->CreateRigidBody(model->GetBoxCollider());
	}
}

SkeletalModel* SkeletalMeshComponent::GetModel() const
{
	return model;
}
