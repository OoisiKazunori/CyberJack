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

int BBDuringEquallyCoordinatePlace::MeshIdNum = 0;

BBDuringEquallyCoordinatePlace::BBDuringEquallyCoordinatePlace(const ResouceBufferHelper::BufferData &BB_BUFFER_DATA, const BoundingBoxData &DATA, const ResouceBufferHelper::BufferData &STACK_MESH_CIRCLE_DATA) :data(DATA), countNum(0), debugFlag(false)
{
	computeHelper = std::make_unique<ResouceBufferHelper>();

	diameter = 5.0f;
	//�����z�u�o���邩�v�Z����
	KazMath::Vec3<float>lDistance = data.maxPos - data.minPos;

	threadNumData.x = CalculatingDeployableNumber(lDistance.x, diameter);
	threadNumData.y = CalculatingDeployableNumber(lDistance.y, diameter);
	threadNumData.z = CalculatingDeployableNumber(lDistance.z, diameter);

	countNum = static_cast<BUFFER_SIZE>(threadNumData.x * threadNumData.y * threadNumData.z);

	computeHelper->SetBuffer(BB_BUFFER_DATA, GRAPHICS_PRAMTYPE_DATA);

	//�����蔻����W�̗p��
	hitBoxDataHandle = computeHelper->CreateBuffer(
		KazBufferHelper::SetOnlyReadStructuredBuffer(sizeof(MeshHitBoxData) * countNum),
		GRAPHICS_RANGE_TYPE_UAV_DESC,
		GRAPHICS_PRAMTYPE_DATA2,
		sizeof(MeshHitBoxData),
		countNum
	);

	//���b�V�����̃X�^�b�N���
	computeHelper->SetBuffer(STACK_MESH_CIRCLE_DATA, GRAPHICS_PRAMTYPE_DATA3);


	//���O�Ɍv�Z���Ă������̗p��
	hitBoxCommonHandle = computeHelper->CreateBuffer(sizeof(HitBoxConstBufferData), GRAPHICS_RANGE_TYPE_CBV_VIEW, GRAPHICS_PRAMTYPE_DATA4, 1);
	HitBoxConstBufferData lData;
	lData.diameter = diameter;
	lData.xMax = threadNumData.x;
	lData.xyMax = threadNumData.x * threadNumData.y;
	lData.id = MeshIdNum;
	computeHelper->TransData(hitBoxCommonHandle, &lData, sizeof(HitBoxConstBufferData));

	++MeshIdNum;
}

void BBDuringEquallyCoordinatePlace::Compute()
{
	if (debugFlag)
	{
		computeHelper->StackToCommandListAndCallDispatch(PIPELINE_COMPUTE_NAME_HITBOX_SETCIRCLE_IN_BB_DEBUG, threadNumData);
	}
	else
	{
		computeHelper->StackToCommandListAndCallDispatch(PIPELINE_COMPUTE_NAME_HITBOX_SETCIRCLE_IN_BB, threadNumData);
	}
}

void BBDuringEquallyCoordinatePlace::SetDebugDraw(const ResouceBufferHelper::BufferData &STACK_DRAW_DATA)
{
	computeHelper->SetBuffer(
		STACK_DRAW_DATA,
		GRAPHICS_PRAMTYPE_DATA4
	);

	computeHelper->SetRootParam(
		hitBoxCommonHandle,
		GRAPHICS_PRAMTYPE_DATA5
	);

	debugFlag = true;
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
