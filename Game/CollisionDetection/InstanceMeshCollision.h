#pragma once
#include"../KazLibrary/Math/KazMath.h"
#include"../KazLibrary/Collision/BoundingBox.h"
#include"../KazLibrary/Collision/BBDuringEquallyCoordinatePlace.h"
#include"../KazLibrary/Helper/ResouceBufferHelper.h"
#include"../Game/CollisionDetection/GenerateCollisionOfParticle.h"

class InstanceMeshCollision
{
public:
	/// <summary>
	/// ���_���A���b�V���p�[�e�B�N�����W�z������ă��b�V���p�[�e�B�N���̓����蔻��𐶐�����B
	/// </summary>
	InstanceMeshCollision(
		const std::vector<ResouceBufferHelper::BufferData> &VERT_DATA,
		const std::vector<UINT> &VERT_NUM_ARRAY
	);

	void Init();
	void Compute();
private:
	std::vector<BoundingBox> bb;
	std::vector<BBDuringEquallyCoordinatePlace> generateMeshHitBox;
	std::vector<GenerateCollisionOfParticle> linkMeshHitBoxAndParticle;
};
