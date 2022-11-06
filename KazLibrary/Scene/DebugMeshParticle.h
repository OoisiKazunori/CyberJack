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


	//�f�o�b�N�p--------------------------------------------
	//�O�p�`�`��
	std::array<KazMath::Vec3<float>, 3>triangelPosArray;
	std::array<LineRender, 3>triangelLine;

	//�p�[�e�B�N���̐�
	static const int PARTICLE_MAX_NUM = 30;
	std::array<BoxPolygonRender, PARTICLE_MAX_NUM>particle;
	int drawParticleNum;

	//�v�Z
	bool initFlag;

	//�f�o�b�N�p--------------------------------------------

};

