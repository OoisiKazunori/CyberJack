#pragma once
#include"../KazLibrary/Math/KazMath.h"
#include"../KazLibrary/Buffer/CreateGpuBuffer.h"
#include<array>
#include<vector>
#include"../KazLibrary/Render/KazRender.h"
#include"../KazLibrary/Render/DrawExcuteIndirect.h"
#include"../Game/Helper/ComputeBufferHelper.h"

/// <summary>
/// BB“à‚É”z’u‚µ‚½‹…”»’è‚Ì•`‰æ
/// </summary>
class DrawHitBoxPos
{
public:
	DrawHitBoxPos(const ComputeBufferHelper::BufferData &POS_HANDLE, UINT MAX_NUM);
	void Update();
	void Draw();

private:
	RESOURCE_HANDLE matHandle, cameraHandle;
	struct OutputData
	{
		DirectX::XMMATRIX matrix;
		DirectX::XMFLOAT4 color;
	};

	struct CameraData
	{
		DirectX::XMMATRIX scaleRotateBillboardMat;
		DirectX::XMMATRIX viewProjection;
		UINT posMaxNum;
	};

	std::unique_ptr<DrawExcuteIndirect> excuteIndirect;
	ComputeBufferHelper computeHelper;
	UINT countNum;
	RESOURCE_HANDLE vertexBufferHandle;
	RESOURCE_HANDLE indexBufferHandle;
	std::unique_ptr<KazRenderHelper::ID3D12ResourceWrapper> vertexBuffer, indexBuffer;
};
