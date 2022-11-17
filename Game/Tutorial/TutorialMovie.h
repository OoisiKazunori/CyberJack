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

	void Noise();
	void Play();

private:
	//動画再生--------------------------------------------
	bool startMovieFlag;
	std::unique_ptr<DirectX12MoviePlayer> moviePlayer;
	RESOURCE_HANDLE seedHandle;
	MovieRender render;
	float seedNum;
	//動画再生--------------------------------------------

	bool startNoiseFlag;
	RESOURCE_HANDLE greenBackHandle;

	std::unique_ptr<GameRenderTarget> renderTarget;


	//UI--------------------------------------------
	
	//UI--------------------------------------------

};