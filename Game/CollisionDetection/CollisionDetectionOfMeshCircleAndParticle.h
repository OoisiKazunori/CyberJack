#pragma once
#include"../Game/Helper/ComputeBufferHelper.h"

/// <summary>
/// BB���ɔz�u���ꂽ�����蔻����W�ƃ��b�V���p�[�e�B�N�����W�Ƃ̓����蔻����s���܂�
/// </summary>
class CollisionDetectionOfMeshCircleAndParticle
{
public:
	CollisionDetectionOfMeshCircleAndParticle(
		const ComputeBufferHelper::BufferData &MESH_HITBOX_POS,
		const ComputeBufferHelper::BufferData &MESH_HITBOX_ID,
		const ComputeBufferHelper::BufferData &PARTICLE_POS
	);
	void Compute();

private:
	ComputeBufferHelper computeHelper;
	RESOURCE_HANDLE particleHitBoxIDHandle;
	RESOURCE_HANDLE commonDataHandle;

	struct CommonData
	{
		UINT meshHitBoxNum;
		float hitBoxRadius;
	};
};

