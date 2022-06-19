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

};