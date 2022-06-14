#pragma once
#include"../DirectXCommon/Base.h"
#include"../Render/IRender.h"
#include"../Math/KazMath.h"
#include"../Helper/DirtyFlag.h"
#include"../Render/RenderData.h"

class Circle2DRender: public IRender
{
public:
	Circle2DRender();
	void Draw();

	Circle2DDrawData data;
private:
	PipeLineNames pipeline;
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView;
	D3D12_INDEX_BUFFER_VIEW indexBufferView;


	XMMATRIX motherMat;
	short vertexBufferHandle;
	short indexBufferHandle;
	short constBufferHandle;

	UINT VertByte;
	UINT IndexByte;

	UINT indicesNum;
private:
};

