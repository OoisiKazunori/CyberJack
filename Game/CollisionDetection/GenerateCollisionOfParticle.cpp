#include"GenerateCollisionOfParticle.h"

GenerateCollisionOfParticle::GenerateCollisionOfParticle(const ResouceBufferHelper::BufferData &HITBOX_DATA, const ResouceBufferHelper::BufferData &PARTICLE_DATA, const ResouceBufferHelper::BufferData &STACK_PARTICLE_HITBOX_DATA)
{
	computeHelper.SetBuffer(HITBOX_DATA, GRAPHICS_PRAMTYPE_DATA);
	computeHelper.SetBuffer(PARTICLE_DATA, GRAPHICS_PRAMTYPE_DATA2);
	computeHelper.SetBuffer(STACK_PARTICLE_HITBOX_DATA, GRAPHICS_PRAMTYPE_DATA3);

	RESOURCE_HANDLE lCommonBufferHandle = computeHelper.CreateBuffer(sizeof(CommonData), GRAPHICS_RANGE_TYPE_CBV_VIEW, GRAPHICS_PRAMTYPE_DATA4, 1);

	CommonData lCommonData;
	lCommonData.particleRadius = 0.1f;
	lCommonData.meshHitBoxMaxNum = 1;
	lCommonData.meshHitBoxRadius = 20.0f;
	computeHelper.TransData(lCommonBufferHandle, &lCommonData, sizeof(CommonData));
}

void GenerateCollisionOfParticle::Compute()
{
	computeHelper.StackToCommandListAndCallDispatch(PIPELINE_COMPUTE_NAME_HITBOX_ID_ATTACH_TO_PARTICLE, { 1000,1,1 }, -1);
}