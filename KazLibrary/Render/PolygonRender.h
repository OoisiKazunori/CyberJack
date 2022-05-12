#pragma once
#include"../KazLibrary/DirectXCommon/Base.h"
#include<vector>
#include"../KazLibrary/Render/IRender.h"
#include"../KazLibrary/Render/RenderData.h"
#include"../KazLibrary/Helper/DirtyFlag.h"

/// <summary>
/// •½–Ê‚Ìƒ|ƒŠƒSƒ“‚ð•`‰æ‚·‚é
/// </summary>
class PolygonRender:public IRender
{
public:
	PolygonRender(const array<SpriteVertex, 4> &DATA);
	void Draw();

	Pera3DData data;
private:
	array<SpriteVertex, 4> vertices;
	array<USHORT, 6> indices;
	XMFLOAT2 anchorPoint;
	XMFLOAT2 texSize;

	PipeLineNames pipeline;
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView;
	D3D12_INDEX_BUFFER_VIEW indexBufferView;


	XMMATRIX motherMat;
	short vertexBufferHandle;
	short indexBufferHandle;
	short constBufferHandle;

	UINT VertByte;
	UINT IndexByte;


	unique_ptr<DirtySet> positionDirtyFlag, scaleDirtyFlag, rotationDirtyFlag;

	unique_ptr<DirtySet> cameraViewDirtyFlag;
	unique_ptr<DirtySet> cameraProjectionDirtyFlag;
	unique_ptr<DirtySet> cameraBillBoardDirtyFlag;
	unique_ptr<DirtySet> sizeDirtyFlag;
	unique_ptr<DirtySet> motherDirtyFlag;
};