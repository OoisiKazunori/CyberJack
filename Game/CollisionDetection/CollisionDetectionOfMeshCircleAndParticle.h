#pragma once
#include"../KazLibrary/Helper/ResouceBufferHelper.h"

/// <summary>
/// BB���ɔz�u���ꂽ�����蔻����W�ƃ��b�V���p�[�e�B�N�����W�Ƃ̓����蔻����s���܂�
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

