#pragma once
#include"../KazLibrary/Helper/ResouceBufferHelper.h"

class ComputeParticleAvoidSphere
{
public:
	ComputeParticleAvoidSphere();
	void SetHitIDBuffer(const ResouceBufferHelper::BufferData &HIT_ID_BUFFER);
	void Compute();

	const ResouceBufferHelper::BufferData &GetStackParticleHitBoxBuffer();

private:
	ResouceBufferHelper computeHelper;
	RESOURCE_HANDLE meshCircleArrayBufferHandle;
	RESOURCE_HANDLE commonDataHandle;

	struct MeshHitBoxData
	{
		DirectX::XMFLOAT3 pos;
		UINT meshID;
		DirectX::XMUINT3 id;
	};
	KazRenderHelper::ID3D12ResourceWrapper copyBuffer;

};

