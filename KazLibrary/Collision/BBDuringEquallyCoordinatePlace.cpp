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

	threadNum.x = CalculatingDeployableNumber(lDistance.x, radius);
	threadNum.y = CalculatingDeployableNumber(lDistance.y, radius);
	threadNum.z = CalculatingDeployableNumber(lDistance.z, radius);

	BUFFER_SIZE lCountNum = static_cast<BUFFER_SIZE>(threadNum.x * threadNum.y * threadNum.z);
	BUFFER_SIZE lHitBoxPosBufferSize = sizeof(DirectX::XMFLOAT3) * lCountNum;
	BUFFER_SIZE lHitBoxIDBufferSize = sizeof(DirectX::XMUINT3) * lCountNum;

	//個数分の座標を確保できるようにする
	hitBoxPosHandle = buffers.CreateBuffer(KazBufferHelper::SetRWStructuredBuffer(lHitBoxPosBufferSize, "MeshCircleHitBoxPosBuffer"));
	hitBoxIDHandle = buffers.CreateBuffer(KazBufferHelper::SetRWStructuredBuffer(lHitBoxIDBufferSize, "MeshCircleHitBoxIDBuffer"));
	hitBoxCommonHandle = buffers.CreateBuffer(KazBufferHelper::SetConstBufferData(sizeof(HitBoxConstBufferData), "HitBoxConstBufferData"));

	HitBoxConstBufferData lData;
	lData.radius = radius;
	lData.xMax = threadNum.x;
	lData.xyMax = threadNum.x * threadNum.y;
	buffers.TransData(hitBoxCommonHandle, &lData, sizeof(HitBoxConstBufferData));


	hitBoxViewHandle = UavViewHandleMgr::Instance()->GetHandle();
	DescriptorHeapMgr::Instance()->CreateBufferView(
		hitBoxViewHandle,
		KazBufferHelper::SetUnorderedAccessView(sizeof(DirectX::XMFLOAT3), lCountNum),
		buffers.GetBufferData(hitBoxPosHandle).Get(),
		nullptr
	);

	hitBoxIDViewHandle = UavViewHandleMgr::Instance()->GetHandle();
	DescriptorHeapMgr::Instance()->CreateBufferView(
		hitBoxIDViewHandle,
		KazBufferHelper::SetUnorderedAccessView(sizeof(DirectX::XMUINT3), lCountNum),
		buffers.GetBufferData(hitBoxIDHandle).Get(),
		nullptr
	);
}

void BBDuringEquallyCoordinatePlace::Compute()
{
	//計算処理--------------------------------------------
	DescriptorHeapMgr::Instance()->SetDescriptorHeap();
	GraphicsPipeLineMgr::Instance()->SetComputePipeLineAndRootSignature(PIPELINE_COMPUTE_NAME_HITBOX_SETCIRCLE_IN_BB);

	//BB
	DirectX12CmdList::Instance()->cmdList->SetComputeRootDescriptorTable(0, bbViewHandle);
	//当たり判定座標
	DirectX12CmdList::Instance()->cmdList->SetComputeRootDescriptorTable(1, DescriptorHeapMgr::Instance()->GetGpuDescriptorView(hitBoxViewHandle));
	//当たり判定ID
	DirectX12CmdList::Instance()->cmdList->SetComputeRootDescriptorTable(2, DescriptorHeapMgr::Instance()->GetGpuDescriptorView(hitBoxIDViewHandle));
	//共通
	DirectX12CmdList::Instance()->cmdList->SetComputeRootConstantBufferView(3, buffers.GetGpuAddress(hitBoxCommonHandle));

	DirectX12CmdList::Instance()->cmdList->Dispatch(threadNum.x, threadNum.y, threadNum.z);
	//計算処理--------------------------------------------
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
