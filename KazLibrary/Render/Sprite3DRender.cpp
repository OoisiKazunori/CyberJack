#include "Sprite3DRender.h"
#include"../Helper/OutPutDebugStringAndCheckResult.h"
#include"../Helper/KazRenderHelper.h"
#include"../Helper/KazHelper.h"
#include"../Buffer/DescriptorHeapMgr.h"
#include"../RenderTarget/RenderTargetStatus.h"

Sprite3DRender::Sprite3DRender(const KazMath::Vec2<float> ANCHOR_POINT)
{
	anchorPoint = ANCHOR_POINT;

	gpuBuffer = std::make_unique<CreateGpuBuffer>();



	//データの定義-----------------------------------------------------------------------------------------------------
	//頂点データ
	KazRenderHelper::InitVerticesPos(&vertices[0].pos, &vertices[1].pos, &vertices[2].pos, &vertices[3].pos, anchorPoint.ConvertXMFLOAT2());
	KazRenderHelper::InitUvPos(&vertices[0].uv, &vertices[1].uv, &vertices[2].uv, &vertices[3].uv);
	//インデックスデータ
	indices = KazRenderHelper::InitIndciesForPlanePolygon();
	//データの定義-----------------------------------------------------------------------------------------------------

	
	VertByte = KazBufferHelper::GetBufferSize<UINT>(vertices.size(), sizeof(SpriteVertex));
	IndexByte = KazBufferHelper::GetBufferSize<UINT>(indices.size(), sizeof(unsigned short));


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

Sprite3DRender::~Sprite3DRender()
{
}

void Sprite3DRender::Draw()
{
	//パイプライン設定-----------------------------------------------------------------------------------------------------
	pipeline = static_cast<PipeLineNames>(data.pipelineName);
	renderData.pipelineMgr->SetPipeLineAndRootSignature(pipeline);
	//パイプライン設定-----------------------------------------------------------------------------------------------------




	//DirtyFlag検知-----------------------------------------------------------------------------------------------------	
	//DirtyFlag検知-----------------------------------------------------------------------------------------------------



	//行列計算-----------------------------------------------------------------------------------------------------
	{
		baseMatWorldData.matWorld = XMMatrixIdentity();
		baseMatWorldData.matScale = KazMath::CaluScaleMatrix(data.transform.scale);
		baseMatWorldData.matTrans = KazMath::CaluTransMatrix(data.transform.pos);
		baseMatWorldData.matRota = KazMath::CaluRotaMatrix(data.transform.rotation);


		baseMatWorldData.matWorld *= baseMatWorldData.matScale;
		baseMatWorldData.matWorld *= baseMatWorldData.matRota;
		//ビルボード行列を掛ける
		if (data.billBoardFlag)
		{
			baseMatWorldData.matWorld *= renderData.cameraMgrInstance->GetMatBillBoard(data.cameraIndex);
		}
		baseMatWorldData.matWorld *= baseMatWorldData.matTrans;
		baseMatWorldData.matWorld *= data.motherMat;


		//親行列を掛ける
		motherMat = baseMatWorldData.matWorld;
	}
	//行列計算-----------------------------------------------------------------------------------------------------



	//頂点データの書き換えとUVの書き換え-----------------------------------------------------------------------------------------------------

	//読み込んだテクスチャのサイズ
	//読み込んだ画像のサイズを合わせる
	{
		if (DescriptorHeapMgr::Instance()->GetType(data.handle) != DESCRIPTORHEAP_MEMORY_TEXTURE_RENDERTARGET)
		{

			KazMath::Vec2<float> tmpSize = {};
			XMFLOAT4 tmpModiSize = {};
			//サイズをXMFLOAT3からXMFLOAT2に直す
			if (!data.changeSizeTypeFlag)
			{
				tmpSize = { data.transform.scale.x, data.transform.scale.y };
			}
			else
			{
				tmpModiSize = data.size;
			}


			texSize.x = static_cast<int>(renderData.shaderResourceMgrInstance->GetTextureSize(data.handle).Width);
			texSize.y = static_cast<int>(renderData.shaderResourceMgrInstance->GetTextureSize(data.handle).Height);


			KazMath::Vec2<float> leftUp, rightDown;
			leftUp = { 0.0f,0.0f };
			rightDown = { 1.0f,1.0f };

			array<KazMath::Vec2<float>, 4>tmp;
			//サイズ変更
			if (!data.changeSizeTypeFlag)
			{
				tmp = KazRenderHelper::ChangePlaneScale(leftUp, rightDown, tmpSize, anchorPoint, texSize);
			}
			else
			{
				tmp = KazRenderHelper::ChangeModiPlaneScale(leftUp, rightDown, tmpModiSize, anchorPoint.ConvertXMFLOAT2(), texSize.ConvertXMFLOAT2());
			}


			for (int i = 0; i < tmp.size(); i++)
			{
				vertices[i].pos = { tmp[i].x,tmp[i].y,0.0f };
			}

		}
		//レンダーターゲットのテクスチャサイズ
		else
		{
			//サイズをXMFLOAT3からXMFLOAT2に直す
			KazMath::Vec2<float> tmpSize = { data.transform.scale.x, data.transform.scale.y };

			texSize.x = static_cast<int>(RenderTargetStatus::Instance()->GetBufferData(data.handle)->GetDesc().Width);
			texSize.y = static_cast<int>(RenderTargetStatus::Instance()->GetBufferData(data.handle)->GetDesc().Height);

			KazMath::Vec2<float> leftUp, rightDown;
			leftUp = { 0.0f,0.0f };
			rightDown = { 1.0f,1.0f };

			//サイズ変更
			array<KazMath::Vec2<float>, 4>tmp = KazRenderHelper::ChangePlaneScale(leftUp, rightDown, tmpSize, anchorPoint, texSize);
			for (int i = 0; i < tmp.size(); i++)
			{
				vertices[i].pos = { tmp[i].x,tmp[i].y,0.0f };
			}
		}
	}

	//UV切り取り
	{
		KazMath::Vec2<int> divSize = renderData.shaderResourceMgrInstance->GetDivData(data.handle).divSize;
		KazMath::Vec2<float> tmpSize = { data.transform.scale.x, data.transform.scale.y };

		bool isItSafeToUseAnimationHandleFlag = KazHelper::IsitInAnArray(data.animationHandle, renderData.shaderResourceMgrInstance->GetDivData(data.handle).divLeftUp.size());
		bool isItSafeToUseDivDataFlag = (divSize.x != -1 && divSize.y != -1);

		if (isItSafeToUseDivDataFlag && isItSafeToUseAnimationHandleFlag)
		{
			KazMath::Vec2<int> divLeftUpPos = renderData.shaderResourceMgrInstance->GetDivData(data.handle).divLeftUp[data.animationHandle];


			KazRenderHelper::VerticesCut(divSize, divLeftUpPos, &vertices[0].pos, &vertices[1].pos, &vertices[2].pos, &vertices[3].pos, tmpSize, anchorPoint);
			KazRenderHelper::UVCut(divLeftUpPos, divSize, texSize, &vertices[0].uv, &vertices[1].uv, &vertices[2].uv, &vertices[3].uv);
		}
	}

	//X軸にUVを反転
	{
		KazRenderHelper::FlipXUv(&vertices[0].uv, &vertices[1].uv, &vertices[2].uv, &vertices[3].uv);
	}
	//Y軸にUVを反転
	{
		KazRenderHelper::FlipYUv(&vertices[0].uv, &vertices[1].uv, &vertices[2].uv, &vertices[3].uv);
	}



	//頂点データに何か変更があったら転送する
	{
		gpuBuffer->TransData(vertexBufferHandle, vertices.data(), VertByte);
	}
	//頂点データの書き換えとUVの書き換え-----------------------------------------------------------------------------------------------------


	//バッファの転送-----------------------------------------------------------------------------------------------------
	//行列
	//if (lMatrixDirtyFlag || lMatFlag)
	//{
	ConstBufferData constMap;
	constMap.world = baseMatWorldData.matWorld;
	constMap.view = renderData.cameraMgrInstance->GetViewMatrix(data.cameraIndex);
	constMap.viewproj = renderData.cameraMgrInstance->GetPerspectiveMatProjection();
	constMap.color = { 0.0f,0.0f,0.0f,data.alpha / 255.0f };
	constMap.mat = constMap.world * constMap.view * constMap.viewproj;

	//gpuBuffer->TransData();
	TransData(&constMap, constBufferHandle, typeid(ConstBufferData).name());
	//}
	//バッファの転送-----------------------------------------------------------------------------------------------------


	//バッファをコマンドリストに積む-----------------------------------------------------------------------------------------------------
	SetConstBufferOnCmdList(pipeline);
	renderData.shaderResourceMgrInstance->SetSRV(data.handle, GraphicsRootSignature::Instance()->GetRootParam(renderData.pipelineMgr->GetRootSignatureName(pipeline)), GRAPHICS_PRAMTYPE_TEX);
	//バッファをコマンドリストに積む-----------------------------------------------------------------------------------------------------

	//描画命令-----------------------------------------------------------------------------------------------------
	renderData.cmdListInstance->cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	renderData.cmdListInstance->cmdList->IASetVertexBuffers(0, 1, &vertexBufferView);
	renderData.cmdListInstance->cmdList->IASetIndexBuffer(&indexBufferView);
	renderData.cmdListInstance->cmdList->DrawIndexedInstanced(static_cast<UINT>(indices.size()), 1, 0, 0, 0);
	//描画命令-----------------------------------------------------------------------------------------------------



	//DirtyFlagの更新-----------------------------------------------------------------------------------------------------
	//DirtyFlagの更新-----------------------------------------------------------------------------------------------------
}

XMMATRIX Sprite3DRender::GetMotherMatrix()
{
	return motherMat;
}
