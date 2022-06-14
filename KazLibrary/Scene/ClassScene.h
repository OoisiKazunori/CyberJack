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
	XMFLOAT3 debugCameraMove;
	XMFLOAT3 eyePos, targetPos;
	XMFLOAT2 angle;

	int handle;
	constBufferVhsData data;
	FbxModelRenderPtr modelRender;
	Sprite2DRenderPtr gameRenderTarget;
	Sprite2DRenderPtr mainRenderTarget;
	BackGroundForDebug bg;

	std::vector<short>multiHandle;

	XMVECTOR rayStartPos = { 0.0f,0.0f,0.0f };
	XMVECTOR rayEndPos = { 10.0f,0.0f,0.0f };
	XMVECTOR circleCentralPos = { 5.0f,11.0f,0.0f };
	float radius = 10.0f;
	bool hitFlag = false;

	Line2DRender line;
	Circle2DRender circle;
};

