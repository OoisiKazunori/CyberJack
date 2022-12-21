#pragma once
#include"../Game/Helper/ComputeBufferHelper.h"
#include"BoundingBox.h"

/// <summary>
/// BoundingBox‚Ìî•ñ‚ğŒ³‚ÉA“™ŠÔŠu‚ÉÀ•W‚ğ’u‚­
/// </summary>
class BBDuringEquallyCoordinatePlace
{
public:
	BBDuringEquallyCoordinatePlace(D3D12_GPU_DESCRIPTOR_HANDLE BB_BUFFER_HANDLE, const BoundingBoxData &DATA);

	void Compute();
	UINT MaxHitBoxPosNum()
	{
		return countNum;
	}
	const ComputeBufferHelper::BufferData GetHitBoxPosData()
	{
		return computeHelper->GetBufferData(hitBoxPosHandle);
	}
	const ComputeBufferHelper::BufferData GetHitBoxIDData()
	{
		return computeHelper->GetBufferData(hitBoxIDHandle);
	}
	std::unique_ptr<ComputeBufferHelper> computeHelper;
private:

	D3D12_GPU_DESCRIPTOR_HANDLE bbViewHandle;
	RESOURCE_HANDLE hitBoxPosHandle, hitBoxIDHandle,hitBoxViewHandle, hitBoxIDViewHandle;
	RESOURCE_HANDLE hitBoxCommonHandle;

	BoundingBoxData data;
	float diameter;
	UINT CalculatingDeployableNumber(float DISTANCE, float RADIUS);

	struct HitBoxConstBufferData
	{
		float diameter;
		UINT xMax;
		UINT xyMax;
	};

	struct MeshHitBoxData
	{
		DirectX::XMFLOAT3 pos;
		DirectX::XMUINT3 id;
	};

	DispatchCallData threadNumData;
	UINT countNum;
};
