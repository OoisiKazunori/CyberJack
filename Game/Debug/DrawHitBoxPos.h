#pragma once
#include"../KazLibrary/Math/KazMath.h"
#include"../KazLibrary/Buffer/CreateGpuBuffer.h"
#include<array>
#include<vector>
#include"../KazLibrary/Render/KazRender.h"
#include"../KazLibrary/Render/DrawExcuteIndirect.h"

/// <summary>
/// BB“à‚É”z’u‚µ‚½‹…”»’è‚Ì•`‰æ
/// </summary>
class DrawHitBoxPos
{
public:
	DrawHitBoxPos(D3D12_GPU_VIRTUAL_ADDRESS POS_HANDLE, UINT MAX_NUM);
	void Update();
	void Draw();

private:
	CreateGpuBuffer buffers;
	RESOURCE_HANDLE matHandle, cameraHandle;
	D3D12_GPU_VIRTUAL_ADDRESS posViewHandle;

	struct OutputData
	{
		DirectX::XMMATRIX matrix;
		DirectX::XMFLOAT4 color;
	};

	struct CameraData
	{
		DirectX::XMMATRIX scaleRotateBillboardMat;
		DirectX::XMMATRIX viewProjection;
	};

	std::unique_ptr<DrawExcuteIndirect> excuteIndirect;
};

