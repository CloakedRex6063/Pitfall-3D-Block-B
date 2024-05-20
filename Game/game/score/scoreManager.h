#pragma once

class ScoreManager
{
public:
	static void SaveScore(int score);
	static void LoadScores();
	static int GetScore();
	static int GetHighScore();
	static int GetTotalScore();
	static void UpdateScores();
	static bool GetNewHighScore();

private:
	static int score;
	static int highScore;
	static int totalScore;
};

