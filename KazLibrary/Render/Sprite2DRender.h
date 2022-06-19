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
	KazMath::Vec2<float> anchorPoint;
	KazMath::Vec2<int> texSize;

	PipeLineNames pipeline;
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView;
	D3D12_INDEX_BUFFER_VIEW indexBufferView;


	XMMATRIX motherMat;
	RESOURCE_HANDLE vertexBufferHandle;
	RESOURCE_HANDLE indexBufferHandle;
	RESOURCE_HANDLE constBufferHandle;

	UINT VertByte;
	UINT IndexByte;


	unique_ptr<DirtySet> positionDirtyFlag, scaleDirtyFlag, rotationDirtyFlag;
	unique_ptr<DirtyFlag<bool>> flipXDirtyFlag;
	unique_ptr<DirtyFlag<bool>> flipYDirtyFlag;
	unique_ptr<DirtyFlag<RESOURCE_HANDLE>> textureHandleDirtyFlag;
	unique_ptr<DirtyFlag<RESOURCE_HANDLE>> animationHandleDirtyFlag;
	unique_ptr<DirtyFlag<float>> alphaDrtyFlag;

	unique_ptr<DirtySet> cameraViewDirtyFlag;
	unique_ptr<DirtySet> cameraProjectionDirtyFlag;
	unique_ptr<DirtySet> cameraBillBoardDirtyFlag;

	unique_ptr<DirtySet> sizeDirtyFlag;

};

