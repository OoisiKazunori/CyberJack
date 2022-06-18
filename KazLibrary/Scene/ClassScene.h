#pragma once
#include"../Scene/SceneBase.h"
#include"../Render/BackGroundForDebug.h"
#include"../Render/KazRender.h"

class ClassScene:public SceneBase
{
public:
	ClassScene();
	~ClassScene();

	void Init();
	void Finalize();
	void Input();
	void Update();
	void Draw();

	int SceneChange();

private:
	KazMath::Vec3<float> debugCameraMove;
	KazMath::Vec3<float> eyePos, targetPos;
	KazMath::Vec2<float> angle;

	int handle;
	constBufferVhsData data;
	FbxModelRenderPtr modelRender;
	Sprite2DRenderPtr gameRenderTarget;
	Sprite2DRenderPtr mainRenderTarget;
	BackGroundForDebug bg;

	std::vector<short>multiHandle;
};

