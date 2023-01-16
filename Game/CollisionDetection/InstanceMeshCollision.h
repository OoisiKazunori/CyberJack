#pragma once
#include"../KazLibrary/Math/KazMath.h"
#include"../KazLibrary/Collision/BoundingBox.h"
#include"../KazLibrary/Collision/BBDuringEquallyCoordinatePlace.h"
#include"../KazLibrary/Helper/ResouceBufferHelper.h"
#include"../Game/CollisionDetection/GenerateCollisionOfParticle.h"
#include"../Game/Effect/MeshParticle.h"
#include"../Game/CollisionDetection/CollisionDetectionOfMeshCircleAndParticle.h"

struct InitMeshCollisionData
{
	ResouceBufferHelper::BufferData vertData;
	UINT vertNumArray;
	InitMeshParticleData meshParticleData;
	Sphere hitBox;
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

};
