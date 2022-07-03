#include "LineRender.h"

LineRender::LineRender()
{
	gpuBuffer = std::make_unique<CreateGpuBuffer>();

	vertices[0].pos.x = 0;
	vertices[0].pos.y = 0;
	vertices[0].pos.z = 0;
	vertices[1].pos.x = 0;
	vertices[1].pos.y = 0;
	vertices[1].pos.z = 0;

	vertices[0].tickness.x = 0.0f;
	vertices[0].tickness.y = 0.0f;
	vertices[1].tickness.x = 1.0f;
	vertices[1].tickness.y = 1.0f;

	
	vertByte = KazBufferHelper::GetBufferSize<UINT>(vertices.size(), sizeof(LineVertex));

	//バッファ生成-----------------------------------------------------------------------------------------------------
	//頂点バッファ
	vertexBufferHandle = gpuBuffer->CreateBuffer
	(
		KazBufferHelper::SetVertexBufferData(vertByte)
	);
	//定数バッファ
	constBufferHandle = CreateConstBuffer(sizeof(ConstBufferData), typeid(ConstBufferData).name(), GRAPHICS_RANGE_TYPE_CBV, GRAPHICS_PRAMTYPE_DRAW);
	//バッファ生成-----------------------------------------------------------------------------------------------------


	drawInstanceCommandData = KazRenderHelper::SetDrawInstanceCommandData(
		D3D_PRIMITIVE_TOPOLOGY_LINELIST,
		KazBufferHelper::SetVertexBufferView(gpuBuffer->GetGpuAddress(vertexBufferHandle), vertByte, sizeof(vertices[0])),
		static_cast<UINT>(vertices.size()),
		1
		);
}

LineRender::~LineRender()
{
}

void LineRender::Draw()
{
	//パイプライン設定-----------------------------------------------------------------------------------------------------
	renderData.pipelineMgr->SetPipeLineAndRootSignature(data.pipelineName);
	//パイプライン設定-----------------------------------------------------------------------------------------------------


	if (data.startPosDirtyFlag.Dirty() || data.endPosDirtyFlag.Dirty())
	{
		vertices[0].pos = data.startPos.ConvertXMFLOAT3();
		vertices[1].pos = data.endPos.ConvertXMFLOAT3();
	}
	if (data.motherMat.dirty.Dirty())
	{
		baseMatWorldData.matWorld = DirectX::XMMatrixIdentity();
		baseMatWorldData.matWorld *= data.motherMat.mat;
	}


	//バッファの転送-----------------------------------------------------------------------------------------------------
	//行列
	if (renderData.cameraMgrInstance->ViewAndProjDirty() || data.color.Dirty() || data.motherMat.dirty.Dirty() || data.cameraIndex.dirty.Dirty())
	{
		ConstBufferData constMap;
		constMap.world = baseMatWorldData.matWorld;
		constMap.view = renderData.cameraMgrInstance->GetViewMatrix(data.cameraIndex.id);
		constMap.viewproj = renderData.cameraMgrInstance->GetPerspectiveMatProjection();
		constMap.color = data.color.ConvertColorRateToXMFLOAT4();
		constMap.mat = constMap.world * constMap.view * constMap.viewproj;
		TransData(&constMap, constBufferHandle, typeid(constMap).name());
	}

	//頂点データに何か変更があったら転送する
	if (data.startPosDirtyFlag.Dirty() || data.endPosDirtyFlag.Dirty())
	{
		gpuBuffer->TransData(vertexBufferHandle, vertices.data(), vertByte);
	}
	//バッファの転送-----------------------------------------------------------------------------------------------------


	//バッファをコマンドリストに積む-----------------------------------------------------------------------------------------------------
	SetConstBufferOnCmdList(data.pipelineName);
	//バッファをコマンドリストに積む-----------------------------------------------------------------------------------------------------


	//描画命令-----------------------------------------------------------------------------------------------------
	DrawInstanceCommand(drawInstanceCommandData);
	//描画命令-----------------------------------------------------------------------------------------------------

	data.Record();
}