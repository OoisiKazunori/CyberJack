#include "ObjModelRender.h"

ObjModelRender::ObjModelRender(bool INSTANCE_FLAG, int INSTANCE_NUM, bool MATERIAL_NONE_FALG)
{

	instanceFlag = INSTANCE_FLAG;
	instanceNum = INSTANCE_NUM;

	positionDirtyFlag.reset(new DirtySet(data.transform.pos));
	scaleDirtyFlag.reset(new DirtySet(data.transform.scale));
	rotationDirtyFlag.reset(new DirtySet(data.transform.rotation));

	upVectorDirtyFlag.reset(new DirtySet(data.upVector));

	objHandleDirtyFlag.reset(new DirtyFlag<short>(&data.handle));


	cameraViewDirtyFlag.reset(new DirtySet(renderData.cameraMgrInstance->view));
	cameraProjectionDirtyFlag.reset(new DirtySet(renderData.cameraMgrInstance->perspectiveMat));


	gpuBuffer.reset(new CreateGpuBuffer);

	if (!instanceFlag)
	{
		constBufferHandle[0] = CreateConstBuffer(sizeof(ConstBufferData), typeid(ConstBufferData).name(), GRAPHICS_RANGE_TYPE_CBV, GRAPHICS_PRAMTYPE_DRAW);
	}

	if (!MATERIAL_NONE_FALG)
	{
		constBufferHandle[1] = CreateConstBuffer(sizeof(ConstBufferDataB1), typeid(ConstBufferDataB1).name(), GRAPHICS_RANGE_TYPE_CBV, GRAPHICS_PRAMTYPE_DATA);
	}
}

ObjModelRender::~ObjModelRender()
{
}

void ObjModelRender::Draw()
{
	modelData = ObjResourceMgr::Instance()->GetResourceData(data.handle);

	//パイプライン設定-----------------------------------------------------------------------------------------------------
	pipeline = static_cast<PipeLineNames>(data.pipelineName);
	renderData.pipelineMgr->SetPipeLineAndRootSignature(pipeline);
	//パイプライン設定-----------------------------------------------------------------------------------------------------


	//DirtyFlag検知-----------------------------------------------------------------------------------------------------	
	bool viewMatFlag = cameraViewDirtyFlag->FloatDirty() || cameraProjectionDirtyFlag->FloatDirty() || upVectorDirtyFlag->FloatDirty();

	bool matrixDirtyFlag = positionDirtyFlag->FloatDirty() || scaleDirtyFlag->FloatDirty() || rotationDirtyFlag->FloatDirty();
	bool objHandleDirtyFlag = this->objHandleDirtyFlag->Dirty();
	//DirtyFlag検知-----------------------------------------------------------------------------------------------------


	//行列計算-----------------------------------------------------------------------------------------------------
	//インスタンシング描画を行うならこの処理は転送しない
	if (!instanceFlag)
	{
		//if (matrixDirtyFlag)
		{
			baseMatWorldData.matWorld = XMMatrixIdentity();
			baseMatWorldData.matScale = KazMath::CaluScaleMatrix(data.transform.scale);
			baseMatWorldData.matTrans = KazMath::CaluTransMatrix(data.transform.pos);
			baseMatWorldData.matRota = KazMath::CaluRotaMatrix(data.transform.rotation);
			//ワールド行列の計算
			baseMatWorldData.matWorld = XMMatrixIdentity();
			baseMatWorldData.matWorld *= baseMatWorldData.matScale;
			baseMatWorldData.matWorld *= baseMatWorldData.matRota;
			if (data.upVector.m128_f32[0] != 0.0f ||
				data.upVector.m128_f32[1] != 1.0f ||
				data.upVector.m128_f32[2] != 0.0f)
			{
				baseMatWorldData.matWorld *= KazMath::CaluSlopeMatrix(data.upVector, { 0.0f,0.0f,1.0f });
			}
			if (data.frontVector.m128_f32[0] != 0.0f ||
				data.frontVector.m128_f32[1] != 0.0f ||
				data.frontVector.m128_f32[2] != 1.0f)
			{
				baseMatWorldData.matWorld *= KazMath::CaluFrontMatrix({ 0.0f,1.0f,0.0f }, data.frontVector);
			}
			baseMatWorldData.matWorld *= baseMatWorldData.matTrans;
			//baseMatWorldData.matWorld *= data.motherMat;

			//親行列を掛ける
			data.motherMat = baseMatWorldData.matWorld;
		}
		//行列計算-----------------------------------------------------------------------------------------------------


		//バッファの転送-----------------------------------------------------------------------------------------------------
		//行列
		//if (matrixDirtyFlag || viewMatFlag)
		{
			ConstBufferData constMap;
			constMap.world = baseMatWorldData.matWorld;
			constMap.view = renderData.cameraMgrInstance->GetViewMatrix(data.cameraIndex);
			constMap.viewproj = renderData.cameraMgrInstance->GetPerspectiveMatProjection();
			constMap.color = KazRenderHelper::SendColorDataToGPU(data.color);
			constMap.mat = constMap.world * constMap.view * constMap.viewproj;

			//gpuBuffer->TransData();
			TransData(&constMap, constBufferHandle[0], typeid(constMap).name());
		}
	}


	if (data.pipelineName != PIPELINE_NAME_COLOR_WIREFLAME && data.pipelineName != PIPELINE_NAME_COLOR_WIREFLAME_MULTITEX)
	{

		//マテリアルデータ
		if (true)
		{
			ConstBufferDataB1 constMap;
			constMap.alpha = data.color.w / 255.0f;
			constMap.ambient = modelData.material.ambient;
			constMap.diffuse = modelData.material.diffuse;
			constMap.specular = modelData.material.specular;
			TransData(&constMap, constBufferHandle[1], typeid(constMap).name());
		}
		//バッファの転送-----------------------------------------------------------------------------------------------------


		//バッファをコマンドリストに積む-----------------------------------------------------------------------------------------------------
		renderData.shaderResourceMgrInstance->SetSRV(modelData.mtlHanlde, GraphicsRootSignature::Instance()->GetRootParam(renderData.pipelineMgr->GetRootSignatureName(pipeline)), GRAPHICS_PRAMTYPE_TEX);
		//バッファをコマンドリストに積む-----------------------------------------------------------------------------------------------------
	}

	SetConstBufferOnCmdList(pipeline, data.removeMaterialFlag);


	//追加のテクスチャを送る---------------------------------------------------------------
	for (int i = 0; i < data.addHandle.handle.size(); ++i)
	{
		renderData.shaderResourceMgrInstance->SetSRV(data.addHandle.handle[i], GraphicsRootSignature::Instance()->GetRootParam(renderData.pipelineMgr->GetRootSignatureName(pipeline)), data.addHandle.paramType[i]);
	}
	//追加のテクスチャを送る---------------------------------------------------------------


	//描画命令-----------------------------------------------------------------------------------------------------
	renderData.cmdListInstance->cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	renderData.cmdListInstance->cmdList->IASetVertexBuffers(0, 1, &modelData.vertexBufferView);
	renderData.cmdListInstance->cmdList->IASetIndexBuffer(&modelData.indexBufferView);
	renderData.cmdListInstance->cmdList->DrawIndexedInstanced(modelData.indexNum, instanceNum, 0, 0, 0);
	//描画命令-----------------------------------------------------------------------------------------------------


	//DirtyFlagの更新-----------------------------------------------------------------------------------------------------
	positionDirtyFlag->Record();
	scaleDirtyFlag->Record();
	rotationDirtyFlag->Record();
	upVectorDirtyFlag->Record();

	this->objHandleDirtyFlag->Record();

	cameraProjectionDirtyFlag->Record();
	cameraViewDirtyFlag->Record();
	//DirtyFlagの更新-----------------------------------------------------------------------------------------------------
}