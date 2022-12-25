#pragma once
#include"../KazLibrary/Render/DrawExcuteIndirect.h"
#include"../KazLibrary/Helper/KazBufferHelper.h"
#include"../KazLibrary/Helper/KazRenderHelper.h"
#include"../KazLibrary/Helper/ResouceBufferHelper.h"
#include"../KazLibrary/Helper/ISinglton.h"

//���o�Ŏg���Ă���S�Ẵp�[�e�B�N���̏����X�^�b�N���Ĉ�C�ɕ`�悷��N���X�ł�
class GPUParticleRender:public ISingleton<GPUParticleRender>
{
public:
	GPUParticleRender();
	void Draw();

	const ComputeBufferHelper::BufferData &GetStackWorldMatBuffer();
	const ComputeBufferHelper::BufferData &GetStackColorBuffer();
private:

	struct OutputData
	{
		DirectX::XMMATRIX mat;
		DirectX::XMFLOAT4 color;
	};


	static const int PARTICLE_MAX_NUM = 3000000;
	ResouceBufferHelper computeCovertWorldMatToDrawMat;
	RESOURCE_HANDLE worldMatHandle, colorHandle, outputHandle, viewProjMatHandle;

	DirectX::XMMATRIX viewProjMat;

	std::unique_ptr<DrawExcuteIndirect> excuteIndirect;
	RESOURCE_HANDLE vertexBufferHandle;
	RESOURCE_HANDLE indexBufferHandle;
	std::unique_ptr<KazRenderHelper::ID3D12ResourceWrapper> vertexBuffer, indexBuffer;
};

