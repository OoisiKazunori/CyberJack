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
	void TransData(void *DATA, RESOURCE_HANDLE HANDLE, const std::string &ID);

	KazMath::BaseMatWorldData baseMatWorldData;
private:
	std::vector<std::unique_ptr<string>> constBufferDataName;
	std::vector<std::unique_ptr<unsigned int>> constBufferDataSize;
	std::vector<std::unique_ptr<RESOURCE_HANDLE>> constBufferHandles;
	std::vector<std::unique_ptr<GraphicsRootParamType>> constBufferRootParam;
	std::vector<std::unique_ptr<GraphicsRangeType>> constBufferRangeType;


protected:
	void SetConstBufferOnCmdList(PipeLineNames pipeline, bool REMOVE_DATA_FLAG = false);

	KazRenderHelper::RenderInstancesData renderData;
	std::unique_ptr<CreateGpuBuffer> gpuBuffer;
	KazRenderHelper::DrawIndexInstanceCommandData drawIndexInstanceCommandData;
	KazRenderHelper::DrawInstanceCommandData drawInstanceCommandData;



	void DrawIndexInstanceCommand(const KazRenderHelper::DrawIndexInstanceCommandData &DATA);

	void DrawInstanceCommand(const KazRenderHelper::DrawInstanceCommandData &DATA);
};