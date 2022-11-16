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

	std::array<std::unique_ptr<MeshParticleEmitter>, 1> meshEmitter;
	int meshIndex,prevMeshIndex;
	bool cpuCheckParticleFlag;
	bool gpuCheckParticleFlag;

	KazMath::Transform3D motherTransform;
	DirectX::XMMATRIX motherMat;

	//デバック用--------------------------------------------
	//三角形描画
	std::array<KazMath::Vec3<float>, 3>triangelPosArray;
	std::array<LineRender, 3>triangelLine;
	std::array<LineRender, 3>clossTriangelLine;

	//パーティクルの数
	static const int PARTICLE_MAX_NUM = 1000;
	std::array<BoxPolygonRender, PARTICLE_MAX_NUM>particle;
	int drawParticleNum;
	int particleBias;//パーティクルをエッジ周辺に偏よらせる値
	static const int PARTICLE_MAX_BIAS = 100;//パーティクルをエッジ周辺に偏よらせる値

	//計算
	bool initFlag;
	BackGroundForDebug debugDraw;
	//デバック用--------------------------------------------


	std::unique_ptr<GameRenderTarget>rendertarget;
	Sprite2DRender mainRenderTarget;
};

