#pragma once
#include"../KazLibrary/Buffer/CreateGpuBuffer.h"
#include"../KazLibrary/Pipeline/GraphicsPipeLineMgr.h"
#include"../KazLibrary/Math/KazMath.h"
#include"../KazLibrary/Helper/KazRenderHelper.h"
#include"../KazLibrary/Helper/KazBufferHelper.h"
#include"ComputeBufferHelper.h"

class ComputeBufferHelperPtr
{
public:
	struct BufferData
	{
		KazRenderHelper::ID3D12ResourceWrapper bufferWrapper;
		KazRenderHelper::ID3D12ResourceWrapper counterWrapper;
		GraphicsRangeType rangeType;
		GraphicsRootParamType rootParamType;
		UINT bufferSize;
		RESOURCE_HANDLE viewHandle;
		UINT elementNum;

		BufferData(const KazBufferHelper::BufferResourceData &BUFFER_DATA) :rangeType(GRAPHICS_RANGE_TYPE_NONE), rootParamType(GRAPHICS_PRAMTYPE_NONE), bufferSize(0), viewHandle(-1), elementNum(0)
		{
			bufferWrapper.CreateBuffer(BUFFER_DATA);
		}
		BufferData() :rangeType(GRAPHICS_RANGE_TYPE_NONE), rootParamType(GRAPHICS_PRAMTYPE_NONE), bufferSize(0), viewHandle(-1), elementNum(0)
		{
		}

		void operator=(const BufferData &rhs)
		{
			rangeType = rhs.rangeType;
			rootParamType = rhs.rootParamType;
			bufferSize = rhs.bufferSize;
			bufferWrapper = rhs.bufferWrapper;
			viewHandle = rhs.viewHandle;
			elementNum = rhs.elementNum;
		};
	};

	ComputeBufferHelperPtr();

	RESOURCE_HANDLE CreateBuffer(UINT STRUCTURE_BYTE_STRIDE, GraphicsRangeType RANGE, GraphicsRootParamType ROOTPARAM, UINT ELEMENT_NUM, bool GENERATE_COUNTER_BUFFER_FLAG = false);
	RESOURCE_HANDLE CreateBuffer(const KazBufferHelper::BufferResourceData &BUFFER_OPTION_DATA, GraphicsRangeType RANGE, GraphicsRootParamType ROOTPARAM, UINT STRUCTURE_BYTE_STRIDE, UINT ELEMENT_NUM, bool GENERATE_COUNTER_BUFFER_FLAG = false);
	BufferData CreateAndGetBuffer(UINT STRUCTURE_BYTE_STRIDE, GraphicsRangeType RANGE, GraphicsRootParamType ROOTPARAM, UINT ELEMENT_NUM, bool GENERATE_COUNTER_BUFFER_FLAG = false);
	BufferData CreateAndGetBuffer(const KazBufferHelper::BufferResourceData &BUFFER_OPTION_DATA, GraphicsRangeType RANGE, GraphicsRootParamType ROOTPARAM, UINT STRUCTURE_BYTE_STRIDE, UINT ELEMENT_NUM, bool GENERATE_COUNTER_BUFFER_FLAG = false);
	void TransData(RESOURCE_HANDLE HANDLE, void *TRANS_DATA, UINT TRANSMISSION_DATA_SIZE);
	void Compute(ComputePipeLineNames NAME, const DispatchCallData &DATA);

	//�ς܂�Ă���f�[�^�̖����ɊO���̃f�[�^���R�s�[���ăn���h����Ԃ�
	RESOURCE_HANDLE SetBuffer(const BufferData *BUFFER_DATA, GraphicsRootParamType ROOTPARAM);
	//�o�b�t�@�̒��g���R�s�[����݂̂ŋ��L�͂��Ȃ�
	RESOURCE_HANDLE CopyBuffer(const BufferData &BUFFER_DATA, GraphicsRootParamType ROOTPARAM);

	void *GetMapAddress(RESOURCE_HANDLE HANDLE);
	RESOURCE_HANDLE GetDescriptorViewHandle(RESOURCE_HANDLE HANDLE);
	const BufferData &GetBufferData(RESOURCE_HANDLE HANDLE);

	void ClearBuffer();
	void SetParamDataType(RESOURCE_HANDLE HANDLE, GraphicsRootParamType ROOTPARAM);

	void InitCounterBuffer(RESOURCE_HANDLE HANDLE);

private:
	HandleMaker handle;
	std::vector<const BufferData*>bufferArrayData;


	BufferData appendBuffer;
	BufferData testBuffer, testBuffer2;
	BufferData commonBuffer;


	std::array<DirectX::XMFLOAT3, 5> buffArray;

};