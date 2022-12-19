#include "BBDuringEquallyCoordinatePlace.h"
#include"../KazLibrary/DirectXCommon/DirectX12Device.h"
#include"../KazLibrary/DirectXCommon/DirectX12CmdList.h"
#include"../KazLibrary/Pipeline/GraphicsRootSignature.h"
#include"../KazLibrary/Pipeline/GraphicsPipeLineMgr.h"
#include"../KazLibrary/Helper/KazRenderHelper.h"
#include"../KazLibrary/RenderTarget/RenderTargetStatus.h"
#include"../KazLibrary/Buffer/DescriptorHeapMgr.h"
#include"../KazLibrary/Helper/ResourceFilePass.h"
#include"../KazLibrary/Imgui/MyImgui.h"
#include"../KazLibrary/Loader/ObjResourceMgr.h"
#include"../KazLibrary/Buffer/UavViewHandleMgr.h"

BBDuringEquallyCoordinatePlace::BBDuringEquallyCoordinatePlace(D3D12_GPU_DESCRIPTOR_HANDLE BB_BUFFER_HANDLE, const BoundingBoxData &DATA) :data(DATA), bbViewHandle(BB_BUFFER_HANDLE)
{
	radius = 0.1f;
	//いくつ配置出来るか計算する
	KazMath::Vec3<float>lDistance = data.maxPos - data.minPos;

	threadNumData.x = CalculatingDeployableNumber(lDistance.x, radius);
	threadNumData.y = CalculatingDeployableNumber(lDistance.y, radius);
	threadNumData.z = CalculatingDeployableNumber(lDistance.z, radius);

	BUFFER_SIZE lCountNum = static_cast<BUFFER_SIZE>(threadNumData.x * threadNumData.y * threadNumData.z);

	//当たり判定座標の用意
	hitBoxPosHandle = computeHelper.CreateBuffer(
		KazBufferHelper::SetOnlyReadStructuredBuffer(sizeof(DirectX::XMFLOAT3) * lCountNum),
		GRAPHICS_RANGE_TYPE_UAV_VIEW,
		GRAPHICS_PRAMTYPE_DATA2,
		sizeof(DirectX::XMFLOAT3),
		lCountNum
	);
	hitBoxViewHandle = computeHelper.GetDescriptorViewHandle(hitBoxPosHandle);

	//当たり判定IDの用意
	hitBoxIDHandle = computeHelper.CreateBuffer(
		KazBufferHelper::SetOnlyReadStructuredBuffer(sizeof(DirectX::XMUINT3) * lCountNum),
		GRAPHICS_RANGE_TYPE_UAV_DESC,
		GRAPHICS_PRAMTYPE_DATA3,
		sizeof(DirectX::XMUINT3),
		lCountNum
	);
	hitBoxIDViewHandle = computeHelper.GetDescriptorViewHandle(hitBoxIDHandle);

	//事前に計算しておくもの用意
	hitBoxCommonHandle = computeHelper.CreateBuffer(sizeof(HitBoxConstBufferData), GRAPHICS_RANGE_TYPE_CBV_VIEW, GRAPHICS_PRAMTYPE_DATA4, 1);
	HitBoxConstBufferData lData;
	lData.radius = radius;
	lData.xMax = threadNumData.x;
	lData.xyMax = threadNumData.x * threadNumData.y;
	computeHelper.TransData(hitBoxCommonHandle, &lData, sizeof(HitBoxConstBufferData));
}

void BBDuringEquallyCoordinatePlace::Compute()
{
	computeHelper.Compute(PIPELINE_COMPUTE_NAME_HITBOX_SETCIRCLE_IN_BB, threadNumData);
}

UINT BBDuringEquallyCoordinatePlace::CalculatingDeployableNumber(float DISTANCE, float RADIUS)
{
	float lDistance = DISTANCE;
	UINT num = 0;
	while (0.0f < lDistance)
	{
		lDistance -= RADIUS;
		++num;
	}
	return num;
}
