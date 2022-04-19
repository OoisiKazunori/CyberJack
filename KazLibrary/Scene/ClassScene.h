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

	double caluDistance;
	double caluDistance2;


	float valueOfAcceleration;
	float initialVelocity;
	float time;
	float landingTime;

	bool startFlag;

	BoxPolygonRenderPtr boxRender;
	BoxPolygonRenderPtr landingPointRender;
	LineRenderPtr distanceRender;
	BackGroundForDebug bg;

	std::array<unique_ptr<DirtySet>, 2>dirtyFlags;
};

