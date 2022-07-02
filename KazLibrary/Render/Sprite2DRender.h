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

	Sprite2DData data;
private:
	array<SpriteVertex, 4> vertices;
	array<USHORT, 6> indices;
	KazMath::Vec2<float> anchorPoint;
	KazMath::Vec2<int> texSize;

	RESOURCE_HANDLE vertexBufferHandle;
	RESOURCE_HANDLE indexBufferHandle;
	RESOURCE_HANDLE constBufferHandle;

	UINT vertByte;
	UINT indexByte;
};

