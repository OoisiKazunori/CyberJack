#pragma once
#include"../KazLibrary/Render/DrawExcuteIndirect.h"
#include"../KazLibrary/Helper/KazBufferHelper.h"
#include"../KazLibrary/Helper/KazRenderHelper.h"
#include"../KazLibrary/Helper/ResouceBufferHelper.h"
#include"../KazLibrary/Helper/ISinglton.h"

//演出で使われている全てのパーティクルの情報をスタックして一気に描画するクラスです
class GPUParticleRender:public ISingleton<GPUParticleRender>
{
public:
	GPUParticleRender();

	void InitCount();
	void Draw();

	const ResouceBufferHelper::BufferData &GetStackBuffer();

	struct InputData
	{
		DirectX::XMMATRIX worldMat;
		DirectX::XMFLOAT4 color;
	};
private:

	
	struct OutputData
	{
		DirectX::XMMATRIX mat;
		DirectX::XMFLOAT4 color;
	};


	static const int PARTICLE_MAX_NUM = 6000000;
	ResouceBufferHelper computeCovertWorldMatToDrawMat;
	RESOURCE_HANDLE worldMatHandle, outputHandle, viewProjMatHandle;

	DirectX::XMMATRIX viewProjMat;

	std::unique_ptr<DrawExcuteIndirect> excuteIndirect;
	RESOURCE_HANDLE vertexBufferHandle;
	RESOURCE_HANDLE indexBufferHandle;
	std::unique_ptr<KazRenderHelper::ID3D12ResourceWrapper> vertexBuffer, indexBuffer;



	KazRenderHelper::ID3D12ResourceWrapper copyBuffer;
};

