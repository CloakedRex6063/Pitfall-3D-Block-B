#include "proceduralGenerator.h"

#include <random>

#include "components/mesh/staticMeshComponent.h"
#include "graphics/model/staticModel.h"
#include "scene/scene.h"
#include "actors/enemy.h"
#include "string"
#include "components/physics/physicsComponent.h"
#include "framework/gameInstance.h"
#include "util/util.h"
#include "actors/collectable.h"

ProceduralGenerator::ProceduralGenerator(Scene* scene, float minDiff, float maxDiff, int iter) :
    scene(scene), minDifficulty(minDiff), maxDifficulty(maxDiff), curDifficulty(minDiff), iterations(iter)
{
	
}

int ProceduralGenerator::GetRandomEnemyCount() const
{
	const int adjustedMinEnemyCount = minEnemyCount + static_cast<int>(static_cast<float>((maxEnemyCount - minEnemyCount)) * (1.0f - curDifficulty));
    const int adjustedMaxEnemyCount = maxEnemyCount;

    const int calculatedEnemyCount = adjustedMinEnemyCount + std::rand() % (adjustedMaxEnemyCount - adjustedMinEnemyCount + 1);

    return calculatedEnemyCount;
}

float ProceduralGenerator::GetRandomEnemyDistance() const
{
    const float adjustedMaxDistance = maxDistance * (1.0f - curDifficulty);
    const float randValue = static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX);

	const float calculatedDistance = minDistance + randValue * (adjustedMaxDistance - minDistance);
	return calculatedDistance;
}

void ProceduralGenerator::GenerateEnemies(float startZ, float endZ)
{
    curDifficulty += (maxDifficulty - minDifficulty) / static_cast<float>(iterations);
    curDifficulty = std::min(curDifficulty, maxDifficulty);
	const int numEnemies = GetRandomEnemyCount();
	constexpr float xPositions[] = { -2.5f, 0.0f, 2.5f };

    float prevZ = startZ;
	for (int i = 0; i < numEnemies; i++)
	{
		const float distance = GetRandomEnemyDistance();
		const float z = prevZ + distance;
        prevZ = z;

		if (z < endZ)
		{
			const float randomX = xPositions[rand() % 3];

			enemyCount++;
			const auto name = "Enemy" + std::to_string(enemyCount);
			const auto model = Util::GetGameInstance()->GetModelManager()->GetModel("Rock");
			scene->AddActor(name, new Enemy());
			scene->GetActor(name)->GetComponent<StaticMeshComponent>()->SetModel(model,
				Vector3(randomX, 1, z));
		}
	}
}

void ProceduralGenerator::RemoveActors(float startZ, float endZ) const
{
	for (const auto& actor : scene->GetActors())
	{
		if (actor.second->HasTag("Character")) continue;
        if (actor.second->HasTag("Floor")) continue;
		const auto& transform = actor.second->GetComponent<PhysicsComponent>()->GetTransform();
		const auto& origin = transform.getOrigin();
		const float z = origin.getZ();

		if (z > startZ && z < endZ)
		{
			scene->RemoveActor(actor.first);
		}
	}
}

int ProceduralGenerator::GetRandomCoinCount() const
{
	return minCoins + (rand() % (maxCoins - minCoins + 1));
}


float ProceduralGenerator::GetRandomCoinDistance() const
{
	return minCoinDistance + static_cast <float> (rand()) / (static_cast<float>(RAND_MAX) / (maxCoinDistance - minCoinDistance));
}

void ProceduralGenerator::GenerateCoins(float startZ, float endZ)
{
    const int numCoins = GetRandomCoinCount();
    constexpr float xPositions[] = { -2.5f, 0.0f, 2.5f };

    float prevZ = startZ;
    for (int i = 0; i < numCoins; i++)
    {
        const float distance = GetRandomCoinDistance();
        const float z = prevZ + distance;
        prevZ = z;

        if (z < endZ)
        {
            const float randomX = xPositions[rand() % 3];

            // Check proximity with enemies
            bool isTooCloseToEnemy = false;

            for (const auto& enemy : scene->GetActorsWithTag("Enemy"))
            {
                const float enemyZ = enemy->GetComponent<PhysicsComponent>()->GetTransform().getOrigin().getZ();
                constexpr float enemyDistanceThreshold = 5.0f;

                if (abs(z - enemyZ) < enemyDistanceThreshold)
                {
                    isTooCloseToEnemy = true;
                    break;
                }
            }

            if (!isTooCloseToEnemy)
            {
                coinCount++;
                const auto coinName = "Coin" + std::to_string(coinCount);
                const auto coinModel = Util::GetGameInstance()->GetModelManager()->GetModel("Coin");
                scene->AddActor(coinName, new Collectable());
                scene->GetActor(coinName)->GetComponent<StaticMeshComponent>()->SetModel(coinModel,
                    Vector3(randomX, 1, z));
            }
        }
    }
}


