#pragma once
#include"../KazLibrary/Math/KazMath.h"
#include"../Tutorial/DirectX12MoviePlayer.h"
#include"../GameRenderTarget.h"
#include"../Game/Event/TutorialGame.h"

/// <summary>
/// �Q�[�����ŗ����`���[�g���A��
/// </summary>
class TutorialMovie
{
public:
	TutorialMovie();
	void Init();
	void Update();
	void Draw();

	void Play();
	void Noise();
	void Stop();

	bool End();

	void Succeed()
	{
		tutorialText.Succeed();
	}
	bool NextTutorial()
	{
		return tutorialText.NextTutorial();
	}

	RESOURCE_HANDLE GetTexture()
	{
		return outputRenderTargetHandle;
	};
private:
	//����Đ�--------------------------------------------
	bool startMovieFlag;
	bool startNoiseFlag;
	bool stopFlag;
	std::unique_ptr<DirectX12MoviePlayer> moviePlayer;
	RESOURCE_HANDLE vhsSeedHandle, noiseSeedHandle;
	MovieRender movieRender;
	float seedNum;
	//����Đ�--------------------------------------------

	//�m�C�Y�Đ�--------------------------------------------
	Sprite2DRender normalRender;
	//�m�C�Y�Đ�--------------------------------------------

	//UI--------------------------------------------
	TutorialGame tutorialText;//����UI
	//UI--------------------------------------------

	RESOURCE_HANDLE renderTargetHandle;
	RESOURCE_HANDLE outputRenderTargetHandle;
	Sprite2DRender outputRender;
};