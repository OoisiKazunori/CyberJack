#pragma once
#include"../DirectXCommon/Base.h"
#include"../Render/IRender.h"
#include"../Render/RenderData.h"
#include"../Helper/DirtyFlag.h"

class BoxPolygonRender :public IRender
{
public:
	BoxPolygonRender(bool INSTANCE_FLAG = false, int INSTANCE_NUM = 1);
	~BoxPolygonRender();
	void Draw();

	XMMATRIX GetMotherMatrix();
	PolygonDrawData data;
private:
	XMMATRIX motherMatrix;
	D3D12_VERTEX_BUFFER_VIEW vertBufferView;
	D3D12_INDEX_BUFFER_VIEW indexBufferView;


	RESOURCE_HANDLE constBufferHandle;
	UINT indicesNum;

	bool instanceFlag;
	int instanceNum;

	PipeLineNames pipeline;

	unique_ptr<DirtySet> positionDirtyFlag, scaleDirtyFlag, rotationDirtyFlag;

	unique_ptr<DirtySet> cameraViewDirtyFlag;
	unique_ptr<DirtySet> cameraProjectionDirtyFlag;
	unique_ptr<DirtySet> cameraBillBoardDirtyFlag;
	unique_ptr<DirtySet> colorDirtyFlag;
};

