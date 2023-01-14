#include "CollisionDetectionOfMeshCircleAndParticle.h"

CollisionDetectionOfMeshCircleAndParticle::CollisionDetectionOfMeshCircleAndParticle(
	const ResouceBufferHelper::BufferData &MESH_HITBOX_POS,
	const ResouceBufferHelper::BufferData &MESH_HITBOX_ID,
	const ResouceBufferHelper::BufferData &PARTICLE_POS
)
{
	//当たり判定座標
	computeHelper.SetBuffer(MESH_HITBOX_POS, GRAPHICS_PRAMTYPE_DATA);
	//ID
	computeHelper.SetBuffer(MESH_HITBOX_ID, GRAPHICS_PRAMTYPE_DATA2);
	//パーティクル座標
	computeHelper.SetBuffer(PARTICLE_POS, GRAPHICS_PRAMTYPE_DATA3);
	//パーティクルID
	particleHitBoxIDHandle = computeHelper.CreateBuffer(
		KazBufferHelper::SetOnlyReadStructuredBuffer(sizeof(DirectX::XMUINT3)),
		GRAPHICS_RANGE_TYPE_UAV_DESC,
		GRAPHICS_PRAMTYPE_DATA4,
		sizeof(DirectX::XMUINT3),
		1);


	commonDataHandle = computeHelper.CreateBuffer(sizeof(CommonData), GRAPHICS_RANGE_TYPE_CBV_VIEW, GRAPHICS_PRAMTYPE_DATA5, 1);

	CommonData lData;
	lData.hitBoxRadius = 1.0f;
	lData.meshHitBoxNum = 20;
	computeHelper.TransData(commonDataHandle, &lData, sizeof(CommonData));
}

void CollisionDetectionOfMeshCircleAndParticle::Compute()
{
	computeHelper.StackToCommandListAndCallDispatch(PIPELINE_COMPUTE_NAME_HITBOX_MESHCIRCLE_PARTICLE, { 100,1,1 });
}
