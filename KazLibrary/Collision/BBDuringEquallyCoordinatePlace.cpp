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
	//�����z�u�o���邩�v�Z����
	KazMath::Vec3<float>lDistance = data.maxPos - data.minPos;

	threadNum.x = CalculatingDeployableNumber(lDistance.x, radius);
	threadNum.y = CalculatingDeployableNumber(lDistance.y, radius);
	threadNum.z = CalculatingDeployableNumber(lDistance.z, radius);

	BUFFER_SIZE lCountNum = static_cast<BUFFER_SIZE>(threadNum.x * threadNum.y * threadNum.z);
	BUFFER_SIZE lBufferSize = sizeof(MeshHitBoxData) * lCountNum;

	//�����̍��W���m�ۂł���悤�ɂ���
	hitBoxHandle = buffers.CreateBuffer(KazBufferHelper::SetRWStructuredBuffer(lBufferSize, "MeshCircleHitBoxBuffer"));
	hitBoxCommonHandle = buffers.CreateBuffer(KazBufferHelper::SetConstBufferData(sizeof(HitBoxConstBufferData), ""));

	HitBoxConstBufferData lData;
	lData.radius = radius;
	lData.xMax = threadNum.x;
	lData.xyMax = threadNum.x * threadNum.y;
	buffers.TransData(hitBoxCommonHandle, &lData, sizeof(HitBoxConstBufferData));
}

void BBDuringEquallyCoordinatePlace::Compute()
{
	//�v�Z����--------------------------------------------
	DescriptorHeapMgr::Instance()->SetDescriptorHeap();
	GraphicsPipeLineMgr::Instance()->SetComputePipeLineAndRootSignature(PIPELINE_COMPUTE_NAME_HITBOX_SETCIRCLE_IN_BB);

	//���_���
	DirectX12CmdList::Instance()->cmdList->SetComputeRootUnorderedAccessView(0, bbViewHandle);
	//�o��
	DirectX12CmdList::Instance()->cmdList->SetComputeRootUnorderedAccessView(1, buffers.GetGpuAddress(hitBoxHandle));

	DirectX12CmdList::Instance()->cmdList->SetComputeRootConstantBufferView(2, buffers.GetGpuAddress(hitBoxCommonHandle));

	DirectX12CmdList::Instance()->cmdList->Dispatch(threadNum.x, threadNum.y, threadNum.z);
	//�v�Z����--------------------------------------------


}

void BBDuringEquallyCoordinatePlace::DebugDraw()
{
	MeshHitBoxData *lData = (MeshHitBoxData *)buffers.GetMapAddres(hitBoxHandle);
	for (int i = 0; i < 10; ++i)
	{
		hitBoxArrayR[i].data.transform.pos = { lData->pos.x,lData->pos.y,lData->pos.z };
		hitBoxArrayR[i].data.transform.scale = { 0.01f,0.01f,0.01f };
		hitBoxArrayR[i].Draw();

		lData += sizeof(MeshHitBoxData);
	}
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
