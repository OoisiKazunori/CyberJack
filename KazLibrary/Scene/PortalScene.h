#pragma once
#include"../Game/Game.h"
#include"../KazLibrary/Scene/SceneBase.h"
#include"../Game/Event/GoalBoxPortalEffect.h"
#include"../Game/Event/GoalBoxPortalStringEffect.h"
#include"../Game/Event/PortalFlame.h"
#include"../KazLibrary/Render/KazRender.h"

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
	GoalBoxPortalStringEffect stringEffect;


	Sprite2DRender mainRenderTarget;
	Sprite2DRender addRenderTarget;
	Sprite2DRender luminaceTex;
	bool changeFlag;
	bool animFlag;
	BackGroundForDebug bg;

	std::vector<RESOURCE_HANDLE>multipassHandle;
	RESOURCE_HANDLE addHandle;

	std::unique_ptr<GaussianBuler> buler;


	PortalFlame portalFlame;
	bool gameModeFlag;

	Cursor cursor;
	BoxPolygonRender besidePoly, verticlaPoly, cameraPoly;
	KazMath::Vec3<float> nowTargerPos, trackingTargetPos;	//本来ポズ、現在ポズ
	KazMath::Vec2<float> leftRightAngleVel;					//左右視点座標の向く角度
	KazMath::Vec2<float> upDownAngleVel;					//上下視点座標の向く角度
	KazMath::Vec2<float> trackLeftRightAngleVel;
	KazMath::Vec2<float> trackUpDownAngleVel;
	KazMath::Vec2<float> forceCameraDirVel;					//カメラの前後左右の向きを強制的に指定する
	KazMath::Vec2<float> mulValue;							//カメラの上下左右の掛ける割合
	KazMath::Vec2<float> mulValue2;							//カメラの上下左右の掛ける割合

	Sprite3DRender redPortal;
	Sprite3DRender greenPortal;
	std::array<BoxPolygonRender, 2>stage;
};


