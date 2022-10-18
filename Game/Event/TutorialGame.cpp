#include"TutorialGame.h"

TutorialGame::TutorialGame()
{
	endFlag = false;
}

void TutorialGame::Init(const std::string &TEXT, const std::vector<KazMath::Vec3<float>> &POS_ARRAY)
{
	blockPosArray = POS_ARRAY;
	KazMath::Transform3D lTrans;
	lTrans.pos = { WIN_X / 2.0f,100.0f,0.0f };
	KazMath::Vec2<float> lScale;
	lScale = { 3.0f,0.8f };
	window.Init(lTrans, lScale, false, WINDOW_2D);

	endFlag = false;
	endTimer = 0;
	window.Start();
	readyToWriteFlag = false;

	text = TEXT;
}

void TutorialGame::Finalize()
{
	tutorialText.Finalize();
	window.End();
}

void TutorialGame::Update()
{

	if (window.ReadyToWrite() && !readyToWriteFlag)
	{
		tutorialText.Init({ WIN_X / 2.0f - 158.0f,100.0f }, text, 1.4f);
		readyToWriteFlag = true;
	}

	tutorialText.Update(0);
	window.Update();

	if (!endFlag)
	{
		return;
	}
	++endTimer;

	if (KazMath::ConvertSecondToFlame(2) <= endTimer)
	{
		window.End();
		tutorialText.Finalize();
	}
}

void TutorialGame::Draw()
{
	tutorialText.Draw();
	window.Draw();
}

void TutorialGame::Succeed()
{
	if (endFlag)
	{
		return;
	}
	tutorialText.Finalize();
	tutorialText.Init({ WIN_X / 2.0f - 120.0f,100.0f }, "Succeed", 1.4f);
	endFlag = true;
}

bool TutorialGame::NextTutorial()
{
	return endFlag && window.IsEnd();
}