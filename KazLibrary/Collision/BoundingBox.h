#pragma once
#include"../KazLibrary/Buffer/CreateGpuBuffer.h"
#include"../KazLibrary/Math/KazMath.h"

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

	void Compute();
private:
	CreateGpuBuffer buffers;

	struct BoundingBoxBufferData
	{
		DirectX::XMFLOAT3 minPos;
		DirectX::XMFLOAT3 maxPos;
	};

	RESOURCE_HANDLE vertBufferHandle;
	RESOURCE_HANDLE bbBufferHandle;
};