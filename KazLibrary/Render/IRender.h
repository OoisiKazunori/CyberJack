#pragma once
#include"../DirectXCommon/Base.h"
#include"../Helper/KazRenderHelper.h"
#include"../Math/KazMath.h"
#include"../Pipeline//GraphicsPipeLineMgr.h"
#include"../Pipeline/GraphicsRootSignature.h"
#include"../Buffer/CreateGpuBuffer.h"

class IRender
{
public:
	RESOURCE_HANDLE CreateConstBuffer(const unsigned int &TRANSMISSION_DATA, const string &ID, const GraphicsRangeType &RANGE, const GraphicsRootParamType &ROOTPARAM);
	void TransData(void *DATA, RESOURCE_HANDLE HANDLE, const string &ID);

private:
	vector<unique_ptr<string>> constBufferDataName;
	vector<unique_ptr<unsigned int>> constBufferDataSize;
	vector<unique_ptr<RESOURCE_HANDLE>> constBufferHandles;
	vector<unique_ptr<GraphicsRootParamType>> constBufferRootParam;
	vector<unique_ptr<GraphicsRangeType>> constBufferRangeType;


protected:
	void SetConstBufferOnCmdList(PipeLineNames pipeline, bool REMOVE_DATA_FLAG = false);

	KazMath::BaseMatWorldData baseMatWorldData;
	KazRenderHelper::RenderInstancesData renderData;
	unique_ptr<CreateGpuBuffer> gpuBuffer;
	KazRenderHelper::DrawInstanceCommandData drawCommandData;



	void DrawCommand(const KazRenderHelper::DrawInstanceCommandData &DATA)
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
};