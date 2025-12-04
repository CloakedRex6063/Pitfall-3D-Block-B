#include "scoreManager.h"
#include "fstream"

int ScoreManager::score = 0;
int ScoreManager::highScore = 0;
int ScoreManager::totalScore = 0;

void ScoreManager::SaveScore(int newScore)
{
	score = newScore;
}

void ScoreManager::LoadScores()
{
	std::ifstream file;
	file.open("scores.txt");
	if (file.is_open())
	{
		file >> highScore;
		file >> totalScore;
		file.close();
	}
	else
	{
		highScore = 0;
		totalScore = 0;
	}
}

int ScoreManager::GetScore()
{
	return score;	
}

int ScoreManager::GetHighScore()
{
	return highScore;
}

int ScoreManager::GetTotalScore()
{
	return totalScore;
}

void ScoreManager::UpdateScores()
{
	if (score > highScore)
	{
		highScore = score;
	}
	totalScore += score;
	std::ofstream file;
	file.open("scores.txt");
	if (file.is_open())
	{
		file << highScore << std::endl;
		file << totalScore << std::endl;
		file.close();
	}
}

bool ScoreManager::GetNewHighScore()
{
	const bool value = score > highScore;
	UpdateScores();
	return value;
}
