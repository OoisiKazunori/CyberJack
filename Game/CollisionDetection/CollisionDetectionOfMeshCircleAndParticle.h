#pragma once
#include"../KazLibrary/Helper/ResouceBufferHelper.h"
#include"../KazLibrary/Collision/CollisionManager.h"

/// <summary>
/// BB���ɔz�u���ꂽ�����蔻����W��CPU�̓����蔻����W�Ƃ̓����蔻����s���܂�
/// </summary>
class CollisionDetectionOfMeshCircleAndCPUHitBox
{
public:
	CollisionDetectionOfMeshCircleAndCPUHitBox(
		const std::vector<Sphere> &CPU_HITBOX_DATA
	);
	void Compute();

	const ResouceBufferHelper::BufferData &GetStackMeshCircleBuffer();
private:
	ResouceBufferHelper computeHelper;
	RESOURCE_HANDLE cpuHitBoxBufferHandle,meshCircleArrayBufferHandle;
	RESOURCE_HANDLE commonDataHandle;

	struct CommonData
	{
		UINT cpuHitBoxNum;
		float particleRadius;
	};
	struct HitIDData
	{
		UINT meshID;
		DirectX::XMUINT3 id;
	};
	struct SphereData
	{
		DirectX::XMFLOAT3 pos;
		float radius;
	};

	struct MeshHitBoxData
	{
		DirectX::XMFLOAT3 pos;
		UINT meshID;
		DirectX::XMUINT3 id;
	};
	std::vector<Sphere>sphereHitBoxArray;

	KazRenderHelper::ID3D12ResourceWrapper motherMatrixBuffer;

	KazRenderHelper::ID3D12ResourceWrapper copyBuffer;
};

