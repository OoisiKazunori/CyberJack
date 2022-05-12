#include "PolygonRender.h"
#include"../KazLibrary/Helper/KazHelper.h"

PolygonRender::PolygonRender(const array<SpriteVertex, 4> &DATA)
{
	anchorPoint = { 0.5f,0.5f };

	gpuBuffer = std::make_unique<CreateGpuBuffer>();

	positionDirtyFlag = std::make_unique<DirtySet>(data.transform.pos);
	scaleDirtyFlag = std::make_unique<DirtySet>(data.transform.scale);
	rotationDirtyFlag = std::make_unique<DirtySet>(data.transform.rotation);

	cameraViewDirtyFlag = std::make_unique<DirtySet>(renderData.cameraMgrInstance->view);
	cameraProjectionDirtyFlag = std::make_unique<DirtySet>(renderData.cameraMgrInstance->perspectiveMat);
	cameraBillBoardDirtyFlag = std::make_unique<DirtySet>(renderData.cameraMgrInstance->billBoard);

	sizeDirtyFlag = std::make_unique<DirtySet>(data.size);

	motherDirtyFlag = std::make_unique<DirtySet>(data.motherMat);


	//�f�[�^�̒�`-----------------------------------------------------------------------------------------------------
	//���_�f�[�^
	//KazRenderHelper::InitVerticesPos(&vertices[0].pos, &vertices[1].pos, &vertices[2].pos, &vertices[3].pos, anchorPoint);
	KazRenderHelper::InitUvPos(&vertices[0].uv, &vertices[1].uv, &vertices[2].uv, &vertices[3].uv);
	//�C���f�b�N�X�f�[�^
	indices = KazRenderHelper::InitIndciesForPlanePolygon();
	//�f�[�^�̒�`-----------------------------------------------------------------------------------------------------

	vertices = DATA;


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

void PolygonRender::Draw()
{

	//�p�C�v���C���ݒ�-----------------------------------------------------------------------------------------------------
	pipeline = static_cast<PipeLineNames>(data.pipelineName);
	renderData.pipelineMgr->SetPipeLineAndRootSignature(pipeline);
	//�p�C�v���C���ݒ�-----------------------------------------------------------------------------------------------------


	//DirtyFlag���m-----------------------------------------------------------------------------------------------------	
	bool matFlag = cameraViewDirtyFlag->FloatDirty() || cameraProjectionDirtyFlag->FloatDirty() || cameraBillBoardDirtyFlag->FloatDirty() || motherDirtyFlag->FloatDirty();
	bool matrixDirtyFlag = positionDirtyFlag->FloatDirty() || scaleDirtyFlag->FloatDirty() || rotationDirtyFlag->FloatDirty();
	bool scaleDirtyFlag = this->scaleDirtyFlag->FloatDirty();

	bool localsizeDirtyFlag = sizeDirtyFlag->FloatDirty();
	//DirtyFlag���m-----------------------------------------------------------------------------------------------------



	//�s��v�Z-----------------------------------------------------------------------------------------------------
	if (matrixDirtyFlag || matFlag)
	{
		baseMatWorldData.matWorld = XMMatrixIdentity();
		baseMatWorldData.matScale = KazMath::CaluScaleMatrix(data.transform.scale);
		baseMatWorldData.matTrans = KazMath::CaluTransMatrix(data.transform.pos);
		baseMatWorldData.matRota = KazMath::CaluRotaMatrix(data.transform.rotation);
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
	sizeDirtyFlag->Record();

	cameraBillBoardDirtyFlag->Record();
	cameraProjectionDirtyFlag->Record();
	cameraViewDirtyFlag->Record();
	motherDirtyFlag->Record();
	//DirtyFlag�̍X�V-----------------------------------------------------------------------------------------------------
}
