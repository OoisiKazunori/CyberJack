#include"../Render/IRender.h"

const short IRender::CreateConstBuffer(const unsigned int &TRANSMISSION_DATA, const string &ID, const GraphicsRangeType &RANGE, const GraphicsRootParamType &ROOTPARAM)
{
	constBufferDataName.push_back(unique_ptr<string>(new string(ID)));
	constBufferDataSize.push_back(unique_ptr<unsigned int>(new unsigned int(TRANSMISSION_DATA)));
	constBufferRootParam.push_back(unique_ptr<GraphicsRootParamType>(new GraphicsRootParamType(ROOTPARAM)));
	constBufferRangeType.push_back(unique_ptr<GraphicsRangeType>(new GraphicsRangeType(RANGE)));

	const short constBufferHandle = gpuBuffer->CreateBuffer
	(
		KazBufferHelper::SetConstBufferData(TRANSMISSION_DATA)
	);

	constBufferHandles.push_back(unique_ptr<short>(new short(constBufferHandle)));

	return constBufferHandle;
}

void IRender::TransData(void *DATA, const short &HANDLE, const string &ID)
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
			UINT num = KazRenderHelper::SetBufferOnCmdList(GraphicsRootSignature::Instance()->GetRootParam(renderData.pipelineMgr->GetRootSignatureName(pipeline)), GRAPHICS_RANGE_TYPE_CBV, *constBufferRootParam[i]);
			renderData.cmdListInstance->cmdList->SetGraphicsRootConstantBufferView(num, gpuBuffer->GetBufferData(*constBufferHandles[i]).Get()->GetGPUVirtualAddress());
		}
	}
}
