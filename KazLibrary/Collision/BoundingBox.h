#pragma once
#include"../Game/Helper/ComputeBufferHelper.h"

struct BoundingBoxData
{
	KazMath::Vec3<float> minPos;
	KazMath::Vec3<float> maxPos;
};

/// <summary>
/// “Ç‚İ‚ñ‚¾ƒ‚ƒfƒ‹‚©‚çBoundingBox‚ğŒ`¬‚·‚éî•ñ‚ğì‚é
/// </summary>
class BoundingBox
{
public:
	BoundingBox(std::vector<DirectX::XMFLOAT4> VERT_DATA);
	BoundingBoxData GetData();
	D3D12_GPU_DESCRIPTOR_HANDLE GetViewHandle();

	void Compute();
private:
	struct BoundingBoxBufferData
	{
		DirectX::XMFLOAT3 minPos;
		DirectX::XMFLOAT3 maxPos;
	};

	RESOURCE_HANDLE vertBufferHandle;
	RESOURCE_HANDLE bbBufferHandle, bbViewHandle;
	RESOURCE_HANDLE matBufferHandle;

	ComputeBufferHelper computeBuffer;
};