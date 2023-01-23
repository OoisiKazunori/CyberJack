#pragma once
#include"../KazLibrary/Math/KazMath.h"
#include"../KazLibrary/Collision/BoundingBox.h"
#include"../KazLibrary/Collision/BBDuringEquallyCoordinatePlace.h"
#include"../KazLibrary/Helper/ResouceBufferHelper.h"
#include"../Game/CollisionDetection/GenerateCollisionOfParticle.h"
#include"../Game/Effect/MeshParticle.h"
#include"../Game/CollisionDetection/CollisionDetectionOfMeshCircleAndParticle.h"
#include"../Game/CollisionDetection/ComputeParticleAvoidSphere.h"

struct InitMeshCollisionData
{
	ResouceBufferHelper::BufferData vertData;
	UINT vertNumArray;
	InitMeshParticleData meshParticleData;
	Sphere hitBox;
	const DirectX::XMMATRIX *motherMat;
};

class InstanceMeshCollision
{
public:
	/// <summary>
	/// 頂点情報、メッシュパーティクル座標配列を入れてメッシュパーティクルの当たり判定を生成する。
	/// </summary>
	InstanceMeshCollision(
		const std::vector<InitMeshCollisionData> &INIT_DATA
	);

	void Init();
	void Compute();

private:
	std::vector<BBDuringEquallyCoordinatePlace> generateMeshHitBox;
	std::vector<GenerateCollisionOfParticle> linkMeshHitBoxAndParticle;

	std::unique_ptr<CollisionDetectionOfMeshCircleAndCPUHitBox> cpuAndMeshCircleHitBox;
	ComputeParticleAvoidSphere particleAvoidParticle;

	struct MeshParticleData
	{
		BoundingBox bb;
		MeshParticle meshParticle;

		MeshParticleData(
			const ResouceBufferHelper::BufferData &VERT_DATA,
			const UINT &VERT_NUM_ARRAY,
			const InitMeshParticleData &DATA, UINT ID)
			:bb(VERT_DATA, VERT_NUM_ARRAY),
			meshParticle(DATA, ID)
		{
		};
	};
	std::vector<MeshParticleData>meshData;
	std::vector<Sphere>hitBoxData;



	KazRenderHelper::ID3D12ResourceWrapper copyBuffer;

	//歪み込みのパーティクル情報を親子関係で動かす-----------------------------
	std::vector<const DirectX::XMMATRIX*>motherMatArray;
	ResouceBufferHelper updatePosCompute;
	ResouceBufferHelper::BufferData motherMatrixBuffer;
	RESOURCE_HANDLE motherMatHandle, scaleRotateBillboardMatHandle;
	DirectX::XMMATRIX scaleRotaMat, scaleRotBillBoardMat;
	//歪み込みのパーティクル情報を親子関係で動かす-----------------------------

	ResouceBufferHelper meshMoveCompute;
	RESOURCE_HANDLE inputMeshCircleBufferHandle, outputMeshCircleBufferHandle;
	struct MeshHitBoxData
	{
		DirectX::XMFLOAT3 pos;
		UINT meshID;
		DirectX::XMUINT3 id;
	};

};
