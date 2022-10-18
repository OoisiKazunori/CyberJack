#pragma once
#include"../KazLibrary/Math/KazMath.h"
#include"../Game/UI/Letter.h"
#include"../Game/Event/WindowFlame.h"

class TutorialGame
{
public:
	TutorialGame();
	void Init(const std::string &TEXT, const std::vector<KazMath::Vec3<float>> &POS_ARRAY);
	void Finalize();
	void Update();
	void Draw();

	void Succeed();
	bool NextTutorial();

	size_t GetBoxNum()
	{
		return blockPosArray.size();
	}
	std::vector<KazMath::Vec3<float>> GetBoxPosArray()
	{
		return blockPosArray;
	}

private:
	KazMath::Vec3<float>basePos;
	std::vector<KazMath::Vec3<float>>blockPosArray;
	WindowFlame window;
	String tutorialText;

	bool endFlag;
	int endTimer;
	bool readyToWriteFlag;

	std::string text;
};