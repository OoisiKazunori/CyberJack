#pragma once
#include"../DirectXCommon/Base.h"
#include"../Render/IRender.h"
#include"../Math/KazMath.h"
#include"../Helper/DirtyFlag.h"
#include"../Render/RenderData.h"


class Sprite3DRender : public IRender
{
public:
	Sprite3DRender(const XMFLOAT2 &ANCHOR_POINT = { 0.5f,0.5f });
	~Sprite3DRender();
	void Draw();

	XMMATRIX GetMotherMatrix();
	Sprite3DData data;
private:
	array<SpriteVertex, 4> vertices;
	array<USHORT,6> indices;
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
	unique_ptr<DirtyFlag<bool>> flipXDirtyFlag;
	unique_ptr<DirtyFlag<bool>> flipYDirtyFlag;
	unique_ptr<DirtyFlag<short>> textureHandleDirtyFlag;
	unique_ptr<DirtyFlag<short>> animationHandleDirtyFlag;

	unique_ptr<DirtySet> cameraViewDirtyFlag;
	unique_ptr<DirtySet> cameraProjectionDirtyFlag;
	unique_ptr<DirtySet> cameraBillBoardDirtyFlag;
	unique_ptr<DirtySet> sizeDirtyFlag;
	unique_ptr<DirtySet> motherDirtyFlag;

};