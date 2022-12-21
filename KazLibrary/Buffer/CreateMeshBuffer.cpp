#include "CreateMeshBuffer.h"
#include"../KazLibrary/Buffer/UavViewHandleMgr.h"
#include"../KazLibrary/Loader/FbxModelResourceMgr.h"
#include"../KazLibrary/Buffer/DescriptorHeapMgr.h"

CreateMeshBuffer::CreateMeshBuffer(RESOURCE_HANDLE HANDLE)
{
	if (HANDLE == -1)
	{
		FailCheck("���f�����ǂݍ��܂ꂹ��ł����B");
		assert(0);
	}


	//���_������������--------------------------------------------
	std::vector<DirectX::XMFLOAT3>lVertData = FbxModelResourceMgr::Instance()->GetResourceData(HANDLE)->vertFloat3Data;
	if (lVertData.size() == 0)
	{
		FailCheck("���_���W���ǂݍ��܂ꂹ��ł����B");
		assert(0);
	}
	bufferHandleDataArray[DATA_VERT].bufferHandle = computeHelper.CreateBuffer(
		KazBufferHelper::GetBufferSize<BUFFER_SIZE>(lVertData.size(), sizeof(DirectX::XMFLOAT3)),
		GRAPHICS_RANGE_TYPE_UAV_DESC,
		GRAPHICS_PRAMTYPE_DATA,
		static_cast<BUFFER_SIZE>(lVertData.size())
	);

	computeHelper.TransData(bufferHandleDataArray[DATA_VERT].bufferHandle, lVertData.data(), KazBufferHelper::GetBufferSize<BUFFER_SIZE>(lVertData.size(), sizeof(DirectX::XMFLOAT3)));

	bufferHandleDataArray[DATA_VERT].descriptorViewHandle = computeHelper.GetDescriptorViewHandle(bufferHandleDataArray[DATA_VERT].bufferHandle);
	//���_������������--------------------------------------------

	//UV������������--------------------------------------------
	std::vector<DirectX::XMFLOAT2>lUvData = FbxModelResourceMgr::Instance()->GetResourceData(HANDLE)->uvData;
	if (lUvData.size() == 0)
	{
		FailCheck("UV��񂪓ǂݍ��܂ꂹ��ł����B");
		assert(0);
	}

	bufferHandleDataArray[DATA_UV].bufferHandle = computeHelper.CreateBuffer(
		KazBufferHelper::GetBufferSize<BUFFER_SIZE>(lUvData.size(), sizeof(DirectX::XMFLOAT2)),
		GRAPHICS_RANGE_TYPE_UAV_DESC,
		GRAPHICS_PRAMTYPE_DATA2,
		static_cast<BUFFER_SIZE>(lUvData.size())
	);

	computeHelper.TransData(bufferHandleDataArray[DATA_UV].bufferHandle, lUvData.data(), KazBufferHelper::GetBufferSize<BUFFER_SIZE>(lUvData.size(), sizeof(DirectX::XMFLOAT2)));

	bufferHandleDataArray[DATA_UV].descriptorViewHandle = computeHelper.GetDescriptorViewHandle(bufferHandleDataArray[DATA_UV].bufferHandle);
	//UV������������--------------------------------------------



	//�@��������������--------------------------------------------
	std::vector<DirectX::XMFLOAT3>lNormalData = FbxModelResourceMgr::Instance()->GetResourceData(HANDLE)->normalData;
	if (lNormalData.size() == 0)
	{
		FailCheck("�@����񂪓ǂݍ��܂ꂹ��ł����B");
		assert(0);
	}
	bufferHandleDataArray[DATA_NORMAL].bufferHandle = computeHelper.CreateBuffer(
		KazBufferHelper::GetBufferSize<BUFFER_SIZE>(lNormalData.size(), sizeof(DirectX::XMFLOAT3)),
		GRAPHICS_RANGE_TYPE_UAV_DESC,
		GRAPHICS_PRAMTYPE_DATA3,
		static_cast<BUFFER_SIZE>(lNormalData.size())
	);

	computeHelper.TransData(bufferHandleDataArray[DATA_NORMAL].bufferHandle, lNormalData.data(), KazBufferHelper::GetBufferSize<BUFFER_SIZE>(lNormalData.size(), sizeof(DirectX::XMFLOAT3)));

	bufferHandleDataArray[DATA_NORMAL].descriptorViewHandle = computeHelper.GetDescriptorViewHandle(bufferHandleDataArray[DATA_NORMAL].bufferHandle);
	//�@��������������--------------------------------------------

}

const ComputeBufferHelper::BufferData &CreateMeshBuffer::GetBufferData(MeshBufferView ENUM_VIEW)
{
	RESOURCE_HANDLE lHandle = static_cast<RESOURCE_HANDLE>(ENUM_VIEW);
	return computeHelper.GetBufferData(lHandle);
}
