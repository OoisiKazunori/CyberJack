#pragma once
#include"../DirectXCommon/Base.h"
#include"../Render/IRender.h"
#include"../Math/KazMath.h"
#include"../Helper/DirtyFlag.h"
#include"../Render/RenderData.h"

class Sprite2DRender : public IRender
{
public:
	Sprite2DRender();
	~Sprite2DRender();
	void Draw();

	XMMATRIX GetMotherMatrix();
	Sprite2DData data;
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
	unique_ptr<DirtyFlag<bool>> flipXDirtyFlag;
	unique_ptr<DirtyFlag<bool>> flipYDirtyFlag;
	unique_ptr<DirtyFlag<short>> textureHandleDirtyFlag;
	unique_ptr<DirtyFlag<short>> animationHandleDirtyFlag;

	unique_ptr<DirtySet> cameraViewDirtyFlag;
	unique_ptr<DirtySet> cameraProjectionDirtyFlag;
	unique_ptr<DirtySet> cameraBillBoardDirtyFlag;

	unique_ptr<DirtySet> sizeDirtyFlag;

};

