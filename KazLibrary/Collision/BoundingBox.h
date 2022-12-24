#pragma once
#include"../Game/Helper/ComputeBufferHelper.h"

struct BoundingBoxData
{
	KazMath::Vec3<float> minPos;
	KazMath::Vec3<float> maxPos;
};

/// <summary>
/// �ǂݍ��񂾃��f������BoundingBox���`������������
/// </summary>
class BoundingBox
{
public:
	BoundingBox(std::vector<DirectX::XMFLOAT4> VERT_DATA);
	BoundingBoxData GetData();
	D3D12_GPU_DESCRIPTOR_HANDLE GetViewHandle();

	void Compute();
	
	ComputeBufferHelper computeBuffer;
	RESOURCE_HANDLE bbBufferHandle;
private:
	struct BoundingBoxBufferData
	{
		DirectX::XMFLOAT3 minPos;
		DirectX::XMFLOAT3 maxPos;
	};
	struct CommonData
	{
		DirectX::XMMATRIX scaleRotaMat;
		UINT vertNum;
	};

	RESOURCE_HANDLE vertBufferHandle;
	RESOURCE_HANDLE bbViewHandle;
	RESOURCE_HANDLE matBufferHandle;

};