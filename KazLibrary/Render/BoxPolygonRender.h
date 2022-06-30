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
	bool instanceFlag;
};

