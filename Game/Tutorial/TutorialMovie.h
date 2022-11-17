#pragma once
#include"../KazLibrary/Math/KazMath.h"
#include"../Tutorial/DirectX12MoviePlayer.h"
#include"../GameRenderTarget.h"

/// <summary>
/// ゲーム内で流れるチュートリアル
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
	//動画再生--------------------------------------------
	bool startMovieFlag;
	std::unique_ptr<DirectX12MoviePlayer> moviePlayer;
	RESOURCE_HANDLE seedHandle;
	MovieRender movieRender;
	float seedNum;
	//動画再生--------------------------------------------

	RESOURCE_HANDLE renderTargetHandle;


	//UI--------------------------------------------
	
	//UI--------------------------------------------

};