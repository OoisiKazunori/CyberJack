#include "BoxPolygonRender.h"

BoxPolygonRender::BoxPolygonRender(bool INSTANCE_FLAG, int INSTANCE_NUM)
{
	gpuBuffer.reset(new CreateGpuBuffer);
	positionDirtyFlag.reset(new DirtySet(data.transform.pos));
	scaleDirtyFlag.reset(new DirtySet(data.transform.scale));
	rotationDirtyFlag.reset(new DirtySet(data.transform.rotation));
	cameraViewDirtyFlag.reset(new DirtySet(renderData.cameraMgrInstance->view));
	cameraProjectionDirtyFlag.reset(new DirtySet(renderData.cameraMgrInstance->perspectiveMat));
	cameraBillBoardDirtyFlag.reset(new DirtySet(renderData.cameraMgrInstance->billBoard));

	colorDirtyFlag.reset(new DirtySet(data.color));

	float size = 1.0f;

	Vertex vertices[] =
	{
		//x,y,z	法線	u,v
	//前
	{{-size, -size, -size},{}, {0.0f,1.0f}},		//左下
	{{-size,  size, -size},{}, {0.0f,0.0f}},		//左上
	{{ size, -size, -size}, {}, {1.0f,1.0f}},		//右下
	{{ size,  size, -size}, {}, {1.0f,0.0f}},		//右上

	//後
	{{-size,-size,  size}, {}, {0.0f,1.0f}},		//左下
	{{-size, size,  size}, {},{0.0f,0.0f}},		//左上
	{{ size,-size,  size},{},{1.0f,1.0f}},		//右下
	{{ size, size,  size},{}, {1.0f,0.0f}},		//右上

	//左
	{{-size,-size, -size},{}, {0.0f,1.0f}},		//左下
	{{-size,-size,  size}, {}, {0.0f,0.0f}},		//左上
	{{-size, size, -size}, {}, {1.0f,1.0f}},		//右下
	{{-size, size,  size}, {}, {1.0f,0.0f}},		//右上

	//右
	{{size,-size, -size},{}, {0.0f,1.0f}},		//左下
	{{size,-size,  size}, {}, {0.0f,0.0f}},		//左上
	{{size, size, -size}, {}, {1.0f,1.0f}},		//右下
	{{size, size,  size}, {}, {1.0f,0.0f}},		//右上

	//下
	{{ size, -size, size}, {}, {0.0f,1.0f}},		//左下
	{{ size, -size,-size}, {}, {0.0f,0.0f}},		//左上
	{{-size, -size, size}, {}, {1.0f,1.0f}},		//右下
	{{-size, -size,-size}, {}, {1.0f,0.0f}},	//右上

	//上
	{{ size, size, size}, {}, {0.0f,1.0f}},			//左下
	{{ size, size,-size}, {}, {0.0f,0.0f}},			//左上
	{{-size, size, size}, {}, {1.0f,1.0f}},			//右下
	{{-size, size,-size}, {}, {1.0f,0.0f}}			//右上
	};

	unsigned short indices[] =
	{
		//前
		0,1,2,		//三角形1つ目
		2,1,3,		//三角形2つ目
		//後
		4,6,5,		//三角形3つ目
		6,7,5,		//三角形4つ目
		//左
		8,9,10,	//三角形1つ目
		10,9,11,	//三角形2つ目
		//右
		12,14,13,	//三角形1つ目
		13,14,15,//三角形2つ目
		//下
		16,18,17,	//三角形1つ目
		17,18,19,	//三角形2つ目
		//上
		20,21,22,	//三角形3つ目
		22,21,23	//三角形4つ目
	};

	UINT VertLEN = sizeof(vertices) / sizeof(vertices[0]);
	UINT IndexLEN = sizeof(indices) / sizeof(indices[0]);
	UINT VertByte = VertLEN * sizeof(Vertex);
	UINT IndexByte = IndexLEN * sizeof(USHORT);


	//indiNum = indices.size();
	indicesNum = IndexLEN;


	//法線ベクトル
	for (int i = 0; i < IndexLEN / 3; i++)
	{
		USHORT index[3];

		//三角形のインデックスを取り出して、一時的な変数に入れる
		index[0] = indices[i * 3 + 0];
		index[1] = indices[i * 3 + 1];
		index[2] = indices[i * 3 + 2];

		//三角形を構築する頂点座標をベクトルに代入
		XMVECTOR p0 = XMLoadFloat3(&vertices[index[0]].pos);
		XMVECTOR p1 = XMLoadFloat3(&vertices[index[1]].pos);
		XMVECTOR p2 = XMLoadFloat3(&vertices[index[2]].pos);

		//p0-p1,p0-p2ベクトルを計算(ベクトルの減算)
		XMVECTOR v1 = XMVectorSubtract(p1, p0);
		XMVECTOR v2 = XMVectorSubtract(p2, p0);

		//外積は両方から垂直なベクトル
		XMVECTOR normal = XMVector3Cross(v1, v2);

		//正規化(長さを1にする)
		normal = XMVector3Normalize(normal);

		//求めた法線を頂点データを代入
		XMStoreFloat3(&vertices[index[0]].normal, normal);
		XMStoreFloat3(&vertices[index[1]].normal, normal);
		XMStoreFloat3(&vertices[index[2]].normal, normal);
	}


	short vertBufferHandle = gpuBuffer->CreateBuffer(KazBufferHelper::SetVertexBufferData(VertByte));
	short indexBufferHandle = gpuBuffer->CreateBuffer(KazBufferHelper::SetIndexBufferData(IndexByte));


	instanceFlag = INSTANCE_FLAG;
	if (!instanceFlag)
	{
		//定数バッファ
		constBufferHandle = CreateConstBuffer(sizeof(ConstBufferData), typeid(ConstBufferData).name(), GRAPHICS_RANGE_TYPE_CBV, GRAPHICS_PRAMTYPE_DRAW);
	}
	instanceNum = INSTANCE_NUM;


	array<Vertex, 24> vertVec;
	array<unsigned short, 36> indiVec;
	for (int i = 0; i < 24; i++)
	{
		vertVec[i].pos = vertices[i].pos;
		vertVec[i].uv = vertices[i].uv;
		vertVec[i].normal = vertices[i].normal;
	}
	for (int i = 0; i < 36; i++)
	{
		indiVec[i] = indices[i];
	}

	gpuBuffer->TransData(vertBufferHandle, vertVec.data(), VertByte);
	gpuBuffer->TransData(indexBufferHandle, indiVec.data(), IndexByte);

	vertBufferView = KazBufferHelper::SetVertexBufferView(gpuBuffer->GetGpuAddress(vertBufferHandle), VertByte, sizeof(vertices[0]));
	indexBufferView = KazBufferHelper::SetIndexBufferView(gpuBuffer->GetGpuAddress(indexBufferHandle), IndexByte);

}

BoxPolygonRender::~BoxPolygonRender()
{
}

void BoxPolygonRender::Draw()
{
	//パイプライン設定-----------------------------------------------------------------------------------------------------
	pipeline = static_cast<PipeLineNames>(data.pipelineName);
	renderData.pipelineMgr->SetPipeLineAndRootSignature(pipeline);
	//パイプライン設定-----------------------------------------------------------------------------------------------------




	//DirtyFlag検知-----------------------------------------------------------------------------------------------------	
	bool matFlag = cameraViewDirtyFlag->FloatDirty() || cameraProjectionDirtyFlag->FloatDirty() || cameraBillBoardDirtyFlag->FloatDirty();
	bool matrixDirtyFlag = positionDirtyFlag->FloatDirty() || scaleDirtyFlag->FloatDirty() || rotationDirtyFlag->FloatDirty();
	bool colorFlag = colorDirtyFlag->FloatDirty();
	//DirtyFlag検知-----------------------------------------------------------------------------------------------------


	if (!instanceFlag)
	{
		//行列計算-----------------------------------------------------------------------------------------------------
		if (matrixDirtyFlag || matFlag || true)
		{
			baseMatWorldData.matWorld = XMMatrixIdentity();
			baseMatWorldData.matScale = KazMath::CaluScaleMatrix(data.transform.scale);
			baseMatWorldData.matTrans = KazMath::CaluTransMatrix(data.transform.pos);
			baseMatWorldData.matRota = KazMath::CaluRotaMatrix(data.transform.rotation);

			//ワールド行列の計算
			baseMatWorldData.matWorld = XMMatrixIdentity();
			baseMatWorldData.matWorld *= baseMatWorldData.matScale;
			baseMatWorldData.matWorld *= baseMatWorldData.matRota;
			baseMatWorldData.matWorld *= baseMatWorldData.matTrans;
			baseMatWorldData.matWorld *= data.motherMat;

			//親行列を掛ける
			motherMatrix = baseMatWorldData.matWorld;
		}
		//行列計算-----------------------------------------------------------------------------------------------------



		//バッファの転送-----------------------------------------------------------------------------------------------------
		//行列
		if (matrixDirtyFlag || matFlag || colorFlag | true)
		{
			ConstBufferData constMap;
			constMap.world = baseMatWorldData.matWorld;
			constMap.view = renderData.cameraMgrInstance->GetViewMatrix(data.cameraIndex);
			constMap.viewproj = renderData.cameraMgrInstance->GetPerspectiveMatProjection();
			constMap.color = KazRenderHelper::SendColorDataToGPU(data.color);
			constMap.mat = constMap.world * constMap.view * constMap.viewproj;
			TransData(&constMap, constBufferHandle, typeid(ConstBufferData).name());
		}
		//バッファの転送-----------------------------------------------------------------------------------------------------
	}

	//バッファをコマンドリストに積む-----------------------------------------------------------------------------------------------------
	SetConstBufferOnCmdList(pipeline);
	//バッファをコマンドリストに積む-----------------------------------------------------------------------------------------------------

	//描画命令-----------------------------------------------------------------------------------------------------
	renderData.cmdListInstance->cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	renderData.cmdListInstance->cmdList->IASetVertexBuffers(0, 1, &vertBufferView);
	renderData.cmdListInstance->cmdList->IASetIndexBuffer(&indexBufferView);
	renderData.cmdListInstance->cmdList->DrawIndexedInstanced(indicesNum, instanceNum, 0, 0, 0);
	//描画命令-----------------------------------------------------------------------------------------------------



	//DirtyFlagの更新-----------------------------------------------------------------------------------------------------
	positionDirtyFlag->Record();
	scaleDirtyFlag->Record();
	rotationDirtyFlag->Record();

	cameraBillBoardDirtyFlag->Record();
	cameraProjectionDirtyFlag->Record();
	cameraViewDirtyFlag->Record();
	colorDirtyFlag->Record();
	//DirtyFlagの更新-----------------------------------------------------------------------------------------------------
}

XMMATRIX BoxPolygonRender::GetMotherMatrix()
{
	return motherMatrix;
}
