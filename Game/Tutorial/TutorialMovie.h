#pragma once
#include"../KazLibrary/Math/KazMath.h"
#include"../Tutorial/DirectX12MoviePlayer.h"
#include"../GameRenderTarget.h"

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


private:
	//����Đ�--------------------------------------------
	bool startMovieFlag;
	std::unique_ptr<DirectX12MoviePlayer> moviePlayer;
	RESOURCE_HANDLE seedHandle;
	MovieRender movieRender;
	float seedNum;
	//����Đ�--------------------------------------------

	RESOURCE_HANDLE renderTargetHandle;


	//UI--------------------------------------------
	
	//UI--------------------------------------------

};