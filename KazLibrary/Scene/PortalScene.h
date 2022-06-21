#pragma once
#include"../Game/Game.h"
#include"../KazLibrary/Scene/SceneBase.h"
#include"../Game/Event/GoalBoxPortalEffect.h"

class PortalScene :public SceneBase
{
public:
	PortalScene();
	~PortalScene();

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

	KazMath::Vec3<float>initPos;
	GoalBoxPortalEffect portal;
	BackGroundForDebug bg;

};


