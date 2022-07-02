#include "Sprite2DRender.h"
#include"../Buffer/DescriptorHeapMgr.h"
#include"../Helper/KazHelper.h"
#include"../RenderTarget/RenderTargetStatus.h"

Sprite2DRender::Sprite2DRender()
{
	anchorPoint = { 0.5f,0.5f };

	gpuBuffer = std::make_unique<CreateGpuBuffer>();


	//�f�[�^�̒�`-----------------------------------------------------------------------------------------------------
	//���_�f�[�^
	KazRenderHelper::InitVerticesPos(&vertices[0].pos, &vertices[1].pos, &vertices[2].pos, &vertices[3].pos, anchorPoint.ConvertXMFLOAT2());
	KazRenderHelper::InitUvPos(&vertices[0].uv, &vertices[1].uv, &vertices[2].uv, &vertices[3].uv);
	//�C���f�b�N�X�f�[�^
	indices = KazRenderHelper::InitIndciesForPlanePolygon();
	//�f�[�^�̒�`-----------------------------------------------------------------------------------------------------


	vertByte = KazBufferHelper::GetBufferSize<UINT>(vertices.size(), sizeof(SpriteVertex));
	IndexByte = KazBufferHelper::GetBufferSize<UINT>(indices.size(), sizeof(sizeof(unsigned short)));


	//�o�b�t�@����-----------------------------------------------------------------------------------------------------
	//���_�o�b�t�@
	vertexBufferHandle = gpuBuffer->CreateBuffer
	(
		KazBufferHelper::SetVertexBufferData(vertByte)
	);
	//�C���f�b�N�X�o�b�t�@
	indexBufferHandle = gpuBuffer->CreateBuffer
	(
		KazBufferHelper::SetIndexBufferData(IndexByte)
	);
	//�萔�o�b�t�@
	constBufferHandle = CreateConstBuffer(sizeof(ConstBufferData), typeid(ConstBufferData).name(), GRAPHICS_RANGE_TYPE_CBV, GRAPHICS_PRAMTYPE_DRAW);
	//�o�b�t�@����-----------------------------------------------------------------------------------------------------


	//�o�b�t�@�]��-----------------------------------------------------------------------------------------------------
	gpuBuffer->TransData(vertexBufferHandle, vertices.data(), vertByte);
	gpuBuffer->TransData(indexBufferHandle, indices.data(), IndexByte);
	//�o�b�t�@�]��-----------------------------------------------------------------------------------------------------


	//�r���[�̐ݒ�-----------------------------------------------------------------------------------------------------
	vertexBufferView = KazBufferHelper::SetVertexBufferView(gpuBuffer->GetGpuAddress(vertexBufferHandle), vertByte, sizeof(vertices[0]));
	indexBufferView = KazBufferHelper::SetIndexBufferView(gpuBuffer->GetGpuAddress(indexBufferHandle), IndexByte);
	//�r���[�̐ݒ�-----------------------------------------------------------------------------------------------------


	drawCommandData = KazRenderHelper::SetDrawCommandData(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST, vertexBufferView, indexBufferView, static_cast<UINT>(indices.size()), 1);

}

Sprite2DRender::~Sprite2DRender()
{
}

void Sprite2DRender::Draw()
{
	//�p�C�v���C���ݒ�-----------------------------------------------------------------------------------------------------
	renderData.pipelineMgr->SetPipeLineAndRootSignature(data.pipelineName);
	//�p�C�v���C���ݒ�-----------------------------------------------------------------------------------------------------



	//�s��v�Z-----------------------------------------------------------------------------------------------------
	if(data.transform.Dirty())
	{
		baseMatWorldData.matWorld = XMMatrixIdentity();
		baseMatWorldData.matWorld *= XMMatrixRotationZ(XMConvertToRadians(data.transform.rotation));
		baseMatWorldData.matWorld *= XMMatrixTranslationFromVector(data.transform.pos.ConvertXMVECTOR());
	}
	//�s��v�Z-----------------------------------------------------------------------------------------------------



	//���_�f�[�^�̏���������UV�̏�������-----------------------------------------------------------------------------------------------------

	//�ǂݍ��񂾃e�N�X�`���̃T�C�Y
	//�ǂݍ��񂾉摜�̃T�C�Y�����킹��
	if (data.handleData.flag.Dirty() || data.transform.scaleDirtyFlag.Dirty())
	{
		//�O�����\�[�X�̃e�N�X�`���T�C�Y
		if (DescriptorHeapMgr::Instance()->GetType(data.handleData.handle) != DESCRIPTORHEAP_MEMORY_TEXTURE_RENDERTARGET)
		{
			//�T�C�Y��XMFLOAT3����XMFLOAT2�ɒ���
			KazMath::Vec2<float> tmpSize = {};
			XMFLOAT4 tmpModiSize = {};
			//�T�C�Y��XMFLOAT3����XMFLOAT2�ɒ���
			if (!data.changeSizeTypeFlag)
			{
				tmpSize =  data.transform.scale;
			}
			else
			{
				tmpModiSize = data.size;
			}
			texSize.x = static_cast<int>(renderData.shaderResourceMgrInstance->GetTextureSize(data.handleData.handle).Width);
			texSize.y = static_cast<int>(renderData.shaderResourceMgrInstance->GetTextureSize(data.handleData.handle).Height);


			KazMath::Vec2<float> leftUp, rightDown;
			leftUp = { 0.0f,0.0f };
			rightDown = { 1.0f,1.0f };

			//�T�C�Y�ύX
			array<KazMath::Vec2<float>, 4>tmp;
			if (!data.changeSizeTypeFlag)
			{
				tmp = KazRenderHelper::ChangePlaneScale(leftUp, rightDown, tmpSize, anchorPoint, texSize);
			}
			else
			{
				tmp = KazRenderHelper::ChangeModiPlaneScale(leftUp, rightDown, tmpModiSize, anchorPoint.ConvertXMFLOAT2(), texSize.ConvertXMFLOAT2());
			}
			
			for (int i = 0; i < tmp.size(); i++)
			{
				vertices[i].pos = { tmp[i].x,-tmp[i].y,0.0f };
			}

			KazRenderHelper::InitUvPos(&vertices[0].uv, &vertices[1].uv, &vertices[2].uv, &vertices[3].uv);
		}
		//�����_�[�^�[�Q�b�g�̃e�N�X�`���T�C�Y
		else
		{
			//�T�C�Y��XMFLOAT3����XMFLOAT2�ɒ���
			KazMath::Vec2<float> tmpSize = data.transform.scale;

			texSize.x = static_cast<int>(RenderTargetStatus::Instance()->GetBufferData(data.handleData.handle)->GetDesc().Width);
			texSize.y = static_cast<int>(RenderTargetStatus::Instance()->GetBufferData(data.handleData.handle)->GetDesc().Height);

			KazMath::Vec2<float> leftUp, rightDown;
			leftUp = { 0.0f,0.0f };
			rightDown = { 1.0f,1.0f };

			//�T�C�Y�ύX
			array<KazMath::Vec2<float>, 4>tmp = KazRenderHelper::ChangePlaneScale(leftUp, rightDown, tmpSize, anchorPoint, texSize);
			for (int i = 0; i < tmp.size(); i++)
			{
				vertices[i].pos = { tmp[i].x,-tmp[i].y,0.0f };
			}
		}
	}


	if (data.handleData.flag.Dirty())
	{

	}


	//UV�؂���
	if (data.handleData.flag.Dirty() || data.animationHandle.flag.Dirty())
	{
		KazMath::Vec2<int> divSize = renderData.shaderResourceMgrInstance->GetDivData(data.handleData.handle).divSize;
		KazMath::Vec2<float> tmpSize = { data.transform.scale.x, data.transform.scale.y };


		bool isItSafeToUseAnimationHandleFlag = KazHelper::IsitInAnArray(data.animationHandle.handle, renderData.shaderResourceMgrInstance->GetDivData(data.handleData.handle).divLeftUp.size());
		bool isItSafeToUseDivDataFlag = (divSize.x != -1 && divSize.y != -1);


		if (isItSafeToUseDivDataFlag && isItSafeToUseAnimationHandleFlag)
		{
			KazMath::Vec2<int> divLeftUpPos = renderData.shaderResourceMgrInstance->GetDivData(data.handleData.handle).divLeftUp[data.animationHandle.handle];
			KazRenderHelper::VerticesCut(divSize, divLeftUpPos, &vertices[0].pos, &vertices[1].pos, &vertices[2].pos, &vertices[3].pos, tmpSize, anchorPoint);
			KazRenderHelper::UVCut(divLeftUpPos, divSize, texSize, &vertices[0].uv, &vertices[1].uv, &vertices[2].uv, &vertices[3].uv);

			for (int i = 0; i < vertices.size(); i++)
			{
				vertices[i].pos.y *= -1.0f;
			}
		}
	}


	//X����UV�𔽓]
	if(data.flip.xDirtyFlag.Dirty())
	{
		KazRenderHelper::FlipXUv(&vertices[0].uv, &vertices[1].uv, &vertices[2].uv, &vertices[3].uv);
	}
	//Y����UV�𔽓]
	if (data.flip.yDirtyFlag.Dirty())
	{
		KazRenderHelper::FlipYUv(&vertices[0].uv, &vertices[1].uv, &vertices[2].uv, &vertices[3].uv);
	}



	//���_�f�[�^�ɉ����ύX����������]������
	if (data.handleData.flag.Dirty() || data.transform.scaleDirtyFlag.Dirty() || data.animationHandle.flag.Dirty())
	{
		gpuBuffer->TransData(vertexBufferHandle, vertices.data(), vertByte);
	}
	//���_�f�[�^�̏���������UV�̏�������-----------------------------------------------------------------------------------------------------


	//�o�b�t�@�̓]��-----------------------------------------------------------------------------------------------------
	//�s��
	if (data.transform.Dirty() || renderData.cameraMgrInstance->Dirty())
	{
		ConstBufferData constMap;
		constMap.world = baseMatWorldData.matWorld;
		constMap.view = XMMatrixIdentity();
		constMap.viewproj = renderData.cameraMgrInstance->orthographicMatProjection;
		constMap.color = { 0.0f,0.0f,0.0f,data.alpha / 255.0f };
		constMap.mat = constMap.world * constMap.viewproj;
		TransData(&constMap, constBufferHandle, typeid(ConstBufferData).name());
	}
	//�o�b�t�@�̓]��-----------------------------------------------------------------------------------------------------


	//�o�b�t�@���R�}���h���X�g�ɐς�-----------------------------------------------------------------------------------------------------
	SetConstBufferOnCmdList(data.pipelineName);


	if (data.handleData.handle != -1)
	{
		D3D12_GPU_DESCRIPTOR_HANDLE gpuDescHandleSRV = DescriptorHeapMgr::Instance()->GetGpuDescriptorView(data.handleData.handle);
		int param = KazRenderHelper::SetBufferOnCmdList(GraphicsRootSignature::Instance()->GetRootParam(renderData.pipelineMgr->GetRootSignatureName(data.pipelineName)), GRAPHICS_RANGE_TYPE_SRV, GRAPHICS_PRAMTYPE_TEX);
		renderData.cmdListInstance->cmdList->SetGraphicsRootDescriptorTable(param, gpuDescHandleSRV);
	}

	for (int i = 0; i < data.addHandle.handle.size(); ++i)
	{
		renderData.shaderResourceMgrInstance->SetSRV(data.addHandle.handle[i], GraphicsRootSignature::Instance()->GetRootParam(renderData.pipelineMgr->GetRootSignatureName(data.pipelineName)), data.addHandle.paramType[i]);
	}
	//�o�b�t�@���R�}���h���X�g�ɐς�-----------------------------------------------------------------------------------------------------




	//�`�施��-----------------------------------------------------------------------------------------------------
	DrawCommand(drawCommandData);
	//�`�施��-----------------------------------------------------------------------------------------------------


	//DirtyFlag�̍X�V-----------------------------------------------------------------------------------------------------
	data.Record();
	//DirtyFlag�̍X�V-----------------------------------------------------------------------------------------------------
}