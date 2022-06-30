#include "Sprite3DRender.h"
#include"../Helper/OutPutDebugStringAndCheckResult.h"
#include"../Helper/KazRenderHelper.h"
#include"../Helper/KazHelper.h"
#include"../Buffer/DescriptorHeapMgr.h"
#include"../RenderTarget/RenderTargetStatus.h"

Sprite3DRender::Sprite3DRender(const KazMath::Vec2<float> ANCHOR_POINT)
{
	anchorPoint = ANCHOR_POINT;

	gpuBuffer = std::make_unique<CreateGpuBuffer>();



	//�f�[�^�̒�`-----------------------------------------------------------------------------------------------------
	//���_�f�[�^
	KazRenderHelper::InitVerticesPos(&vertices[0].pos, &vertices[1].pos, &vertices[2].pos, &vertices[3].pos, anchorPoint.ConvertXMFLOAT2());
	KazRenderHelper::InitUvPos(&vertices[0].uv, &vertices[1].uv, &vertices[2].uv, &vertices[3].uv);
	//�C���f�b�N�X�f�[�^
	indices = KazRenderHelper::InitIndciesForPlanePolygon();
	//�f�[�^�̒�`-----------------------------------------------------------------------------------------------------

	
	VertByte = KazBufferHelper::GetBufferSize<UINT>(vertices.size(), sizeof(SpriteVertex));
	IndexByte = KazBufferHelper::GetBufferSize<UINT>(indices.size(), sizeof(unsigned short));


	//�o�b�t�@����-----------------------------------------------------------------------------------------------------
	//���_�o�b�t�@
	vertexBufferHandle = gpuBuffer->CreateBuffer
	(
		KazBufferHelper::SetVertexBufferData(VertByte)
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
	gpuBuffer->TransData(vertexBufferHandle, vertices.data(), VertByte);
	gpuBuffer->TransData(indexBufferHandle, indices.data(), IndexByte);
	//�o�b�t�@�]��-----------------------------------------------------------------------------------------------------


	//�r���[�̐ݒ�-----------------------------------------------------------------------------------------------------
	vertexBufferView = KazBufferHelper::SetVertexBufferView(gpuBuffer->GetGpuAddress(vertexBufferHandle), VertByte, sizeof(vertices[0]));
	indexBufferView = KazBufferHelper::SetIndexBufferView(gpuBuffer->GetGpuAddress(indexBufferHandle), IndexByte);
	//�r���[�̐ݒ�-----------------------------------------------------------------------------------------------------
}

Sprite3DRender::~Sprite3DRender()
{
}

void Sprite3DRender::Draw()
{
	//�p�C�v���C���ݒ�-----------------------------------------------------------------------------------------------------
	pipeline = static_cast<PipeLineNames>(data.pipelineName);
	renderData.pipelineMgr->SetPipeLineAndRootSignature(pipeline);
	//�p�C�v���C���ݒ�-----------------------------------------------------------------------------------------------------




	//DirtyFlag���m-----------------------------------------------------------------------------------------------------	
	//DirtyFlag���m-----------------------------------------------------------------------------------------------------



	//�s��v�Z-----------------------------------------------------------------------------------------------------
	{
		baseMatWorldData.matWorld = XMMatrixIdentity();
		baseMatWorldData.matScale = KazMath::CaluScaleMatrix(data.transform.scale);
		baseMatWorldData.matTrans = KazMath::CaluTransMatrix(data.transform.pos);
		baseMatWorldData.matRota = KazMath::CaluRotaMatrix(data.transform.rotation);


		baseMatWorldData.matWorld *= baseMatWorldData.matScale;
		baseMatWorldData.matWorld *= baseMatWorldData.matRota;
		//�r���{�[�h�s����|����
		if (data.billBoardFlag)
		{
			baseMatWorldData.matWorld *= renderData.cameraMgrInstance->GetMatBillBoard(data.cameraIndex);
		}
		baseMatWorldData.matWorld *= baseMatWorldData.matTrans;
		baseMatWorldData.matWorld *= data.motherMat;


		//�e�s����|����
		motherMat = baseMatWorldData.matWorld;
	}
	//�s��v�Z-----------------------------------------------------------------------------------------------------



	//���_�f�[�^�̏���������UV�̏�������-----------------------------------------------------------------------------------------------------

	//�ǂݍ��񂾃e�N�X�`���̃T�C�Y
	//�ǂݍ��񂾉摜�̃T�C�Y�����킹��
	{
		if (DescriptorHeapMgr::Instance()->GetType(data.handle) != DESCRIPTORHEAP_MEMORY_TEXTURE_RENDERTARGET)
		{

			KazMath::Vec2<float> tmpSize = {};
			XMFLOAT4 tmpModiSize = {};
			//�T�C�Y��XMFLOAT3����XMFLOAT2�ɒ���
			if (!data.changeSizeTypeFlag)
			{
				tmpSize = { data.transform.scale.x, data.transform.scale.y };
			}
			else
			{
				tmpModiSize = data.size;
			}


			texSize.x = static_cast<int>(renderData.shaderResourceMgrInstance->GetTextureSize(data.handle).Width);
			texSize.y = static_cast<int>(renderData.shaderResourceMgrInstance->GetTextureSize(data.handle).Height);


			KazMath::Vec2<float> leftUp, rightDown;
			leftUp = { 0.0f,0.0f };
			rightDown = { 1.0f,1.0f };

			array<KazMath::Vec2<float>, 4>tmp;
			//�T�C�Y�ύX
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
				vertices[i].pos = { tmp[i].x,tmp[i].y,0.0f };
			}

		}
		//�����_�[�^�[�Q�b�g�̃e�N�X�`���T�C�Y
		else
		{
			//�T�C�Y��XMFLOAT3����XMFLOAT2�ɒ���
			KazMath::Vec2<float> tmpSize = { data.transform.scale.x, data.transform.scale.y };

			texSize.x = static_cast<int>(RenderTargetStatus::Instance()->GetBufferData(data.handle)->GetDesc().Width);
			texSize.y = static_cast<int>(RenderTargetStatus::Instance()->GetBufferData(data.handle)->GetDesc().Height);

			KazMath::Vec2<float> leftUp, rightDown;
			leftUp = { 0.0f,0.0f };
			rightDown = { 1.0f,1.0f };

			//�T�C�Y�ύX
			array<KazMath::Vec2<float>, 4>tmp = KazRenderHelper::ChangePlaneScale(leftUp, rightDown, tmpSize, anchorPoint, texSize);
			for (int i = 0; i < tmp.size(); i++)
			{
				vertices[i].pos = { tmp[i].x,tmp[i].y,0.0f };
			}
		}
	}

	//UV�؂���
	{
		KazMath::Vec2<int> divSize = renderData.shaderResourceMgrInstance->GetDivData(data.handle).divSize;
		KazMath::Vec2<float> tmpSize = { data.transform.scale.x, data.transform.scale.y };

		bool isItSafeToUseAnimationHandleFlag = KazHelper::IsitInAnArray(data.animationHandle, renderData.shaderResourceMgrInstance->GetDivData(data.handle).divLeftUp.size());
		bool isItSafeToUseDivDataFlag = (divSize.x != -1 && divSize.y != -1);

		if (isItSafeToUseDivDataFlag && isItSafeToUseAnimationHandleFlag)
		{
			KazMath::Vec2<int> divLeftUpPos = renderData.shaderResourceMgrInstance->GetDivData(data.handle).divLeftUp[data.animationHandle];


			KazRenderHelper::VerticesCut(divSize, divLeftUpPos, &vertices[0].pos, &vertices[1].pos, &vertices[2].pos, &vertices[3].pos, tmpSize, anchorPoint);
			KazRenderHelper::UVCut(divLeftUpPos, divSize, texSize, &vertices[0].uv, &vertices[1].uv, &vertices[2].uv, &vertices[3].uv);
		}
	}

	//X����UV�𔽓]
	{
		KazRenderHelper::FlipXUv(&vertices[0].uv, &vertices[1].uv, &vertices[2].uv, &vertices[3].uv);
	}
	//Y����UV�𔽓]
	{
		KazRenderHelper::FlipYUv(&vertices[0].uv, &vertices[1].uv, &vertices[2].uv, &vertices[3].uv);
	}



	//���_�f�[�^�ɉ����ύX����������]������
	{
		gpuBuffer->TransData(vertexBufferHandle, vertices.data(), VertByte);
	}
	//���_�f�[�^�̏���������UV�̏�������-----------------------------------------------------------------------------------------------------


	//�o�b�t�@�̓]��-----------------------------------------------------------------------------------------------------
	//�s��
	//if (lMatrixDirtyFlag || lMatFlag)
	//{
	ConstBufferData constMap;
	constMap.world = baseMatWorldData.matWorld;
	constMap.view = renderData.cameraMgrInstance->GetViewMatrix(data.cameraIndex);
	constMap.viewproj = renderData.cameraMgrInstance->GetPerspectiveMatProjection();
	constMap.color = { 0.0f,0.0f,0.0f,data.alpha / 255.0f };
	constMap.mat = constMap.world * constMap.view * constMap.viewproj;

	//gpuBuffer->TransData();
	TransData(&constMap, constBufferHandle, typeid(ConstBufferData).name());
	//}
	//�o�b�t�@�̓]��-----------------------------------------------------------------------------------------------------


	//�o�b�t�@���R�}���h���X�g�ɐς�-----------------------------------------------------------------------------------------------------
	SetConstBufferOnCmdList(pipeline);
	renderData.shaderResourceMgrInstance->SetSRV(data.handle, GraphicsRootSignature::Instance()->GetRootParam(renderData.pipelineMgr->GetRootSignatureName(pipeline)), GRAPHICS_PRAMTYPE_TEX);
	//�o�b�t�@���R�}���h���X�g�ɐς�-----------------------------------------------------------------------------------------------------

	//�`�施��-----------------------------------------------------------------------------------------------------
	renderData.cmdListInstance->cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	renderData.cmdListInstance->cmdList->IASetVertexBuffers(0, 1, &vertexBufferView);
	renderData.cmdListInstance->cmdList->IASetIndexBuffer(&indexBufferView);
	renderData.cmdListInstance->cmdList->DrawIndexedInstanced(static_cast<UINT>(indices.size()), 1, 0, 0, 0);
	//�`�施��-----------------------------------------------------------------------------------------------------



	//DirtyFlag�̍X�V-----------------------------------------------------------------------------------------------------
	//DirtyFlag�̍X�V-----------------------------------------------------------------------------------------------------
}

XMMATRIX Sprite3DRender::GetMotherMatrix()
{
	return motherMat;
}
