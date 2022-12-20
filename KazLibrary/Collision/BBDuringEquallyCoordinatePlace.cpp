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

BBDuringEquallyCoordinatePlace::BBDuringEquallyCoordinatePlace(D3D12_GPU_DESCRIPTOR_HANDLE BB_BUFFER_HANDLE, const BoundingBoxData &DATA) :data(DATA), bbViewHandle(BB_BUFFER_HANDLE), countNum(0)
{
	computeHelper = std::make_unique<ComputeBufferHelper>();

	diameter = 0.5f;
	//いくつ配置出来るか計算する
	KazMath::Vec3<float>lDistance = data.maxPos - data.minPos;

	threadNumData.x = CalculatingDeployableNumber(lDistance.x, diameter);
	threadNumData.y = CalculatingDeployableNumber(lDistance.y, diameter);
	threadNumData.z = CalculatingDeployableNumber(lDistance.z, diameter);

	countNum = static_cast<BUFFER_SIZE>(threadNumData.x * threadNumData.y * threadNumData.z);

	//当たり判定座標の用意
	hitBoxPosHandle = computeHelper->CreateBuffer(
		KazBufferHelper::SetOnlyReadStructuredBuffer(sizeof(DirectX::XMFLOAT3) * countNum),
		GRAPHICS_RANGE_TYPE_UAV_DESC,
		GRAPHICS_PRAMTYPE_DATA2,
		sizeof(DirectX::XMFLOAT3),
		countNum
	);
	hitBoxViewHandle = computeHelper->GetDescriptorViewHandle(hitBoxPosHandle);

	//当たり判定IDの用意
	hitBoxIDHandle = computeHelper->CreateBuffer(
		KazBufferHelper::SetOnlyReadStructuredBuffer(sizeof(DirectX::XMUINT3) * countNum),
		GRAPHICS_RANGE_TYPE_UAV_DESC,
		GRAPHICS_PRAMTYPE_DATA3,
		sizeof(DirectX::XMUINT3),
		countNum
	);
	hitBoxIDViewHandle = computeHelper->GetDescriptorViewHandle(hitBoxIDHandle);

	//事前に計算しておくもの用意
	hitBoxCommonHandle = computeHelper->CreateBuffer(sizeof(HitBoxConstBufferData), GRAPHICS_RANGE_TYPE_CBV_VIEW, GRAPHICS_PRAMTYPE_DATA4, 1);
	HitBoxConstBufferData lData;
	lData.diameter = diameter;
	lData.xMax = threadNumData.x;
	lData.xyMax = threadNumData.x * threadNumData.y;
	computeHelper->TransData(hitBoxCommonHandle, &lData, sizeof(HitBoxConstBufferData));
}

void BBDuringEquallyCoordinatePlace::Compute()
{
	computeHelper->Compute(PIPELINE_COMPUTE_NAME_HITBOX_SETCIRCLE_IN_BB, threadNumData);
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
