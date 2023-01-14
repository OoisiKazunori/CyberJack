#include"GenerateCollisionOfParticle.h"

GenerateCollisionOfParticle::GenerateCollisionOfParticle(const ResouceBufferHelper::BufferData &HITBOX_DATA, const ResouceBufferHelper::BufferData &PARTICLE_DATA)
{
	computeHelper.SetBuffer(HITBOX_DATA, GRAPHICS_PRAMTYPE_DATA);
	computeHelper.SetBuffer(PARTICLE_DATA, GRAPHICS_PRAMTYPE_DATA2);

	//当たり判定付きパーティクル
	computeHelper.CreateBuffer(
		KazBufferHelper::SetOnlyReadStructuredBuffer(sizeof(ParticleHitBoxData) * PARTICLE_MAX_NUM),
		GRAPHICS_RANGE_TYPE_UAV_DESC,
		GRAPHICS_PRAMTYPE_DATA3,
		sizeof(ParticleHitBoxData),
		PARTICLE_MAX_NUM
	);

	RESOURCE_HANDLE lCommonBufferHandle = computeHelper.CreateBuffer(sizeof(CommonData), GRAPHICS_RANGE_TYPE_CBV_VIEW, GRAPHICS_PRAMTYPE_DATA4, 1);

	CommonData lCommonData;
	lCommonData.particleRadius = 10.0f;
	lCommonData.meshHitBoxMaxNum = 4;
	lCommonData.meshHitBoxRadius = 1.0f;
	computeHelper.TransData(lCommonBufferHandle, &lCommonData, sizeof(CommonData));
}

void GenerateCollisionOfParticle::Compute()
{
	computeHelper.StackToCommandListAndCallDispatch(PIPELINE_COMPUTE_NAME_HITBOX_ID_ATTACH_TO_PARTICLE, { 100,1,1 });
}