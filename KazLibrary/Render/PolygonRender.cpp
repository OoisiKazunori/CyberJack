#include "PolygonRender.h"
#include"../KazLibrary/Helper/KazHelper.h"

PolygonRender::PolygonRender(const array<SpriteVertex, 4> &DATA)
{
	anchorPoint = { 0.5f,0.5f };

	gpuBuffer = std::make_unique<CreateGpuBuffer>();

	positionDirtyFlag = std::make_unique<DirtySet>(data.transform.pos);
	scaleDirtyFlag = std::make_unique<DirtySet>(data.transform.scale);
	rotationDirtyFlag = std::make_unique<DirtySet>(data.transform.rotation);

	cameraViewDirtyFlag = std::make_unique<DirtySet>(renderData.cameraMgrInstance->view);
	cameraProjectionDirtyFlag = std::make_unique<DirtySet>(renderData.cameraMgrInstance->perspectiveMat);
	cameraBillBoardDirtyFlag = std::make_unique<DirtySet>(renderData.cameraMgrInstance->billBoard);

	sizeDirtyFlag = std::make_unique<DirtySet>(data.size);

	motherDirtyFlag = std::make_unique<DirtySet>(data.motherMat);


	//データの定義-----------------------------------------------------------------------------------------------------
	//頂点データ
	//KazRenderHelper::InitVerticesPos(&vertices[0].pos, &vertices[1].pos, &vertices[2].pos, &vertices[3].pos, anchorPoint);
	KazRenderHelper::InitUvPos(&vertices[0].uv, &vertices[1].uv, &vertices[2].uv, &vertices[3].uv);
	//インデックスデータ
	indices = KazRenderHelper::InitIndciesForPlanePolygon();
	//データの定義-----------------------------------------------------------------------------------------------------

	vertices = DATA;


	VertByte = vertices.size() * sizeof(SpriteVertex);
	IndexByte = indices.size() * sizeof(unsigned short);


	//バッファ生成-----------------------------------------------------------------------------------------------------
	//頂点バッファ
	vertexBufferHandle = gpuBuffer->CreateBuffer
	(
		KazBufferHelper::SetVertexBufferData(VertByte)
	);
	//インデックスバッファ
	indexBufferHandle = gpuBuffer->CreateBuffer
	(
		KazBufferHelper::SetIndexBufferData(IndexByte)
	);
	//定数バッファ
	constBufferHandle = CreateConstBuffer(sizeof(ConstBufferData), typeid(ConstBufferData).name(), GRAPHICS_RANGE_TYPE_CBV, GRAPHICS_PRAMTYPE_DRAW);
	//バッファ生成-----------------------------------------------------------------------------------------------------


	//バッファ転送-----------------------------------------------------------------------------------------------------
	gpuBuffer->TransData(vertexBufferHandle, vertices.data(), VertByte);
	gpuBuffer->TransData(indexBufferHandle, indices.data(), IndexByte);
	//バッファ転送-----------------------------------------------------------------------------------------------------


	//ビューの設定-----------------------------------------------------------------------------------------------------
	vertexBufferView = KazBufferHelper::SetVertexBufferView(gpuBuffer->GetGpuAddress(vertexBufferHandle), VertByte, sizeof(vertices[0]));
	indexBufferView = KazBufferHelper::SetIndexBufferView(gpuBuffer->GetGpuAddress(indexBufferHandle), IndexByte);
	//ビューの設定-----------------------------------------------------------------------------------------------------
}

void PolygonRender::Draw()
{

	//パイプライン設定-----------------------------------------------------------------------------------------------------
	pipeline = static_cast<PipeLineNames>(data.pipelineName);
	renderData.pipelineMgr->SetPipeLineAndRootSignature(pipeline);
	//パイプライン設定-----------------------------------------------------------------------------------------------------


	//DirtyFlag検知-----------------------------------------------------------------------------------------------------	
	bool matFlag = cameraViewDirtyFlag->FloatDirty() || cameraProjectionDirtyFlag->FloatDirty() || cameraBillBoardDirtyFlag->FloatDirty() || motherDirtyFlag->FloatDirty();
	bool matrixDirtyFlag = positionDirtyFlag->FloatDirty() || scaleDirtyFlag->FloatDirty() || rotationDirtyFlag->FloatDirty();
	bool scaleDirtyFlag = this->scaleDirtyFlag->FloatDirty();

	bool localsizeDirtyFlag = sizeDirtyFlag->FloatDirty();
	//DirtyFlag検知-----------------------------------------------------------------------------------------------------



	//行列計算-----------------------------------------------------------------------------------------------------
	if (matrixDirtyFlag || matFlag)
	{
		baseMatWorldData.matWorld = XMMatrixIdentity();
		baseMatWorldData.matScale = KazMath::CaluScaleMatrix(data.transform.scale);
		baseMatWorldData.matTrans = KazMath::CaluTransMatrix(data.transform.pos);
		baseMatWorldData.matRota = KazMath::CaluRotaMatrix(data.transform.rotation);
		//ビルボード行列を掛ける
		if (data.billBoardFlag)
		{
			baseMatWorldData.matWorld *= renderData.cameraMgrInstance->GetMatBillBoard();
		}
		baseMatWorldData.matWorld *= baseMatWorldData.matScale;
		baseMatWorldData.matWorld *= baseMatWorldData.matRota;
		baseMatWorldData.matWorld *= baseMatWorldData.matTrans;
		baseMatWorldData.matWorld *= data.motherMat;

		//親行列を掛ける
		motherMat = baseMatWorldData.matWorld;
	}
	//行列計算-----------------------------------------------------------------------------------------------------


	//バッファの転送-----------------------------------------------------------------------------------------------------
	//行列
	//if (matrixDirtyFlag || matFlag)
	//{
	ConstBufferData constMap;
	constMap.world = baseMatWorldData.matWorld;
	constMap.view = renderData.cameraMgrInstance->GetViewMatrix();
	constMap.viewproj = renderData.cameraMgrInstance->GetPerspectiveMatProjection();
	constMap.color = { 0.0f,0.0f,0.0f,data.alpha / 255.0f };
	constMap.mat = constMap.world * constMap.view * constMap.viewproj;

	//gpuBuffer->TransData();
	TransData(&constMap, constBufferHandle, typeid(ConstBufferData).name());
	//}
	//バッファの転送-----------------------------------------------------------------------------------------------------


	//バッファをコマンドリストに積む-----------------------------------------------------------------------------------------------------
	SetConstBufferOnCmdList(pipeline);
	//バッファをコマンドリストに積む-----------------------------------------------------------------------------------------------------

	//描画命令-----------------------------------------------------------------------------------------------------
	renderData.cmdListInstance->cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	renderData.cmdListInstance->cmdList->IASetVertexBuffers(0, 1, &vertexBufferView);
	renderData.cmdListInstance->cmdList->IASetIndexBuffer(&indexBufferView);
	renderData.cmdListInstance->cmdList->DrawIndexedInstanced(indices.size(), 1, 0, 0, 0);
	//描画命令-----------------------------------------------------------------------------------------------------



	//DirtyFlagの更新-----------------------------------------------------------------------------------------------------
	positionDirtyFlag->Record();
	this->scaleDirtyFlag->Record();
	rotationDirtyFlag->Record();
	sizeDirtyFlag->Record();

	cameraBillBoardDirtyFlag->Record();
	cameraProjectionDirtyFlag->Record();
	cameraViewDirtyFlag->Record();
	motherDirtyFlag->Record();
	//DirtyFlagの更新-----------------------------------------------------------------------------------------------------
}
