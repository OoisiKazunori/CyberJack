#pragma once
#include"../KazLibrary/Helper/ResouceBufferHelper.h"

/// <summary>
/// BB内に配置された当たり判定座標とメッシュパーティクル座標との当たり判定を行います
/// </summary>
class CollisionDetectionOfMeshCircleAndParticle
{
public:
	CollisionDetectionOfMeshCircleAndParticle(
		const ResouceBufferHelper::BufferData &MESH_HITBOX_POS,
		const ResouceBufferHelper::BufferData &MESH_HITBOX_ID,
		const ResouceBufferHelper::BufferData &PARTICLE_POS
	);
	void Compute();

private:
	ResouceBufferHelper computeHelper;
	RESOURCE_HANDLE particleHitBoxIDHandle;
	RESOURCE_HANDLE commonDataHandle;

	struct CommonData
	{
		UINT meshHitBoxNum;
		float hitBoxRadius;
	};
};

