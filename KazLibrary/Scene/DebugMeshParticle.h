#pragma once
#include"../Scene/SceneBase.h"
#include"../Render/BackGroundForDebug.h"
#include"../Render/KazRender.h"
#include"../Game/Helper/CameraWork.h"
#include"../Game/GameRenderTarget.h"
#include"../Game/Effect/MeshParticleEmitter.h"
#include"../KazLibrary/Math/KazMath.h"


class DebugMeshParticleScene :public SceneBase
{
public:
	DebugMeshParticleScene();
	~DebugMeshParticleScene();

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

	CameraWork cameraWork;

	//MeshParticleEmitter meshEmitter;


	//デバック用--------------------------------------------
	//三角形描画
	std::array<KazMath::Vec3<float>, 3>triangelPosArray;
	std::array<LineRender, 3>triangelLine;

	//パーティクルの数
	static const int PARTICLE_MAX_NUM = 30;
	std::array<BoxPolygonRender, PARTICLE_MAX_NUM>particle;
	int drawParticleNum;

	//計算
	bool initFlag;

	//デバック用--------------------------------------------

};

