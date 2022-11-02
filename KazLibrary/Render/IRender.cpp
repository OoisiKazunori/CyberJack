#include"../Render/IRender.h"

RESOURCE_HANDLE IRender::CreateConstBuffer(const unsigned int &TRANSMISSION_DATA, const string &ID, const GraphicsRangeType &RANGE, const GraphicsRootParamType &ROOTPARAM)
{
	constBufferDataName.push_back(unique_ptr<string>(new string(ID)));
	constBufferDataSize.push_back(unique_ptr<unsigned int>(new unsigned int(TRANSMISSION_DATA)));
	constBufferRootParam.push_back(unique_ptr<GraphicsRootParamType>(new GraphicsRootParamType(ROOTPARAM)));
	constBufferRangeType.push_back(unique_ptr<GraphicsRangeType>(new GraphicsRangeType(RANGE)));

	RESOURCE_HANDLE lBufferHandle = 0;

	switch (RANGE)
	{
	case GRAPHICS_RANGE_TYPE_CBV:
		lBufferHandle = gpuBuffer->CreateBuffer
		(
			KazBufferHelper::SetConstBufferData(TRANSMISSION_DATA)
		);
		break;
	case GRAPHICS_RANGE_TYPE_UAV_VIEW:
		lBufferHandle = gpuBuffer->CreateBuffer
		(
			KazBufferHelper::SetRWStructuredBuffer(TRANSMISSION_DATA)
		);
		break;
	}


	constBufferHandles.push_back(std::make_unique<RESOURCE_HANDLE>(lBufferHandle));

	return lBufferHandle;
}

void IRender::TransData(void *DATA, RESOURCE_HANDLE HANDLE, const string &ID)
{
	bool succeedFlag = false;
	for (int i = 0; i < constBufferDataName.size(); i++)
	{
		if (*constBufferDataName[i] == ID)
		{
			gpuBuffer->TransData(HANDLE, DATA, *constBufferDataSize[i]);
			succeedFlag = true;
		}
	}
	if (!succeedFlag)
	{
		assert(0);
	}
}

void IRender::SetConstBufferOnCmdList(PipeLineNames pipeline, bool REMOVE_DATA_FLAG)
{
	for (int i = 0; i < constBufferRootParam.size(); i++)
	{
		if (i != 1 || !REMOVE_DATA_FLAG)
		{
			UINT num = KazRenderHelper::SetBufferOnCmdList(GraphicsRootSignature::Instance()->GetRootParam(renderData.pipelineMgr->GetRootSignatureName(pipeline)), *constBufferRangeType[i], *constBufferRootParam[i]);

			switch (*constBufferRangeType[i])
			{
			case GRAPHICS_RANGE_TYPE_CBV:
				renderData.cmdListInstance->cmdList->SetGraphicsRootConstantBufferView(num, gpuBuffer->GetBufferData(*constBufferHandles[i]).Get()->GetGPUVirtualAddress());
				break;
			case GRAPHICS_RANGE_TYPE_UAV_VIEW:
				renderData.cmdListInstance->cmdList->SetGraphicsRootUnorderedAccessView(num, gpuBuffer->GetBufferData(*constBufferHandles[i]).Get()->GetGPUVirtualAddress());
				break;
			default:
				break;
			}
		}
	}
}

void IRender::DrawIndexInstanceCommand(const KazRenderHelper::DrawIndexInstanceCommandData &DATA)
{
	//•`‰æ–½—ß-----------------------------------------------------------------------------------------------------
	renderData.cmdListInstance->cmdList->IASetPrimitiveTopology(DATA.topology);
	renderData.cmdListInstance->cmdList->IASetVertexBuffers(DATA.vertexBufferDrawData.slot, DATA.vertexBufferDrawData.numViews, &DATA.vertexBufferDrawData.vertexBufferView);
	renderData.cmdListInstance->cmdList->IASetIndexBuffer(&DATA.indexBufferView);
	renderData.cmdListInstance->cmdList->DrawIndexedInstanced(
		DATA.drawIndexInstancedData.indexCountPerInstance,
		DATA.drawIndexInstancedData.instanceCount,
		DATA.drawIndexInstancedData.startIndexLocation,
		DATA.drawIndexInstancedData.baseVertexLocation,
		DATA.drawIndexInstancedData.startInstanceLocation
	);
	//•`‰æ–½—ß-----------------------------------------------------------------------------------------------------
}

void IRender::DrawInstanceCommand(const KazRenderHelper::DrawInstanceCommandData &DATA)
{
	renderData.cmdListInstance->cmdList->IASetPrimitiveTopology(DATA.topology);
	renderData.cmdListInstance->cmdList->IASetVertexBuffers(DATA.vertexBufferDrawData.slot, DATA.vertexBufferDrawData.numViews, &DATA.vertexBufferDrawData.vertexBufferView);
	renderData.cmdListInstance->cmdList->DrawInstanced(
		DATA.drawInstanceData.vertexCountPerInstance,
		DATA.drawInstanceData.instanceCount,
		DATA.drawInstanceData.startVertexLocation,
		DATA.drawInstanceData.startInstanceLocation
	);
}
