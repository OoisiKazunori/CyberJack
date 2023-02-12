#pragma once
#include"../KazLibrary/Helper/ResouceBufferHelper.h"

class ComputeParticleAvoidSphere
{
public:
	ComputeParticleAvoidSphere();
	void SetHitIDBuffer(const ResouceBufferHelper::BufferData &HIT_ID_BUFFER);
	void Compute();

	const ResouceBufferHelper::BufferData &GetStackParticleHitBoxBuffer();
	const ResouceBufferHelper::BufferData &GetOutputParticleData();

private:
	ResouceBufferHelper computeHelper;
	RESOURCE_HANDLE meshCircleArrayBufferHandle, outputParticleBufferHandle;
	RESOURCE_HANDLE commonDataHandle;

	struct MeshHitBoxData
	{
		DirectX::XMFLOAT3 meshPos;
		UINT meshID;
		DirectX::XMUINT3 id;
		DirectX::XMFLOAT3 circlePos;

	};
	KazRenderHelper::ID3D12ResourceWrapper copyBuffer;

	static const int PARTICLE_MAX_NUM = 200000;

};

