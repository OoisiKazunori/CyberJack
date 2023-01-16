#include "CollisionDetectionOfMeshCircleAndParticle.h"

CollisionDetectionOfMeshCircleAndCPUHitBox::CollisionDetectionOfMeshCircleAndCPUHitBox(
	const std::vector<Sphere> &CPU_HITBOX_DATA
) :sphereHitBoxArray(CPU_HITBOX_DATA)
{
	//BB当たり判定座標
	meshCircleArrayBufferHandle = computeHelper.CreateBuffer(
		KazBufferHelper::SetOnlyReadStructuredBuffer(sizeof(MeshHitBoxData) * 10000),
		GRAPHICS_RANGE_TYPE_UAV_DESC,
		GRAPHICS_PRAMTYPE_DATA,
		sizeof(MeshHitBoxData),
		10000,
		true);

	//CPU判定
	KazBufferHelper::BufferResourceData lData(
		CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE,
		CD3DX12_RESOURCE_DESC::Buffer(KazBufferHelper::GetBufferSize<BUFFER_SIZE>(sphereHitBoxArray.size(), sizeof(SphereData))),
		D3D12_RESOURCE_STATE_UNORDERED_ACCESS,
		nullptr,
		"VRAMSphereData"
	);
	lData.resourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;

	cpuHitBoxBufferHandle = computeHelper.CreateBuffer(
		lData,
		GRAPHICS_RANGE_TYPE_UAV_DESC,
		GRAPHICS_PRAMTYPE_DATA2,
		sizeof(SphereData),
		static_cast<UINT>(sphereHitBoxArray.size()),
		false
	);

	motherMatrixBuffer.CreateBuffer(
		KazBufferHelper::BufferResourceData(
			CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
			D3D12_HEAP_FLAG_NONE,
			CD3DX12_RESOURCE_DESC::Buffer(KazBufferHelper::GetBufferSize<BUFFER_SIZE>(sphereHitBoxArray.size(), sizeof(DirectX::XMMATRIX))),
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			"RAMmatData")
	);

	//当たった情報判定
	computeHelper.CreateBuffer(
		sizeof(HitIDData),
		GRAPHICS_RANGE_TYPE_UAV_DESC,
		GRAPHICS_PRAMTYPE_DATA3,
		1000,
		true
	);

	commonDataHandle = computeHelper.CreateBuffer(sizeof(CommonData), GRAPHICS_RANGE_TYPE_CBV_VIEW, GRAPHICS_PRAMTYPE_DATA4, 1);

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


	CommonData lCommonData;
	lCommonData.cpuHitBoxNum = 1;
	lCommonData.particleRadius = 0.5f;
	computeHelper.TransData(commonDataHandle, &lCommonData, sizeof(CommonData));
}

void CollisionDetectionOfMeshCircleAndCPUHitBox::Compute()
{
	//CPU当たり判定の転送ーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーー
	std::vector<SphereData>lMatArray(sphereHitBoxArray.size());
	for (int i = 0; i < lMatArray.size(); ++i)
	{
		lMatArray[i].pos = sphereHitBoxArray[i].center->ConvertXMFLOAT3();
		lMatArray[i].radius = sphereHitBoxArray[i].radius;
	}
	motherMatrixBuffer.TransData(lMatArray.data(), sizeof(DirectX::XMMATRIX) * static_cast<int>(lMatArray.size()));


	DirectX12CmdList::Instance()->cmdList->ResourceBarrier(
		1,
		&CD3DX12_RESOURCE_BARRIER::Transition(computeHelper.GetBufferData(cpuHitBoxBufferHandle).bufferWrapper.buffer.Get(),
			D3D12_RESOURCE_STATE_UNORDERED_ACCESS,
			D3D12_RESOURCE_STATE_COPY_DEST
		)
	);

	DirectX12CmdList::Instance()->cmdList->CopyResource(computeHelper.GetBufferData(cpuHitBoxBufferHandle).bufferWrapper.buffer.Get(), motherMatrixBuffer.buffer.Get());

	DirectX12CmdList::Instance()->cmdList->ResourceBarrier(
		1,
		&CD3DX12_RESOURCE_BARRIER::Transition(computeHelper.GetBufferData(cpuHitBoxBufferHandle).bufferWrapper.buffer.Get(),
			D3D12_RESOURCE_STATE_COPY_DEST,
			D3D12_RESOURCE_STATE_UNORDERED_ACCESS
		)
	);
	//CPU当たり判定の転送ーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーー


	computeHelper.InitCounterBuffer(copyBuffer.buffer);
	computeHelper.StackToCommandListAndCallDispatch(PIPELINE_COMPUTE_NAME_HITBOX_MESHCIRCLE_PARTICLE, { 100,1,1 });
}

const ResouceBufferHelper::BufferData &CollisionDetectionOfMeshCircleAndCPUHitBox::GetStackMeshCircleBuffer()
{
	return computeHelper.GetBufferData(meshCircleArrayBufferHandle);
}
