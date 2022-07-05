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
	KazMath::Vec3<float> nowTargerPos, trackingTargetPos;	//�{���|�Y�A���݃|�Y
	KazMath::Vec2<float> leftRightAngleVel;					//���E���_���W�̌����p�x
	KazMath::Vec2<float> upDownAngleVel;					//�㉺���_���W�̌����p�x
	KazMath::Vec2<float> trackLeftRightAngleVel;
	KazMath::Vec2<float> trackUpDownAngleVel;
	KazMath::Vec2<float> forceCameraDirVel;					//�J�����̑O�㍶�E�̌����������I�Ɏw�肷��
	KazMath::Vec2<float> mulValue;							//�J�����̏㉺���E�̊|���銄��
	KazMath::Vec2<float> mulValue2;							//�J�����̏㉺���E�̊|���銄��

	Sprite3DRender redPortal;
	Sprite3DRender greenPortal;
	std::array<BoxPolygonRender, 2>stage;
};


