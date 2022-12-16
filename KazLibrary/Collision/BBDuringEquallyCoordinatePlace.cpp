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

BBDuringEquallyCoordinatePlace::BBDuringEquallyCoordinatePlace(D3D12_GPU_VIRTUAL_ADDRESS BB_BUFFER_HANDLE, const BoundingBoxData &DATA) :data(DATA), bbViewHandle(BB_BUFFER_HANDLE)
{
	radius = 0.1f;
	//いくつ配置出来るか計算する
	KazMath::Vec3<float>lDistance = data.maxPos - data.minPos;

	threadNum.x = CalculatingDeployableNumber(lDistance.x, radius);
	threadNum.y = CalculatingDeployableNumber(lDistance.y, radius);
	threadNum.z = CalculatingDeployableNumber(lDistance.z, radius);

	BUFFER_SIZE lCountNum = static_cast<BUFFER_SIZE>(threadNum.x * threadNum.y * threadNum.z);
	BUFFER_SIZE lBufferSize = sizeof(MeshHitBoxData) * lCountNum;

	//個数分の座標を確保できるようにする
	hitBoxHandle = buffers.CreateBuffer(KazBufferHelper::SetRWStructuredBuffer(lBufferSize, "MeshCircleHitBoxBuffer"));
	hitBoxCommonHandle = buffers.CreateBuffer(KazBufferHelper::SetConstBufferData(sizeof(HitBoxConstBufferData), ""));

	HitBoxConstBufferData lData;
	lData.radius = radius;
	lData.xMax = threadNum.x;
	lData.xyMax = threadNum.x * threadNum.y;
	buffers.TransData(hitBoxCommonHandle, &lData, sizeof(HitBoxConstBufferData));



	//デバック用の描画-------------------------------------------------------------------------------------
	InitExcuteIndirect lInitData;
	lInitData.elementNum = lCountNum;
	RESOURCE_HANDLE lSphereHandle = FbxModelResourceMgr::Instance()->LoadModel(KazFilePathName::TestPath + "sphere.fbx");
	lInitData.vertexBufferView = FbxModelResourceMgr::Instance()->GetResourceData(lSphereHandle)->vertexBufferView;
	lInitData.updateView = buffers.GetGpuAddress(hitBoxHandle);
	lInitData.rootsignatureName = ROOTSIGNATURE_DATA_DRAW_UAV;

	std::array<D3D12_INDIRECT_ARGUMENT_DESC, 2> args{};
	args[0].Type = D3D12_INDIRECT_ARGUMENT_TYPE_UNORDERED_ACCESS_VIEW;
	args[0].UnorderedAccessView.RootParameterIndex = 0;
	args[1].Type = D3D12_INDIRECT_ARGUMENT_TYPE_DRAW;

	lInitData.argument.push_back(args[0]);
	lInitData.argument.push_back(args[1]);
	excuteIndirect = std::make_unique<DrawExcuteIndirect>(lInitData);
	//デバック用の描画-------------------------------------------------------------------------------------
}

void BBDuringEquallyCoordinatePlace::Compute()
{
	//計算処理--------------------------------------------
	DescriptorHeapMgr::Instance()->SetDescriptorHeap();
	GraphicsPipeLineMgr::Instance()->SetComputePipeLineAndRootSignature(PIPELINE_COMPUTE_NAME_HITBOX_SETCIRCLE_IN_BB);

	//頂点情報
	DirectX12CmdList::Instance()->cmdList->SetComputeRootUnorderedAccessView(0, bbViewHandle);
	//出力
	DirectX12CmdList::Instance()->cmdList->SetComputeRootUnorderedAccessView(1, buffers.GetGpuAddress(hitBoxHandle));

	DirectX12CmdList::Instance()->cmdList->SetComputeRootConstantBufferView(2, buffers.GetGpuAddress(hitBoxCommonHandle));

	DirectX12CmdList::Instance()->cmdList->Dispatch(threadNum.x, threadNum.y, threadNum.z);
	//計算処理--------------------------------------------
}

void BBDuringEquallyCoordinatePlace::DebugDraw()
{
	excuteIndirect->Draw(PIPELINE_NAME_GPUPARTICLE);
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
