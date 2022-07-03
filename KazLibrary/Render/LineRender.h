#pragma once
#include"../DirectXCommon/Base.h"
#include"../Render/IRender.h"
#include"../Math/KazMath.h"
#include"../Helper/DirtyFlag.h"
#include"../Render/RenderData.h"

class LineRender :public IRender
{
public:
	LineRender();
	~LineRender();

	void Draw();

	LineDrawData data;
private:
	array<LineVertex, 2>vertices;
	UINT vertByte;

	RESOURCE_HANDLE vertexBufferHandle;
	RESOURCE_HANDLE constBufferHandle;
};

