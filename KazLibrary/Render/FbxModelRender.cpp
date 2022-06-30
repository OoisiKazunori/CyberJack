#include"FbxModelRender.h"
#include"../Helper/KazBufferHelper.h"
#include"../Buffer/DescriptorHeapMgr.h"

FbxModelRender::FbxModelRender()
{
	gpuBuffer = std::make_unique<CreateGpuBuffer>();
	constBufferHandle[0] = CreateConstBuffer(sizeof(ConstBufferData), typeid(ConstBufferData).name(), GRAPHICS_RANGE_TYPE_CBV, GRAPHICS_PRAMTYPE_DRAW);
	constBufferHandle[1] = CreateConstBuffer(sizeof(ConstBufferDataSkin), typeid(ConstBufferDataSkin).name(), GRAPHICS_RANGE_TYPE_CBV, GRAPHICS_PRAMTYPE_SKINING);

	frameTime.SetTime(0, 0, 0, 1, 0, FbxTime::EMode::eFrames60);
}

void FbxModelRender::Draw()
{
	resourceData = FbxModelResourceMgr::Instance()->GetResourceData(data.handle);


	if (data.handle != -1)
	{
		if (data.isPlay)
		{
			currentTime += frameTime;
			if (currentTime > resourceData->endTime[data.animationNumber])
			{
				currentTime = resourceData->startTime[data.animationNumber];
			}
		}
		else
		{
			currentTime = resourceData->startTime[data.animationNumber];
		}





		//パイプライン設定-----------------------------------------------------------------------------------------------------
		pipeline = static_cast<PipeLineNames>(data.pipelineName);
		renderData.pipelineMgr->SetPipeLineAndRootSignature(pipeline);
		//パイプライン設定-----------------------------------------------------------------------------------------------------


		//DirtyFlag検知-----------------------------------------------------------------------------------------------------	
		//bool viewMatFlag = cameraViewDirtyFlag->FloatDirty() || cameraProjectionDirtyFlag->FloatDirty();

		//bool lMatrixDirtyFlag = positionDirtyFlag->FloatDirty() || scaleDirtyFlag->FloatDirty() || rotationDirtyFlag->FloatDirty();
		//bool lFbxHandleDirtyFlag = this->fbxHandleDirtyFlag->Dirty();
		//DirtyFlag検知-----------------------------------------------------------------------------------------------------


		//行列計算-----------------------------------------------------------------------------------------------------
		//if (lMatrixDirtyFlag)
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
			motherMat = baseMatWorldData.matWorld;
		}
		//行列計算-----------------------------------------------------------------------------------------------------


		//バッファの転送-----------------------------------------------------------------------------------------------------
		//行列
		//if (lMatrixDirtyFlag || viewMatFlag)
		{
			ConstBufferData constMap;
			constMap.world = baseMatWorldData.matWorld;
			constMap.view = renderData.cameraMgrInstance->GetViewMatrix(data.cameraIndex);
			constMap.viewproj = renderData.cameraMgrInstance->GetPerspectiveMatProjection();
			constMap.color = { 0.0f,0.0f,0.0f,0.0f };
			constMap.mat = constMap.world * constMap.view * constMap.viewproj;

			//gpuBuffer->TransData();
			TransData(&constMap, constBufferHandle[0], typeid(constMap).name());
		}


		{
			ConstBufferDataSkin *constMap = nullptr;
			gpuBuffer->GetBufferData(constBufferHandle[1])->Map(0, nullptr, (void **)&constMap);
			for (int i = 0; i < resourceData->bone.size(); i++)
			{
				XMMATRIX matCurrentPose;
				FbxAMatrix fbxCurrentPose = resourceData->bone[i].fbxCluster->GetLink()->EvaluateGlobalTransform(currentTime);
				KazMath::ConvertMatrixFromFbx(&matCurrentPose, fbxCurrentPose);


				if (resourceData->startTime.size() == 0)
				{
					constMap->bones[i] = XMMatrixIdentity();
				}
				else
				{
					constMap->bones[i] = resourceData->bone[i].invInitialPose * matCurrentPose;
				}
			}
			gpuBuffer->GetBufferData(constBufferHandle[1])->Unmap(0, nullptr);
		}



		for (int i = 0; i < resourceData->textureHandle.size(); i++)
		{
			if (resourceData->textureHandle[i] != -1)
			{
				D3D12_GPU_DESCRIPTOR_HANDLE gpuDescHandleSRV = DescriptorHeapMgr::Instance()->GetGpuDescriptorView(resourceData->textureHandle[i]);
				int param = KazRenderHelper::SetBufferOnCmdList(GraphicsRootSignature::Instance()->GetRootParam(renderData.pipelineMgr->GetRootSignatureName(pipeline)), GRAPHICS_RANGE_TYPE_SRV, GRAPHICS_PRAMTYPE_TEX);
				renderData.cmdListInstance->cmdList->SetGraphicsRootDescriptorTable(param, gpuDescHandleSRV);
			}
		}
		SetConstBufferOnCmdList(pipeline);


		renderData.cmdListInstance->cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		renderData.cmdListInstance->cmdList->IASetVertexBuffers(0, 1, &resourceData->vertexBufferView);
		renderData.cmdListInstance->cmdList->IASetIndexBuffer(&resourceData->indexBufferView);
		renderData.cmdListInstance->cmdList->DrawIndexedInstanced(resourceData->indicisNum, 1, 0, 0, 0);
	}
	else
	{

	}
}
