#include "ComputeParticleAvoidSphere.h"
#include"../KazLibrary/Render/GPUParticleRender.h"

ComputeParticleAvoidSphere::ComputeParticleAvoidSphere()
{
	//パーティクル情報の判定
	meshCircleArrayBufferHandle = computeHelper.CreateBuffer(
		KazBufferHelper::SetOnlyReadStructuredBuffer(sizeof(MeshHitBoxData) * 100000),
		GRAPHICS_RANGE_TYPE_UAV_DESC,
		GRAPHICS_PRAMTYPE_DATA,
		sizeof(MeshHitBoxData),
		100000,
		true);

	UINT lNum = 0;
	KazBufferHelper::BufferResourceData lBufferData
	(
		CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		CD3DX12_RESOURCE_DESC::Buffer(sizeof(UINT)),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		"CopyCounterBuffer"
	);
	copyBuffer.CreateBuffer(lBufferData);
	copyBuffer.TransData(&lNum, sizeof(UINT));

	computeHelper.InitCounterBuffer(copyBuffer.buffer);


	computeHelper.CreateBuffer(
		KazBufferHelper::SetOnlyReadStructuredBuffer(sizeof(DirectX::XMFLOAT3) * 100000),
		GRAPHICS_RANGE_TYPE_UAV_DESC,
		GRAPHICS_PRAMTYPE_DATA4,
		sizeof(DirectX::XMFLOAT3),
		100000);
}

void ComputeParticleAvoidSphere::SetHitIDBuffer(const ResouceBufferHelper::BufferData &HIT_ID_BUFFER)
{
	//何処のメッシュ球と判定を取ったか
	computeHelper.SetBuffer(HIT_ID_BUFFER, GRAPHICS_PRAMTYPE_DATA2);
	computeHelper.SetBuffer(GPUParticleRender::Instance()->GetStackBuffer(), GRAPHICS_PRAMTYPE_DATA3);
}

void ComputeParticleAvoidSphere::Compute()
{
	computeHelper.StackToCommandListAndCallDispatch(PIPELINE_COMPUTE_NAME_HITBOX_AVOID_PARTICLE, { 1000,1,1 });
}

const ResouceBufferHelper::BufferData &ComputeParticleAvoidSphere::GetStackParticleHitBoxBuffer()
{
	return computeHelper.GetBufferData(meshCircleArrayBufferHandle);
}
