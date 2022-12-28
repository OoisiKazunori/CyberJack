#pragma once
#include"../Game/Helper/ComputeBufferHelper.h"
#include"../KazLibrary/Helper/ResouceBufferHelper.h"
#include"../KazLibrary/Render/DrawExcuteIndirect.h"
#include"../KazLibrary/Helper/KazRenderHelper.h"

struct InitMeshParticleData
{
	ComputeBufferHelper::BufferData vertData;
	ComputeBufferHelper::BufferData uvData;
	RESOURCE_HANDLE textureHandle;
	//x vertNum, y bias,z perTriangleNum,w faceCountNum
	DirectX::XMUINT4 triagnleData;
	const DirectX::XMMATRIX *motherMat;
	KazMath::Vec4<float>color;

	InitMeshParticleData() :textureHandle(-1)
	{
	}
};

class InstanceMeshParticle
{
public:
	InstanceMeshParticle(std::vector<InitMeshParticleData> &INIT_DATA);
	void Compute();

private:
	ResouceBufferHelper computeInitMeshParticle;
	RESOURCE_HANDLE vertHandle, uvHandle, meshDataAndColorHandle, colorHandle, meshParticleOutputHandle, meshParticleIDHandle;
	RESOURCE_HANDLE motherMatrixHandle,particlePosHandle, particleColorHandle,particleMotherMatrixHandle;
	RESOURCE_HANDLE scaleRotateBillboardMatHandle;

	ResouceBufferHelper computeUpdateMeshParticle;
	ResouceBufferHelper computeConvert;

	ComputeBufferHelper::BufferData commonAndColorBufferData;
	std::array<ComputeBufferHelper::BufferData,2> commonBufferData;
	ComputeBufferHelper::BufferData meshParticleBufferData;
	struct WorldMatData
	{
		DirectX::XMMATRIX scaleRotateBillboardMat;
	};
	struct InitOutputData
	{
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT4 color;
		UINT id;
	};
	struct CommonWithColorData
	{
		DirectX::XMUINT4 meshData;
		DirectX::XMFLOAT4 color;
		UINT id;
	};
	struct CommonData
	{
		DirectX::XMUINT4 meshData;
		UINT id;
	};

	static const int PARTICLE_MAX_NUM = 1000000;
	static const int VERT_BUFFER_SIZE = sizeof(DirectX::XMFLOAT3);
	static const int UV_BUFFER_SIZE = sizeof(DirectX::XMFLOAT2);
	static const int COMMON_BUFFER_SIZE = sizeof(CommonWithColorData);

	std::vector<const DirectX::XMMATRIX *>motherMatArray;

	enum InitPipelineType
	{
		INIT_POS,
		INIT_POS_UV,
		INIT_POS_UV_NORMAL,
	};
	void IsSetBuffer(const ComputeBufferHelper::BufferData &BUFFER_DATA)
	{
		if (BUFFER_DATA.bufferWrapper.buffer)
		{
			GraphicsRootParamType lType = static_cast<GraphicsRootParamType>(GRAPHICS_PRAMTYPE_DATA + setCountNum);
			computeInitMeshParticle.SetBuffer(BUFFER_DATA, lType);
			++setCountNum;
		}
	};
	int setCountNum;


	DirectX::XMMATRIX scaleRotMat;
	DirectX::XMMATRIX scaleRotBillBoardMat;

	std::vector<InitMeshParticleData> initData;


	KazRenderHelper::ID3D12ResourceWrapper copyBuffer;
	KazRenderHelper::ID3D12ResourceWrapper motherMatrixBuffer;
};

