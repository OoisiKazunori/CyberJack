#pragma once
#include"../DirectXCommon/Base.h"

struct BufferMemorySize
{
	short startSize;
	short endSize;
};

enum BufferMemory
{
	DESCRIPTORHEAP_MEMORY_TEXTURE_SPRITE,
	DESCRIPTORHEAP_MEMORY_TEXTURE_OBJ,
	DESCRIPTORHEAP_MEMORY_TEXTURE_FBX,
	DESCRIPTORHEAP_MEMORY_TEXTURE_RENDERTARGET,
	DESCRIPTORHEAP_MEMORY_TEXTURE_COMPUTEBUFFER,
	DESCRIPTORHEAP_MEMORY_MAX = 5
};

namespace KazBufferHelper
{
	struct BufferResourceData
	{
		D3D12_HEAP_PROPERTIES heapProperties;
		D3D12_HEAP_FLAGS heapFlags;
		D3D12_RESOURCE_DESC resourceDesc;
		D3D12_RESOURCE_STATES resourceState;
		D3D12_CLEAR_VALUE *clearValue;
		string BufferName;

		BufferResourceData
		(
			const D3D12_HEAP_PROPERTIES &HEAP_PROP,
			const D3D12_HEAP_FLAGS &HEAP_FLAGS,
			const D3D12_RESOURCE_DESC &RESOURCE_DESC,
			const D3D12_RESOURCE_STATES &RESOURCE_STATE,
			D3D12_CLEAR_VALUE *CLEAR_VALUE,
			const string &BUFFER_NAME
		)
			:heapProperties(HEAP_PROP),
			heapFlags(HEAP_FLAGS),
			resourceDesc(RESOURCE_DESC),
			resourceState(RESOURCE_STATE),
			clearValue(CLEAR_VALUE),
			BufferName(BUFFER_NAME)
		{
		};
	};


	/// <summary>
	/// �萔�o�b�t�@�𐶐�����ۂɕK�v�Ȑݒ���ȈՂɓZ�߂���
	/// </summary>
	/// <param name="BUFFER_SIZE">�萔�o�b�t�@�̃T�C�Y</param>
	/// <returns>�萔�o�b�t�@�̐����ɕK�v�Ȑݒ�</returns>
	KazBufferHelper::BufferResourceData SetConstBufferData(const unsigned int &BUFFER_SIZE, const string &BUFFER_NAME = "ConstBuffer");

	/// <summary>
	/// �V�F�[�_�[���\�[�X�o�b�t�@�𐶐�����ۂɕK�v�Ȑݒ���ȈՂɓZ�߂���
	/// <param name="TEXTURE_DATA">�ǂݍ��񂾃e�N�X�`���̐ݒ�</param>
	/// <returns>�V�F�[�_�[���\�[�X�o�b�t�@�̐����ɕK�v�Ȑݒ�</returns>
	KazBufferHelper::BufferResourceData SetShaderResourceBufferData(const D3D12_RESOURCE_DESC &TEXTURE_DATA, const string &BUFFER_NAME = "ShaderResourceBuffer");


	KazBufferHelper::BufferResourceData SetVertexBufferData(const unsigned int &BUFFER_SIZE, const string &BUFFER_NAME = "VertexBuffer");


	KazBufferHelper::BufferResourceData SetIndexBufferData(const unsigned int &BUFFER_SIZE, const string &BUFFER_NAME = "IndexBuffer");


	KazBufferHelper::BufferResourceData SetRenderTargetData(const D3D12_HEAP_PROPERTIES &HEAP_PROPERTIES, const D3D12_RESOURCE_DESC &RESOURCE, D3D12_CLEAR_VALUE *CLEAR_COLOR, const string &BUFFER_NAME = "RenderTarget");

	KazBufferHelper::BufferResourceData SetStructureBuffer(const unsigned int &BUFFER_SIZE, const string &BUFFER_NAME = "StructureBuffer");

	KazBufferHelper::BufferResourceData SetRWStructuredBuffer(const unsigned int &BUFFER_SIZE, const string &BUFFER_NAME = "RWStructureBuffer");

	KazBufferHelper::BufferResourceData SetCommandBufferData(const unsigned int &BUFFER_SIZE, const string &BUFFER_NAME = "CommandBuffer");


	D3D12_VERTEX_BUFFER_VIEW SetVertexBufferView(const D3D12_GPU_VIRTUAL_ADDRESS &GPU_ADDRESS, const unsigned int &BUFFER_SIZE, const unsigned int &ONE_VERTICES_SIZE);

	D3D12_INDEX_BUFFER_VIEW SetIndexBufferView(const D3D12_GPU_VIRTUAL_ADDRESS &GPU_ADDRESS, const unsigned int &BUFFER_SIZE);
}

