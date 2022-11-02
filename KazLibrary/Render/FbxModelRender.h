#pragma once
#include"../DirectXCommon/Base.h"
#include"../Render/IRender.h"
#include"../Helper/DirtyFlag.h"
#include"../Render/RenderData.h"
#include"../Loader/FbxModelResourceMgr.h"

class FbxModelRender :public IRender
{
public:
	FbxModelRender();
	void Draw();

	void ReleaseSkining();
	FbxModelData data;
	FbxTime currentTime;
private:

	UINT vertByte;
	UINT indexByte;

	DirectX::XMMATRIX motherMat;
	short vertexBufferHandle;
	short indexBufferHandle;

	std::array<RESOURCE_HANDLE, 2> constBufferHandle;
	std::shared_ptr<FbxResourceData> resourceData;

	FbxTime frameTime;
	FbxTime startTime;
	FbxTime endTime;


	bool removeSkining;
};

