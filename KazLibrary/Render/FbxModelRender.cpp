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
	if (data.handle.flag.Dirty())
	{
		resourceData = FbxModelResourceMgr::Instance()->GetResourceData(data.handle.handle);
		drawIndexInstanceCommandData = KazRenderHelper::SetDrawIndexInstanceCommandData(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST, resourceData->vertexBufferView, resourceData->indexBufferView, resourceData->indicisNum, 1);
	}


	if (data.handle.handle != -1)
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
		renderData.pipelineMgr->SetPipeLineAndRootSignature(data.pipelineName);
		//パイプライン設定-----------------------------------------------------------------------------------------------------


		//行列計算-----------------------------------------------------------------------------------------------------
		if (data.transform.Dirty() || data.motherMat.dirty.Dirty())
		{
			baseMatWorldData.matWorld = DirectX::XMMatrixIdentity();
			baseMatWorldData.matScale = KazMath::CaluScaleMatrix(data.transform.scale);
			baseMatWorldData.matTrans = KazMath::CaluTransMatrix(data.transform.pos);
			baseMatWorldData.matRota = KazMath::CaluRotaMatrix(data.transform.rotation);

			//ワールド行列の計算
			baseMatWorldData.matWorld = DirectX::XMMatrixIdentity();
			baseMatWorldData.matWorld *= baseMatWorldData.matScale;
			baseMatWorldData.matWorld *= baseMatWorldData.matRota;
			baseMatWorldData.matWorld *= baseMatWorldData.matTrans;
			baseMatWorldData.matWorld *= data.motherMat.mat;

			//親行列を掛ける
			motherMat = baseMatWorldData.matWorld;
		}
		//行列計算-----------------------------------------------------------------------------------------------------


		//バッファの転送-----------------------------------------------------------------------------------------------------
		//行列
		if (renderData.cameraMgrInstance->ViewAndProjDirty(data.cameraIndex.id) || data.transform.Dirty() || data.motherMat.dirty.Dirty() || data.cameraIndex.dirty.Dirty())
		{
			ConstBufferData constMap;
			constMap.world = baseMatWorldData.matWorld;
			constMap.view = renderData.cameraMgrInstance->GetViewMatrix(data.cameraIndex.id);
			constMap.viewproj = renderData.cameraMgrInstance->GetPerspectiveMatProjection();
			constMap.color = data.colorData.ConvertColorRateToXMFLOAT4();
			constMap.mat = constMap.world * constMap.view * constMap.viewproj;
			TransData(&constMap, constBufferHandle[0], typeid(constMap).name());
		}


		{
			ConstBufferDataSkin *constMap = nullptr;
			gpuBuffer->GetBufferData(constBufferHandle[1])->Map(0, nullptr, (void **)&constMap);
			for (int i = 0; i < resourceData->bone.size(); i++)
			{
				DirectX::XMMATRIX matCurrentPose;
				FbxAMatrix fbxCurrentPose = resourceData->bone[i].fbxCluster->GetLink()->EvaluateGlobalTransform(currentTime);
				KazMath::ConvertMatrixFromFbx(&matCurrentPose, fbxCurrentPose);


				if (resourceData->startTime.size() == 0)
				{
					constMap->bones[i] = DirectX::XMMatrixIdentity();
				}
				else
				{
					constMap->bones[i] = resourceData->bone[i].invInitialPose * matCurrentPose;
				}
			}
			if (resourceData->bone.size() == 0)
			{
				for (int i = 0; i < 32; i++)
				{
					constMap->bones[i] = DirectX::XMMatrixIdentity();
				}
			}
			gpuBuffer->GetBufferData(constBufferHandle[1])->Unmap(0, nullptr);
		}


		SetConstBufferOnCmdList(data.pipelineName);

		for (int i = 0; i < resourceData->textureHandle.size(); i++)
		{
			if (resourceData->textureHandle[i] != -1)
			{
				D3D12_GPU_DESCRIPTOR_HANDLE gpuDescHandleSRV = DescriptorHeapMgr::Instance()->GetGpuDescriptorView(resourceData->textureHandle[i]);
				int param = KazRenderHelper::SetBufferOnCmdList(GraphicsRootSignature::Instance()->GetRootParam(renderData.pipelineMgr->GetRootSignatureName(data.pipelineName)), GRAPHICS_RANGE_TYPE_SRV, GRAPHICS_PRAMTYPE_TEX);
				renderData.cmdListInstance->cmdList->SetGraphicsRootDescriptorTable(param, gpuDescHandleSRV);
			}
		}
	

		DrawIndexInstanceCommand(drawIndexInstanceCommandData);
	}

	data.Record();
}
