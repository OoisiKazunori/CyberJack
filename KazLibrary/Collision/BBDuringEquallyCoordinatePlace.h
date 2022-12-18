#pragma once
#include"../KazLibrary/Buffer/CreateGpuBuffer.h"
#include"../KazLibrary/Math/KazMath.h"
#include"BoundingBox.h"
#include"../KazLibrary/Render/DrawExcuteIndirect.h"

/// <summary>
/// BoundingBoxÇÃèÓïÒÇå≥Ç…ÅAìôä‘äuÇ…ç¿ïWÇíuÇ≠
/// </summary>
class BBDuringEquallyCoordinatePlace
{
public:
	BBDuringEquallyCoordinatePlace(D3D12_GPU_DESCRIPTOR_HANDLE BB_BUFFER_HANDLE, const BoundingBoxData &DATA);

	void Compute();

private:
	CreateGpuBuffer buffers;

	D3D12_GPU_DESCRIPTOR_HANDLE bbViewHandle;
	RESOURCE_HANDLE hitBoxPosHandle, hitBoxIDHandle,hitBoxViewHandle, hitBoxIDViewHandle;
	RESOURCE_HANDLE hitBoxCommonHandle;

	BoundingBoxData data;
	float radius;
	KazMath::Vec3<UINT>threadNum;
	UINT CalculatingDeployableNumber(float DISTANCE, float RADIUS);

	struct HitBoxConstBufferData
	{
		float radius;
		UINT xMax;
		UINT xyMax;
	};

	struct MeshHitBoxData
	{
		DirectX::XMFLOAT3 pos;
		DirectX::XMUINT3 id;
	};
};
