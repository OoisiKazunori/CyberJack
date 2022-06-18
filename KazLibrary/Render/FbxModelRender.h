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

	FbxModelData data;
private:
	PipeLineNames pipeline;

	UINT VertByte;
	UINT IndexByte;

	XMMATRIX motherMat;
	short vertexBufferHandle;
	short indexBufferHandle;

	array<short, 2> constBufferHandle;
	const FbxResourceData *resourceData;



	unique_ptr<DirtySet> positionDirtyFlag, scaleDirtyFlag, rotationDirtyFlag;
	unique_ptr<DirtyFlag<short>> fbxHandleDirtyFlag;

	unique_ptr<DirtySet> cameraViewDirtyFlag;
	unique_ptr<DirtySet> cameraProjectionDirtyFlag;


	FbxTime frameTime;
	FbxTime startTime;
	FbxTime endTime;
	FbxTime currentTime;

};

