#include "CircleRender.h"

CircleRender::CircleRender()
{
	const USHORT VERT_NUMBER = 300;
	const float RADIUS = 10;


	std::vector<Vertex> vert(VERT_NUMBER);
	std::vector<USHORT> indi(VERT_NUMBER * 3);

	indicesNum = static_cast<UINT>(indi.size());
	float PI_F2 = XM_2PI;


	//���_�f�[�^
	for (int i = 0; i < vert.size(); i++) {
		vert[i].pos.x = (RADIUS * sin((PI_F2 / VERT_NUMBER) * i));
		vert[i].pos.y = -(RADIUS * cos((PI_F2 / VERT_NUMBER) * i));
		vert[i].pos.z = 0.0f;

		vert[i].uv.x = 1;
		vert[i].uv.y = 1;
	}

	//�C���f�b�N�X�f�[�^
	for (int i = 0; i < VERT_NUMBER; i++) {
		indi[i * 3] = VERT_NUMBER;	//���S
		indi[i * 3 + 1] = static_cast<USHORT>(i);			//�ė��p
		indi[i * 3 + 2] = static_cast<USHORT>(i + 1);		//�V�������_
	}
	indi[indi.size() - 1] = 0;

	BUFFER_SIZE VertByte = KazBufferHelper::GetBufferSize<BUFFER_SIZE>(vert.size(), sizeof(Vertex));
	BUFFER_SIZE IndexByte = KazBufferHelper::GetBufferSize<BUFFER_SIZE>(indi.size(), sizeof(USHORT));


	gpuBuffer = std::make_unique<CreateGpuBuffer>();

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
	gpuBuffer->TransData(vertexBufferHandle, vert.data(), VertByte);
	gpuBuffer->TransData(indexBufferHandle, indi.data(), IndexByte);
	//�o�b�t�@�]��-----------------------------------------------------------------------------------------------------


	//�r���[�̐ݒ�-----------------------------------------------------------------------------------------------------
	vertexBufferView = KazBufferHelper::SetVertexBufferView(gpuBuffer->GetGpuAddress(vertexBufferHandle), VertByte, sizeof(vert[0]));
	indexBufferView = KazBufferHelper::SetIndexBufferView(gpuBuffer->GetGpuAddress(indexBufferHandle), IndexByte);
	//�r���[�̐ݒ�-----------------------------------------------------------------------------------------------------

}

void CircleRender::Draw()
{
	//�p�C�v���C���ݒ�-----------------------------------------------------------------------------------------------------
	pipeline = static_cast<PipeLineNames>(data.pipelineName);
	renderData.pipelineMgr->SetPipeLineAndRootSignature(pipeline);
	//�p�C�v���C���ݒ�-----------------------------------------------------------------------------------------------------

	//�s��v�Z-----------------------------------------------------------------------------------------------------
	if (data.change3DFlag)
	{
		baseMatWorldData.matWorld = XMMatrixIdentity();
		baseMatWorldData.matScale = KazMath::CaluScaleMatrix(data.transform.scale);
		baseMatWorldData.matTrans = KazMath::CaluTransMatrix(data.transform.pos);
		baseMatWorldData.matRota = KazMath::CaluRotaMatrix(data.transform.rotation);

		//���[���h�s��̌v�Z
		baseMatWorldData.matWorld = XMMatrixIdentity();
		baseMatWorldData.matWorld *= baseMatWorldData.matScale;
		baseMatWorldData.matWorld *= baseMatWorldData.matRota;
		baseMatWorldData.matWorld *= baseMatWorldData.matTrans;
	}
	else
	{
		baseMatWorldData.matWorld = XMMatrixIdentity();
		baseMatWorldData.matScale = KazMath::CaluScaleMatrix(data.transform.scale);
		baseMatWorldData.matTrans = KazMath::CaluTransMatrix(data.transform.pos);

		//���[���h�s��̌v�Z
		baseMatWorldData.matWorld = XMMatrixIdentity();
		baseMatWorldData.matWorld *= baseMatWorldData.matScale;
		baseMatWorldData.matWorld *= baseMatWorldData.matTrans;
	}
	//�s��v�Z-----------------------------------------------------------------------------------------------------


	const int VERT_NUMBER = 300;
	const float RADIUS = data.radius;

	std::vector<Vertex> vert(VERT_NUMBER);

	float PI_F2 = XM_2PI;
	//���_�f�[�^
	for (int i = 0; i < vert.size(); i++) {
		vert[i].pos.x = (RADIUS * sin((PI_F2 / VERT_NUMBER) * i));
		vert[i].pos.y = -(RADIUS * cos((PI_F2 / VERT_NUMBER) * i));
		vert[i].pos.z = 0.0f;

		vert[i].uv.x = 1.0f;
		vert[i].uv.y = 1.0f;
	}
	BUFFER_SIZE VertByte = KazBufferHelper::GetBufferSize<BUFFER_SIZE>(vert.size(), sizeof(Vertex));
	gpuBuffer->TransData(vertexBufferHandle, vert.data(), VertByte);





	//�o�b�t�@�̓]��-----------------------------------------------------------------------------------------------------
	
	ConstBufferData constMap;
	//�s��
	if (data.change3DFlag)
	{
		constMap.world = baseMatWorldData.matWorld;
		constMap.view = renderData.cameraMgrInstance->GetViewMatrix();
		constMap.viewproj = renderData.cameraMgrInstance->GetPerspectiveMatProjection();
		constMap.color = KazRenderHelper::SendColorDataToGPU(data.color);
		constMap.mat = constMap.world * constMap.view * constMap.viewproj;
	}
	else
	{
		constMap.world = baseMatWorldData.matWorld;
		constMap.view = XMMatrixIdentity();
		constMap.viewproj = renderData.cameraMgrInstance->orthographicMatProjection;
		constMap.color = KazRenderHelper::SendColorDataToGPU(data.color);
		constMap.mat = constMap.world * constMap.viewproj;
	}
	TransData(&constMap, constBufferHandle, typeid(ConstBufferData).name());
	//�o�b�t�@�̓]��-----------------------------------------------------------------------------------------------------


	//�o�b�t�@���R�}���h���X�g�ɐς�-----------------------------------------------------------------------------------------------------
	SetConstBufferOnCmdList(pipeline);
	//�o�b�t�@���R�}���h���X�g�ɐς�-----------------------------------------------------------------------------------------------------

	//�`�施��-----------------------------------------------------------------------------------------------------
	renderData.cmdListInstance->cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	renderData.cmdListInstance->cmdList->IASetVertexBuffers(0, 1, &vertexBufferView);
	renderData.cmdListInstance->cmdList->IASetIndexBuffer(&indexBufferView);
	renderData.cmdListInstance->cmdList->DrawIndexedInstanced(indicesNum, 1, 0, 0, 0);
	//�`�施��-----------------------------------------------------------------------------------------------------

}
