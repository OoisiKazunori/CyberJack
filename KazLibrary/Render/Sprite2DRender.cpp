#include "Sprite2DRender.h"
#include"../Buffer/DescriptorHeapMgr.h"
#include"../Helper/KazHelper.h"
#include"../RenderTarget/RenderTargetStatus.h"

Sprite2DRender::Sprite2DRender()
{
	anchorPoint = { 0.5f,0.5f };

	gpuBuffer.reset(new CreateGpuBuffer);

	positionDirtyFlag.reset(new DirtySet(data.transform.pos));
	scaleDirtyFlag.reset(new DirtySet(data.transform.scale));
	rotationDirtyFlag.reset(new DirtySet(data.transform.rotation));

	flipXDirtyFlag.reset(new DirtyFlag<bool>(&data.flip.x, false));
	flipYDirtyFlag.reset(new DirtyFlag<bool>(&data.flip.y, false));

	textureHandleDirtyFlag.reset(new DirtyFlag<short>(&data.handle));
	animationHandleDirtyFlag.reset(new DirtyFlag<short>(&data.animationHandle));


	cameraViewDirtyFlag.reset(new DirtySet(renderData.cameraMgrInstance->view));
	cameraProjectionDirtyFlag.reset(new DirtySet(renderData.cameraMgrInstance->perspectiveMat));
	cameraBillBoardDirtyFlag.reset(new DirtySet(renderData.cameraMgrInstance->billBoard));

	sizeDirtyFlag.reset(new DirtySet(data.size));

	alphaDrtyFlag = std::make_unique<DirtyFlag<float>>(&data.alpha);


	//データの定義-----------------------------------------------------------------------------------------------------
	//頂点データ
	KazRenderHelper::InitVerticesPos(&vertices[0].pos, &vertices[1].pos, &vertices[2].pos, &vertices[3].pos, anchorPoint);
	KazRenderHelper::InitUvPos(&vertices[0].uv, &vertices[1].uv, &vertices[2].uv, &vertices[3].uv);
	//インデックスデータ
	indices = KazRenderHelper::InitIndciesForPlanePolygon();
	//データの定義-----------------------------------------------------------------------------------------------------


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

Sprite2DRender::~Sprite2DRender()
{
}

void Sprite2DRender::Draw()
{
	//パイプライン設定-----------------------------------------------------------------------------------------------------
	pipeline = static_cast<PipeLineNames>(data.pipelineName);
	renderData.pipelineMgr->SetPipeLineAndRootSignature(pipeline);
	//パイプライン設定-----------------------------------------------------------------------------------------------------




	//DirtyFlag検知-----------------------------------------------------------------------------------------------------
	bool matFlag = cameraViewDirtyFlag->FloatDirty() || cameraProjectionDirtyFlag->FloatDirty() || cameraBillBoardDirtyFlag->FloatDirty();
	bool matrixDirtyFlag = positionDirtyFlag->FloatDirty() || scaleDirtyFlag->FloatDirty() || rotationDirtyFlag->FloatDirty();

	bool flipXDirtyFlag = this->flipXDirtyFlag->Dirty();
	bool flipYDirtyFlag = this->flipYDirtyFlag->Dirty();

	bool textureHandleDirtyFlag = this->textureHandleDirtyFlag->Dirty();
	bool animationHandleDirtyFlag = this->animationHandleDirtyFlag->Dirty();

	bool localsizeDirtyFlag = sizeDirtyFlag->FloatDirty();

	bool verticesDirtyFlag = flipXDirtyFlag || flipYDirtyFlag || textureHandleDirtyFlag || animationHandleDirtyFlag || localsizeDirtyFlag;
	//DirtyFlag検知-----------------------------------------------------------------------------------------------------



	//行列計算-----------------------------------------------------------------------------------------------------
	if (matrixDirtyFlag || matFlag)
	{
		baseMatWorldData.matWorld = XMMatrixIdentity();
		baseMatWorldData.matWorld *= XMMatrixRotationZ(XMConvertToRadians(data.transform.rotation));
		baseMatWorldData.matWorld *= XMMatrixTranslationFromVector(data.transform.pos);

		//親行列を掛ける
		motherMat = baseMatWorldData.matWorld;
	}
	//行列計算-----------------------------------------------------------------------------------------------------



	//頂点データの書き換えとUVの書き換え-----------------------------------------------------------------------------------------------------

	//読み込んだテクスチャのサイズ
	//読み込んだ画像のサイズを合わせる
	if (textureHandleDirtyFlag || matrixDirtyFlag || localsizeDirtyFlag)
	{
		//外部リソースのテクスチャサイズ
		if (DescriptorHeapMgr::Instance()->GetType(data.handle) != DESCRIPTORHEAP_MEMORY_TEXTURE_RENDERTARGET)
		{
			//サイズをXMFLOAT3からXMFLOAT2に直す
			XMFLOAT2 tmpSize = {};
			XMFLOAT4 tmpModiSize = {};
			//サイズをXMFLOAT3からXMFLOAT2に直す
			if (!data.changeSizeTypeFlag)
			{
				tmpSize = { data.transform.scale.m128_f32[0], data.transform.scale.m128_f32[1] };
			}
			else
			{
				tmpModiSize = data.size;
			}
			texSize.x = static_cast<float>(renderData.shaderResourceMgrInstance->GetTextureSize(data.handle).Width);
			texSize.y = static_cast<float>(renderData.shaderResourceMgrInstance->GetTextureSize(data.handle).Height);


			XMFLOAT2 leftUp, rightDown;
			leftUp = { 0.0f,0.0f };
			rightDown = { 1.0f,1.0f };

			//サイズ変更
			array<XMFLOAT2, 4>tmp;
			if (!data.changeSizeTypeFlag)
			{
				tmp = KazRenderHelper::ChangePlaneScale(leftUp, rightDown, tmpSize, anchorPoint, texSize);
			}
			else
			{
				tmp = KazRenderHelper::ChangeModiPlaneScale(leftUp, rightDown, tmpModiSize, anchorPoint, texSize);
			}
			
			for (int i = 0; i < tmp.size(); i++)
			{
				vertices[i].pos = { tmp[i].x,-tmp[i].y,0.0f };
			}
		}
		//レンダーターゲットのテクスチャサイズ
		else
		{
			//サイズをXMFLOAT3からXMFLOAT2に直す
			XMFLOAT2 tmpSize = { data.transform.scale.m128_f32[0], data.transform.scale.m128_f32[1] };

			texSize.x = static_cast<float>(RenderTargetStatus::Instance()->GetBufferData(data.handle)->GetDesc().Width);
			texSize.y = static_cast<float>(RenderTargetStatus::Instance()->GetBufferData(data.handle)->GetDesc().Height);

			XMFLOAT2 leftUp, rightDown;
			leftUp = { 0.0f,0.0f };
			rightDown = { 1.0f,1.0f };

			//サイズ変更
			array<XMFLOAT2, 4>tmp = KazRenderHelper::ChangePlaneScale(leftUp, rightDown, tmpSize, anchorPoint, texSize);
			for (int i = 0; i < tmp.size(); i++)
			{
				vertices[i].pos = { tmp[i].x,-tmp[i].y,0.0f };
			}
		}
	}

	//UV切り取り
	if (animationHandleDirtyFlag || matrixDirtyFlag)
	{
		XMFLOAT2 divSize = renderData.shaderResourceMgrInstance->GetDivData(data.handle).divSize;
		XMFLOAT2 tmpSize = { data.transform.scale.m128_f32[0], data.transform.scale.m128_f32[1] };


		bool isItSafeToUseAnimationHandleFlag = KazHelper::IsitInAnArray(data.animationHandle, renderData.shaderResourceMgrInstance->GetDivData(data.handle).divLeftUp.size());
		bool isItSafeToUseDivDataFlag = (divSize.x != -1 && divSize.y != -1);


		if (isItSafeToUseDivDataFlag && isItSafeToUseAnimationHandleFlag)
		{
			XMFLOAT2 divLeftUpPos = renderData.shaderResourceMgrInstance->GetDivData(data.handle).divLeftUp[data.animationHandle];
			KazRenderHelper::VerticesCut(divSize, divLeftUpPos, &vertices[0].pos, &vertices[1].pos, &vertices[2].pos, &vertices[3].pos, tmpSize, anchorPoint);
			KazRenderHelper::UVCut(divLeftUpPos, divSize, texSize, &vertices[0].uv, &vertices[1].uv, &vertices[2].uv, &vertices[3].uv);

			for (int i = 0; i < vertices.size(); i++)
			{
				vertices[i].pos.y *= -1.0f;
			}
		}
	}

	//X軸にUVを反転
	if (flipXDirtyFlag)
	{
		KazRenderHelper::FlipXUv(&vertices[0].uv, &vertices[1].uv, &vertices[2].uv, &vertices[3].uv);
	}
	//Y軸にUVを反転
	if (flipYDirtyFlag)
	{
		KazRenderHelper::FlipYUv(&vertices[0].uv, &vertices[1].uv, &vertices[2].uv, &vertices[3].uv);
	}



	//頂点データに何か変更があったら転送する
	if (verticesDirtyFlag || matrixDirtyFlag)
	{
		gpuBuffer->TransData(vertexBufferHandle, vertices.data(), VertByte);
	}
	//頂点データの書き換えとUVの書き換え-----------------------------------------------------------------------------------------------------


	//バッファの転送-----------------------------------------------------------------------------------------------------
	//行列
	if (matrixDirtyFlag || matFlag || alphaDrtyFlag->Dirty())
	{
		ConstBufferData constMap;
		constMap.world = baseMatWorldData.matWorld;
		constMap.view = renderData.cameraMgrInstance->GetViewMatrix();
		constMap.viewproj = renderData.cameraMgrInstance->orthographicMatProjection;
		constMap.color = { 0.0f,0.0f,0.0f,data.alpha / 255.0f };
		constMap.mat = constMap.world * constMap.viewproj;

		//gpuBuffer->TransData();
		TransData(&constMap, constBufferHandle, typeid(ConstBufferData).name());
	}
	//バッファの転送-----------------------------------------------------------------------------------------------------


	//バッファをコマンドリストに積む-----------------------------------------------------------------------------------------------------
	SetConstBufferOnCmdList(pipeline);


	if (data.handle != -1)
	{
		D3D12_GPU_DESCRIPTOR_HANDLE gpuDescHandleSRV = DescriptorHeapMgr::Instance()->GetGpuDescriptorView(data.handle);
		int param = KazRenderHelper::SetBufferOnCmdList(GraphicsRootSignature::Instance()->GetRootParam(renderData.pipelineMgr->GetRootSignatureName(pipeline)), GRAPHICS_RANGE_TYPE_SRV, GRAPHICS_PRAMTYPE_TEX);
		renderData.cmdListInstance->cmdList->SetGraphicsRootDescriptorTable(param, gpuDescHandleSRV);
	}

	for (int i = 0; i < data.addHandle.handle.size(); ++i)
	{
		renderData.shaderResourceMgrInstance->SetSRV(data.addHandle.handle[i], GraphicsRootSignature::Instance()->GetRootParam(renderData.pipelineMgr->GetRootSignatureName(pipeline)), data.addHandle.paramType[i]);
	}
	//バッファをコマンドリストに積む-----------------------------------------------------------------------------------------------------




	//描画命令-----------------------------------------------------------------------------------------------------
	renderData.cmdListInstance->cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	renderData.cmdListInstance->cmdList->IASetVertexBuffers(0, 1, &vertexBufferView);
	renderData.cmdListInstance->cmdList->IASetIndexBuffer(&indexBufferView);
	renderData.cmdListInstance->cmdList->DrawIndexedInstanced(indices.size(), 1, 0, 0, 0);
	//描画命令-----------------------------------------------------------------------------------------------------



	//DirtyFlagの更新-----------------------------------------------------------------------------------------------------
	positionDirtyFlag->Record();
	scaleDirtyFlag->Record();
	rotationDirtyFlag->Record();
	this->flipXDirtyFlag->Record();
	this->flipYDirtyFlag->Record();
	this->textureHandleDirtyFlag->Record();
	this->animationHandleDirtyFlag->Record();

	cameraBillBoardDirtyFlag->Record();
	cameraProjectionDirtyFlag->Record();
	cameraViewDirtyFlag->Record();

	alphaDrtyFlag->Record();

	sizeDirtyFlag->Record();
	//DirtyFlagの更新-----------------------------------------------------------------------------------------------------
}

XMMATRIX Sprite2DRender::GetMotherMatrix()
{
	return motherMat;
}
