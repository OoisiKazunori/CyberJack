#include "LineRender.h"

LineRender::LineRender()
{
	gpuBuffer = std::make_unique<CreateGpuBuffer>();

	positionDirtyFlag[0].reset(new DirtySet(data.startPos));
	positionDirtyFlag[1].reset(new DirtySet(data.endPos));

	cameraViewDirtyFlag.reset(new DirtySet(renderData.cameraMgrInstance->view));
	cameraProjectionDirtyFlag.reset(new DirtySet(renderData.cameraMgrInstance->perspectiveMat));
	cameraBillBoardDirtyFlag.reset(new DirtySet(renderData.cameraMgrInstance->billBoard));


	vertices[0].pos.x = 0;
	vertices[0].pos.y = 0;
	vertices[0].pos.z = 0;
	vertices[1].pos.x = 0;
	vertices[1].pos.y = 0;
	vertices[1].pos.z = 0;

	vertices[0].tickness.x = 0.5;
	vertices[0].tickness.y = 0.5;
	vertices[1].tickness.x = 0.5;
	vertices[1].tickness.y = 0.5;

	
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


	//ビューの設定-----------------------------------------------------------------------------------------------------
	vertexBufferView = KazBufferHelper::SetVertexBufferView(gpuBuffer->GetGpuAddress(vertexBufferHandle), vertByte, sizeof(vertices[0]));
	//ビューの設定-----------------------------------------------------------------------------------------------------

	baseMatWorldData.matWorld = XMMatrixIdentity();
}

LineRender::~LineRender()
{
}

void LineRender::Draw()
{
	//パイプライン設定-----------------------------------------------------------------------------------------------------
	pipeline = static_cast<PipeLineNames>(data.pipelineName);
	renderData.pipelineMgr->SetPipeLineAndRootSignature(pipeline);
	//パイプライン設定-----------------------------------------------------------------------------------------------------



	//DirtyFlag検知-----------------------------------------------------------------------------------------------------	
	bool positionDirtyFlag = this->positionDirtyFlag[0]->FloatDirty() || this->positionDirtyFlag[1]->FloatDirty();

	bool cameraMatDirtyFlag = cameraViewDirtyFlag->FloatDirty() || cameraProjectionDirtyFlag->FloatDirty() || cameraBillBoardDirtyFlag->FloatDirty();
	//DirtyFlag検知-----------------------------------------------------------------------------------------------------





	if (positionDirtyFlag)
	{
		vertices[0].pos = data.startPos.ConvertXMFLOAT3();
		vertices[1].pos = data.endPos.ConvertXMFLOAT3();
	}
	baseMatWorldData.matWorld *= data.motherMat;


	//バッファの転送-----------------------------------------------------------------------------------------------------
	//行列
	if (cameraMatDirtyFlag || true)
	{
		ConstBufferData constMap;
		constMap.world = baseMatWorldData.matWorld;
		constMap.view = renderData.cameraMgrInstance->GetViewMatrix(data.cameraIndex);
		constMap.viewproj = renderData.cameraMgrInstance->GetPerspectiveMatProjection();
		constMap.color = KazRenderHelper::SendColorDataToGPU(data.color);
		constMap.mat = constMap.world * constMap.view * constMap.viewproj;
		TransData(&constMap, constBufferHandle, typeid(constMap).name());
	}

	//頂点データに何か変更があったら転送する
	if (positionDirtyFlag)
	{
		gpuBuffer->TransData(vertexBufferHandle, vertices.data(), vertByte);
	}
	//バッファの転送-----------------------------------------------------------------------------------------------------


	//バッファをコマンドリストに積む-----------------------------------------------------------------------------------------------------
	SetConstBufferOnCmdList(pipeline);
	//バッファをコマンドリストに積む-----------------------------------------------------------------------------------------------------


	//描画命令-----------------------------------------------------------------------------------------------------
	renderData.cmdListInstance->cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);
	renderData.cmdListInstance->cmdList->IASetVertexBuffers(0, 1, &vertexBufferView);
	renderData.cmdListInstance->cmdList->DrawInstanced(static_cast<UINT>(vertices.size()), 1, 0, 0);
	//描画命令-----------------------------------------------------------------------------------------------------


	//DirtyFlagの更新-----------------------------------------------------------------------------------------------------
	this->positionDirtyFlag[0]->Record();
	this->positionDirtyFlag[1]->Record();

	cameraBillBoardDirtyFlag->Record();
	cameraProjectionDirtyFlag->Record();
	cameraViewDirtyFlag->Record();
	//DirtyFlagの更新-----------------------------------------------------------------------------------------------------
}
