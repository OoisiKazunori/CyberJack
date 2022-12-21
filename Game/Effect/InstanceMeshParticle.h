#pragma once
#include"../Game/Helper/ComputeBufferHelper.h"
#include"../KazLibrary/Render/DrawExcuteIndirect.h"
#include"../KazLibrary/Helper/KazRenderHelper.h"

struct InitMeshParticleData
{
	std::unique_ptr<ComputeBufferHelper::BufferData> vertData;
	std::unique_ptr<ComputeBufferHelper::BufferData> uvData;
	RESOURCE_HANDLE textureViewHandle;
	DirectX::XMUINT4 triagnleData;
	const DirectX::XMMATRIX *motherMat;

	InitMeshParticleData(const DirectX::XMMATRIX *MOTHER_MAT) 
		:motherMat(MOTHER_MAT), 
		vertData(std::make_unique<ComputeBufferHelper::BufferData>()),
		uvData(std::make_unique<ComputeBufferHelper::BufferData>())
	{
	}
};

class InstanceMeshParticle
{
public:
	InstanceMeshParticle(std::vector<InitMeshParticleData> &INIT_DATA);
	void Compute();

private:
	ComputeBufferHelper computeInitMeshParticle;
	RESOURCE_HANDLE vertHandle, uvHandle, commonHandle, meshParticleOutputHandle, meshParticleIDHandle;
	RESOURCE_HANDLE motherMatrixHandle,particlePosHandle, particleColorHandle,particleMotherMatrixHandle;
	ComputeBufferHelper computeUpdateMeshParticle;

	static const int PARTICLE_MAX_NUM = 3000000;
	static const int VERT_BUFFER_SIZE = sizeof(DirectX::XMFLOAT3);
	static const int UV_BUFFER_SIZE = sizeof(DirectX::XMFLOAT2);
	static const int COMMON_BUFFER_SIZE = sizeof(DirectX::XMUINT4);

	struct CameraData
	{
		DirectX::XMMATRIX scaleRotateBillboardMat;
		DirectX::XMMATRIX viewProjection;
	};
	struct InitOutputData
	{
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT4 color;
		UINT id;
	};

	std::vector<const DirectX::XMMATRIX *>motherMatArray;

};

