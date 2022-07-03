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
	DirectX::XMFLOAT2 anchorPoint;
	DirectX::XMFLOAT2 texSize;

	DirectX::XMMATRIX motherMat;
	RESOURCE_HANDLE vertexBufferHandle;
	RESOURCE_HANDLE indexBufferHandle;
	RESOURCE_HANDLE constBufferHandle;

	UINT vertByte;
	UINT indexByte;
};