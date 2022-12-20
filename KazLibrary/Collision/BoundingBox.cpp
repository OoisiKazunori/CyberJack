#include "BoundingBox.h"
#include"../KazLibrary/Buffer/DescriptorHeapMgr.h"

BoundingBox::BoundingBox(std::vector<DirectX::XMFLOAT4> VERT_DATA)
{
	//�ǂݍ��񂾃��f���̏�񂩂璸�_���W����肵�AVRAM�ɕۑ�����
	vertBufferHandle = computeBuffer.CreateBuffer(sizeof(DirectX::XMFLOAT4), GRAPHICS_RANGE_TYPE_UAV_VIEW, GRAPHICS_PRAMTYPE_DATA, static_cast<BUFFER_SIZE>(VERT_DATA.size()));
	computeBuffer.TransData(vertBufferHandle, VERT_DATA.data(), sizeof(DirectX::XMFLOAT4) * static_cast<BUFFER_SIZE>(VERT_DATA.size()));

	//BB���`�����鏈���p��
	bbBufferHandle = computeBuffer.CreateBuffer(sizeof(BoundingBoxBufferData), GRAPHICS_RANGE_TYPE_UAV_DESC, GRAPHICS_PRAMTYPE_DATA2, 1);
	bbViewHandle = computeBuffer.GetDescriptorViewHandle(bbBufferHandle);

	//���f����Transform�w��
	matBufferHandle = computeBuffer.CreateBuffer(sizeof(CommonData), GRAPHICS_RANGE_TYPE_CBV_VIEW, GRAPHICS_PRAMTYPE_DATA3, 1);
	DirectX::XMMATRIX lMat = KazMath::CaluWorld(KazMath::Transform3D({ 0.0f,0.0f,0.0f }, { 10.0f,10.0f,10.0f }, { 0.0f,0.0f,0.0f }), { 0.0f,1.0f,0.0f }, { 0.0f, 0.0f, 1.0f });

	CommonData lData;
	lData.scaleRotaMat = lMat;
	lData.vertNum = static_cast<UINT>(VERT_DATA.size());
	computeBuffer.TransData(matBufferHandle, &lData, sizeof(CommonData));
}

BoundingBoxData BoundingBox::GetData()
{
	BoundingBoxBufferData *lData = (BoundingBoxBufferData *)computeBuffer.GetMapAddress(bbBufferHandle);

	BoundingBoxData lTmpData;
	lTmpData.minPos = { lData->minPos.x, lData->minPos.y, lData->minPos.z };
	lTmpData.maxPos = { lData->maxPos.x, lData->maxPos.y, lData->maxPos.z };
	return lTmpData;
}

D3D12_GPU_DESCRIPTOR_HANDLE BoundingBox::GetViewHandle()
{
	return DescriptorHeapMgr::Instance()->GetGpuDescriptorView(bbViewHandle);
}

void BoundingBox::Compute()
{
	computeBuffer.Compute(PIPELINE_COMPUTE_NAME_HITBOX_BB, { 3,1,1 });
}