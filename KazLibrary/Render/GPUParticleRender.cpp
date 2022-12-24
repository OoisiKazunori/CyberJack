#include "GPUParticleRender.h"

GPUParticleRender::GPUParticleRender()
{
	worldMatHandle = computeCovertWorldMatToDrawMat.CreateBuffer(
		KazBufferHelper::SetOnlyReadStructuredBuffer(sizeof(DirectX::XMMATRIX) * PARTICLE_MAX_NUM),
		GRAPHICS_RANGE_TYPE_UAV_DESC,
		GRAPHICS_PRAMTYPE_DATA,
		sizeof(DirectX::XMMATRIX),
		PARTICLE_MAX_NUM,
		true);

	colorHandle = computeCovertWorldMatToDrawMat.CreateBuffer(
		KazBufferHelper::SetOnlyReadStructuredBuffer(sizeof(DirectX::XMFLOAT4) * PARTICLE_MAX_NUM),
		GRAPHICS_RANGE_TYPE_UAV_DESC,
		GRAPHICS_PRAMTYPE_DATA2,
		sizeof(DirectX::XMFLOAT4),
		PARTICLE_MAX_NUM,
		true);

	outputHandle = computeCovertWorldMatToDrawMat.CreateBuffer(
		KazBufferHelper::SetOnlyReadStructuredBuffer(sizeof(OutputData) * PARTICLE_MAX_NUM),
		GRAPHICS_RANGE_TYPE_UAV_DESC,
		GRAPHICS_PRAMTYPE_DATA3,
		sizeof(OutputData),
		PARTICLE_MAX_NUM,
		true);

	viewProjMatHandle = computeCovertWorldMatToDrawMat.CreateBuffer(
		sizeof(DirectX::XMMATRIX),
		GRAPHICS_RANGE_TYPE_CBV_VIEW,
		GRAPHICS_PRAMTYPE_DATA3,
		1,
		false);






	std::array<Vertex, 4>lVerticesArray;
	std::array<USHORT, 6> lIndicesArray;
	lIndicesArray = KazRenderHelper::InitIndciesForPlanePolygon();
	KazRenderHelper::InitVerticesPos(&lVerticesArray[0].pos, &lVerticesArray[1].pos, &lVerticesArray[2].pos, &lVerticesArray[3].pos, { 0.5f,0.5f });
	KazRenderHelper::InitUvPos(&lVerticesArray[0].uv, &lVerticesArray[1].uv, &lVerticesArray[2].uv, &lVerticesArray[3].uv);

	BUFFER_SIZE lVertBuffSize = KazBufferHelper::GetBufferSize<BUFFER_SIZE>(lVerticesArray.size(), sizeof(Vertex));
	BUFFER_SIZE lIndexBuffSize = KazBufferHelper::GetBufferSize<BUFFER_SIZE>(lIndicesArray.size(), sizeof(UINT));


	vertexBuffer = std::make_unique<KazRenderHelper::ID3D12ResourceWrapper>();
	indexBuffer = std::make_unique<KazRenderHelper::ID3D12ResourceWrapper>();

	vertexBuffer->CreateBuffer(KazBufferHelper::SetVertexBufferData(lVertBuffSize));
	indexBuffer->CreateBuffer(KazBufferHelper::SetIndexBufferData(lIndexBuffSize));

	vertexBuffer->TransData(lVerticesArray.data(), lVertBuffSize);
	indexBuffer->TransData(lIndicesArray.data(), lIndexBuffSize);



	InitDrawIndexedExcuteIndirect lInitData;
	lInitData.vertexBufferView = KazBufferHelper::SetVertexBufferView(vertexBuffer->GetGpuAddress(), lVertBuffSize, sizeof(lVerticesArray[0]));
	lInitData.indexBufferView = KazBufferHelper::SetIndexBufferView(indexBuffer->GetGpuAddress(), lIndexBuffSize);
	lInitData.indexNum = static_cast<UINT>(lIndicesArray.size());
	//lInitData.elementNum = countNum;
	//lInitData.updateView = computeHelper.GetBufferData(matHandle).bufferWrapper.buffer->GetGPUVirtualAddress();
	lInitData.rootsignatureName = ROOTSIGNATURE_DATA_DRAW_UAV;

	std::array<D3D12_INDIRECT_ARGUMENT_DESC, 2> args{};
	args[0].Type = D3D12_INDIRECT_ARGUMENT_TYPE_UNORDERED_ACCESS_VIEW;
	args[0].UnorderedAccessView.RootParameterIndex = 0;
	args[1].Type = D3D12_INDIRECT_ARGUMENT_TYPE_DRAW_INDEXED;

	lInitData.argument.push_back(args[0]);
	lInitData.argument.push_back(args[1]);
	excuteIndirect = std::make_unique<DrawExcuteIndirect>(lInitData);
}

void GPUParticleRender::Draw()
{
	computeCovertWorldMatToDrawMat.InitCounterBuffer(worldMatHandle);
	computeCovertWorldMatToDrawMat.InitCounterBuffer(outputHandle);
	computeCovertWorldMatToDrawMat.Compute(PIPELINE_COMPUTE_NAME_CONVERT_WORLDMAT_TO_DRAWMAT, { 100,1,1 });
	excuteIndirect->Draw(PIPELINE_NAME_GPUPARTICLE);
}

const ComputeBufferHelper::BufferData &GPUParticleRender::GetStackWorldMatBuffer()
{
	return computeCovertWorldMatToDrawMat.GetBufferData(worldMatHandle);
}

const ComputeBufferHelper::BufferData &GPUParticleRender::GetStackColorBuffer()
{
	return computeCovertWorldMatToDrawMat.GetBufferData(colorHandle);
}
