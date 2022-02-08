#include "Sprite3DRender.h"
#include"../Helper/OutPutDebugStringAndCheckResult.h"
#include"../Helper/KazRenderHelper.h"
#include"../Helper/KazHelper.h"

Sprite3DRender::Sprite3DRender()
{
	anchorPoint = { 0.5f,0.5f };

	gpuBuffer.reset(new CreateGpuBuffer);

	positionDirtyFlag.reset(new DirtySet(data.transform.pos));
	scaleDirtyFlag.reset(new DirtySet(data.transform.scale));
	rotationDirtyFlag.reset(new DirtySet(data.transform.rotation));

	flipXDirtyFlag.reset(new DirtyFlag<bool>(&data.flip.x,false));
	flipYDirtyFlag.reset(new DirtyFlag<bool>(&data.flip.y,false));

	textureHandleDirtyFlag.reset(new DirtyFlag<short>(&data.handle));
	animationHandleDirtyFlag.reset(new DirtyFlag<short>(&data.animationHandle));


	cameraViewDirtyFlag.reset(new DirtySet(renderData.cameraMgrInstance->view));
	cameraProjectionDirtyFlag.reset(new DirtySet(renderData.cameraMgrInstance->perspectiveMat));
	cameraBillBoardDirtyFlag.reset(new DirtySet(renderData.cameraMgrInstance->billBoard));

	sizeDirtyFlag.reset(new DirtySet(data.size));


	//�f�[�^�̒�`-----------------------------------------------------------------------------------------------------
	//���_�f�[�^
	KazRenderHelper::InitVerticesPos(&vertices[0].pos, &vertices[1].pos, &vertices[2].pos, &vertices[3].pos, anchorPoint);
	KazRenderHelper::InitUvPos(&vertices[0].uv, &vertices[1].uv, &vertices[2].uv, &vertices[3].uv);
	//�C���f�b�N�X�f�[�^
	indices = KazRenderHelper::InitIndciesForPlanePolygon();
	//�f�[�^�̒�`-----------------------------------------------------------------------------------------------------


	VertByte = vertices.size() * sizeof(SpriteVertex);
	IndexByte = indices.size() * sizeof(unsigned short);


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
	bool matFlag = cameraViewDirtyFlag->FloatDirty() || cameraProjectionDirtyFlag->FloatDirty() || cameraBillBoardDirtyFlag->FloatDirty();
	bool matrixDirtyFlag = positionDirtyFlag->FloatDirty() || scaleDirtyFlag->FloatDirty() || rotationDirtyFlag->FloatDirty();
	bool scaleDirtyFlag = this->scaleDirtyFlag->FloatDirty();

	bool localsizeDirtyFlag = sizeDirtyFlag->FloatDirty();

	bool flipXDirtyFlag = this->flipXDirtyFlag->Dirty();
	bool flipYDirtyFlag = this->flipYDirtyFlag->Dirty();

	bool textureHandleDirtyFlag = this->textureHandleDirtyFlag->Dirty();
	bool animationHandleDirtyFlag = this->animationHandleDirtyFlag->Dirty();

	bool verticesDirtyFlag = flipXDirtyFlag || flipYDirtyFlag || textureHandleDirtyFlag || animationHandleDirtyFlag || scaleDirtyFlag || localsizeDirtyFlag;
	//DirtyFlag���m-----------------------------------------------------------------------------------------------------



	//�s��v�Z-----------------------------------------------------------------------------------------------------
	if (matrixDirtyFlag || matFlag)
	{
		baseMatWorldData.matWorld = XMMatrixIdentity();
		baseMatWorldData.matScale = KazMath::CaluScaleMatrix(data.transform.scale);
		baseMatWorldData.matTrans = KazMath::CaluTransMatrix(data.transform.pos);
		baseMatWorldData.matRota = KazMath::CaluRotaMatrix(data.transform.rotation);

		//���[���h�s��̌v�Z
		baseMatWorldData.matWorld = XMMatrixIdentity();
		//�r���{�[�h�s����|����
		if (data.billBoardFlag)
		{
			baseMatWorldData.matWorld *= renderData.cameraMgrInstance->GetMatBillBoard();
		}
		baseMatWorldData.matWorld *= baseMatWorldData.matScale;
		baseMatWorldData.matWorld *= baseMatWorldData.matRota;
		baseMatWorldData.matWorld *= baseMatWorldData.matTrans;
		baseMatWorldData.matWorld *= data.motherMat;

		//�e�s����|����
		motherMat = baseMatWorldData.matWorld;
	}
	//�s��v�Z-----------------------------------------------------------------------------------------------------



	//���_�f�[�^�̏���������UV�̏�������-----------------------------------------------------------------------------------------------------

	//�ǂݍ��񂾃e�N�X�`���̃T�C�Y
	//�ǂݍ��񂾉摜�̃T�C�Y�����킹��
	if (textureHandleDirtyFlag || scaleDirtyFlag || localsizeDirtyFlag)
	{
		XMFLOAT2 tmpSize = {};
		XMFLOAT4 tmpModiSize = {};
		//�T�C�Y��XMFLOAT3����XMFLOAT2�ɒ���
		if (!data.changeSizeTypeFlag)
		{
			tmpSize = { data.transform.scale.m128_f32[0], data.transform.scale.m128_f32[1] };
		}
		else
		{
			tmpModiSize = data.size;
		}
		texSize.x = static_cast<float>(renderData.shaderResourceMgrInstance->GetTextureSize(data.handle).Width);
		texSize.y = static_cast<float>(renderData.shaderResourceMgrInstance->GetTextureSize(data.handle).Height);


		XMFLOAT2 leftUp, rightDown;
		leftUp = { 0.0f,0.0f };
		rightDown = { 1.0f,1.0f };

		array<XMFLOAT2, 4>tmp;
		//�T�C�Y�ύX
		if (!data.changeSizeTypeFlag)
		{
			tmp = KazRenderHelper::ChangePlaneScale(leftUp, rightDown, tmpSize, anchorPoint, texSize);
		}
		else
		{
			tmp = KazRenderHelper::ChangeModiPlaneScale(leftUp, rightDown, tmpModiSize, anchorPoint, texSize);
		}


		for (int i = 0; i < tmp.size(); i++)
		{
			vertices[i].pos = { tmp[i].x,tmp[i].y,0.0f };
		}
	}

	//UV�؂���
	if (animationHandleDirtyFlag || scaleDirtyFlag)
	{
		XMFLOAT2 divSize = renderData.shaderResourceMgrInstance->GetDivData(data.handle).divSize;
		XMFLOAT2 tmpSize = { data.transform.scale.m128_f32[0], data.transform.scale.m128_f32[1] };

		bool isItSafeToUseAnimationHandleFlag = KazHelper::IsitInAnArray(data.animationHandle, renderData.shaderResourceMgrInstance->GetDivData(data.handle).divLeftUp.size());
		bool isItSafeToUseDivDataFlag = (divSize.x != -1 && divSize.y != -1);

		if (isItSafeToUseDivDataFlag && isItSafeToUseAnimationHandleFlag)
		{
			XMFLOAT2 divLeftUpPos = renderData.shaderResourceMgrInstance->GetDivData(data.handle).divLeftUp[data.animationHandle];


			KazRenderHelper::VerticesCut(divSize, divLeftUpPos, &vertices[0].pos, &vertices[1].pos, &vertices[2].pos, &vertices[3].pos, tmpSize, anchorPoint);
			KazRenderHelper::UVCut(divLeftUpPos, divSize, texSize, &vertices[0].uv, &vertices[1].uv, &vertices[2].uv, &vertices[3].uv);
		}
	}

	//X����UV�𔽓]
	if (flipXDirtyFlag)
	{
		KazRenderHelper::FlipXUv(&vertices[0].uv, &vertices[1].uv, &vertices[2].uv, &vertices[3].uv);
	}
	//Y����UV�𔽓]
	if (flipYDirtyFlag)
	{
		KazRenderHelper::FlipYUv(&vertices[0].uv, &vertices[1].uv, &vertices[2].uv, &vertices[3].uv);
	}



	//���_�f�[�^�ɉ����ύX����������]������
	if (verticesDirtyFlag)
	{
		gpuBuffer->TransData(vertexBufferHandle, vertices.data(), VertByte);
	}
	//���_�f�[�^�̏���������UV�̏�������-----------------------------------------------------------------------------------------------------


	//�o�b�t�@�̓]��-----------------------------------------------------------------------------------------------------
	//�s��
	//if (matrixDirtyFlag || matFlag)
	//{
		ConstBufferData constMap;
		constMap.world = baseMatWorldData.matWorld;
		constMap.view = renderData.cameraMgrInstance->GetViewMatrix();
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
	renderData.cmdListInstance->cmdList->DrawIndexedInstanced(indices.size(), 1, 0, 0, 0);
	//�`�施��-----------------------------------------------------------------------------------------------------



	//DirtyFlag�̍X�V-----------------------------------------------------------------------------------------------------
	positionDirtyFlag->Record();
	this->scaleDirtyFlag->Record();
	rotationDirtyFlag->Record();
	this->flipXDirtyFlag->Record();
	this->flipYDirtyFlag->Record();
	this->textureHandleDirtyFlag->Record();
	this->animationHandleDirtyFlag->Record();

	sizeDirtyFlag->Record();

	cameraBillBoardDirtyFlag->Record();
	cameraProjectionDirtyFlag->Record();
	cameraViewDirtyFlag->Record();
	//DirtyFlag�̍X�V-----------------------------------------------------------------------------------------------------
}

XMMATRIX Sprite3DRender::GetMotherMatrix()
{
	return motherMat;
}
