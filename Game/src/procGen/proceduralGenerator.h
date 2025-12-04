#pragma once
#include "string"
#include "unordered_map"

class Scene;
class Vector3;

class ProceduralGenerator
{
public:
	ProceduralGenerator(Scene* scene,float minDiff, float maxDiff, int iter);
	~ProceduralGenerator();

	int GetRandomEnemyCount() const;
	float GetRandomEnemyDistance() const;
	void GenerateEnemies(float startZ, float endZ);
	void RemoveActors(float startZ, float endZ) const;

	const int minEnemyCount = 3;
	const int maxEnemyCount = 15;

	const float minDistance = 10;
	const float maxDistance = 100;

	int enemyCount = 0;

	int GetRandomCoinCount() const;
	float GetRandomCoinDistance() const;
	void GenerateCoins(float startZ, float endZ);

	const int minCoins = 10;
	const int maxCoins = 70;

	const float minCoinDistance = 3.f;
	const float maxCoinDistance = 5.f;

	int coinCount = 0;

private:
	Scene* scene = nullptr;
	std::unordered_map<std::string, float> enemyPositions;

	float minDifficulty = 0.4f;
	float maxDifficulty = 0.8f;
	float curDifficulty = 0.4f;
	int iterations = 0;
};

