#pragma once

/// <summary>
/// ステージの抽象クラス
/// ステージ毎の描画を纏めてあります
/// </summary>
class IStage
{
public:
	virtual ~IStage() {};
	virtual void Update() = 0;
	virtual void Draw() = 0;

	void SetCamera(int CAMERA_INDEX)
	{
		cameraIndex = CAMERA_INDEX;
	};

	bool startFlag = false;
protected:
	int cameraIndex = 0;
};

