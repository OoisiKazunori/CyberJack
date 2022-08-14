#pragma once
#include"../KazLibrary/Scene/SceneBase.h"
#include"../KazLibrary/RenderTarget/RenderTargetStatus.h"
#include"../KazLibrary/Render/KazRender.h"
#include"../Game/Helper/EnemyHelper.h"
#include"../KazLibrary/Render/BackGroundForDebug.h"

class EnemyDebugScene :public SceneBase
{
public:
	EnemyDebugScene();
	~EnemyDebugScene();

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

	//“G----------------------------------------------------------------
	std::array<std::unique_ptr<IEnemy>, 10>enemies;
	std::array<std::array<std::unique_ptr<IEnemy>, 10>, 10>misiles;
	std::array<int, 10>kidEnemyHandles;
	//“G----------------------------------------------------------------


	BoxPolygonRender player;
	int gameFlame;
	int specifiedEnemyType, oldEnemyType;
	bool initFlag, deadFlag, attackFlag;
	KazMath::Vec3<float>responePos;

	BackGroundForDebug bg;

	FbxModelRender kidFbxModel;
	ObjModelRender kidObjModel;
};

