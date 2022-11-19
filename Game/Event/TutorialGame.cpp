#include"TutorialGame.h"

TutorialGame::TutorialGame()
{
	endFlag = false;
}

void TutorialGame::Init(const std::string &TEXT, const std::vector<KazMath::Vec3<float>> &POS_ARRAY)
{
	blockPosArray = POS_ARRAY;
	KazMath::Transform3D lTrans;
	lTrans.pos = { WIN_X / 2.0f,150.0f,0.0f };
	KazMath::Vec2<float> lScale;
	lScale = { 5.0f,1.3f };
	window.Init(lTrans, lScale, false, WINDOW_2D);

	endFlag = false;
	endTimer = 0;
	window.Start();
	readyToWriteFlag = false;

	std::string lText;
	int lCount = 0;
	for (int i = 0; i < TEXT.size(); ++i)
	{
		if ('\n' == TEXT[i])
		{
			text.push_back(lText);
			lText = {};
			++lCount;
			tutorialText.push_back(std::make_unique<String>());
		}
		else
		{
			lText += TEXT[i];
		}
	}
	text.push_back(lText);
	tutorialText.push_back(std::make_unique<String>());

}

void TutorialGame::Finalize()
{
	for (int i = 0; i < tutorialText.size(); ++i)
	{
		tutorialText[i]->Finalize();
	}
	window.End();
}

void TutorialGame::Update()
{
	if (window.ReadyToWrite() && !readyToWriteFlag)
	{
		for (int i = 0; i < tutorialText.size(); ++i)
		{
			tutorialText[i]->Init({ WIN_X / 2.0f - 158.0f,125.0f }, text[i], 3.0f);
		}
		readyToWriteFlag = true;
	}
	for (int i = 0; i < tutorialText.size(); ++i)
	{
		tutorialText[i]->Update(i * 3);
	}
	window.Update();

	if (!endFlag)
	{
		return;
	}
	++endTimer;

	if (KazMath::ConvertSecondToFlame(2) <= endTimer)
	{
		window.End();
		for (int i = 0; i < tutorialText.size(); ++i)
		{
			tutorialText[i]->Finalize();
		}
	}
}

void TutorialGame::Draw()
{
	for (int i = 0; i < tutorialText.size(); ++i)
	{
		tutorialText[i]->Draw();
	}
	window.Draw();
}

void TutorialGame::Succeed()
{
	if (endFlag)
	{
		return;
	}
	for (int i = 0; i < tutorialText.size(); ++i)
	{
		tutorialText[i]->Finalize();
	}
	tutorialText[0]->Init({WIN_X / 2.0f - 120.0f,125.0f}, "Succeed", 3.0f);
	endFlag = true;
}

bool TutorialGame::NextTutorial()
{
	return endFlag && window.IsEnd();
}