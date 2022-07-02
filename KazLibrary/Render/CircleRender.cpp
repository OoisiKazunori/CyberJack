#include "CircleRender.h"

CircleRender::CircleRender()
{
	const USHORT VERT_NUMBER = 300;
	const float RADIUS = 10;


	std::vector<Vertex> lVert(VERT_NUMBER);
	std::vector<USHORT> lIndi(VERT_NUMBER * 3);

	indicesNum = static_cast<UINT>(lIndi.size());
	const float PI_F2 = XM_2PI;


	//���_�f�[�^
	for (int i = 0; i < lVert.size(); i++)
	{
		lVert[i].pos.x = (RADIUS * sin((PI_F2 / VERT_NUMBER) * i));
		lVert[i].pos.y = -(RADIUS * cos((PI_F2 / VERT_NUMBER) * i));
		lVert[i].pos.z = 0.0f;

		lVert[i].uv.x = 1;
		lVert[i].uv.y = 1;
	}

	//�C���f�b�N�X�f�[�^
	for (int i = 0; i < VERT_NUMBER; i++)
	{
		lIndi[i * 3] = VERT_NUMBER;	//���S
		lIndi[i * 3 + 1] = static_cast<USHORT>(i);			//�ė��p
		lIndi[i * 3 + 2] = static_cast<USHORT>(i + 1);		//�V�������_
	}
	lIndi[lIndi.size() - 1] = 0;

	BUFFER_SIZE VertByte = KazBufferHelper::GetBufferSize<BUFFER_SIZE>(lVert.size(), sizeof(Vertex));
	BUFFER_SIZE IndexByte = KazBufferHelper::GetBufferSize<BUFFER_SIZE>(lIndi.size(), sizeof(USHORT));


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
	gpuBuffer->TransData(vertexBufferHandle, lVert.data(), VertByte);
	gpuBuffer->TransData(indexBufferHandle, lIndi.data(), IndexByte);
	//�o�b�t�@�]��-----------------------------------------------------------------------------------------------------

	drawCommandData = KazRenderHelper::SetDrawCommandData(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST, 
		KazBufferHelper::SetVertexBufferView(gpuBuffer->GetGpuAddress(vertexBufferHandle), VertByte, sizeof(lVert[0])),
		KazBufferHelper::SetIndexBufferView(gpuBuffer->GetGpuAddress(indexBufferHandle), IndexByte),
		indicesNum,
		1
	);
}

void CircleRender::Draw()
{
	//�p�C�v���C���ݒ�-----------------------------------------------------------------------------------------------------
	renderData.pipelineMgr->SetPipeLineAndRootSignature(data.pipelineName);
	//�p�C�v���C���ݒ�-----------------------------------------------------------------------------------------------------

	//�s��v�Z-----------------------------------------------------------------------------------------------------
	if (data.transform.Dirty()||data.change3DDirtyFlag.Dirty())
	{
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
	}
	//�s��v�Z-----------------------------------------------------------------------------------------------------

	//�~�̊g�k-------------------------
	if (data.radiusDirtyFlag.Dirty())
	{
		const int VERT_NUMBER = 300;
		const float RADIUS = data.radius;

		std::vector<Vertex> lVert(VERT_NUMBER);

		float PI_F2 = XM_2PI;
		//���_�f�[�^
		for (int i = 0; i < lVert.size(); i++) {
			lVert[i].pos.x = (RADIUS * sin((PI_F2 / VERT_NUMBER) * i));
			lVert[i].pos.y = -(RADIUS * cos((PI_F2 / VERT_NUMBER) * i));
			lVert[i].pos.z = 0.0f;

			lVert[i].uv.x = 1.0f;
			lVert[i].uv.y = 1.0f;
		}
		BUFFER_SIZE lVertByte = KazBufferHelper::GetBufferSize<BUFFER_SIZE>(lVert.size(), sizeof(Vertex));
		gpuBuffer->TransData(vertexBufferHandle, lVert.data(), lVertByte);
	}
	//�~�̊g�k-------------------------



	//�o�b�t�@�̓]��-----------------------------------------------------------------------------------------------------
	
	if (data.color.Dirty() || data.transform.Dirty() || data.change3DDirtyFlag.Dirty() || (renderData.cameraMgrInstance->ViewDirty() && data.change3DFlag))
	{
		//�s��
		if (data.change3DFlag)
		{
			constMap.world = baseMatWorldData.matWorld;
			constMap.view = renderData.cameraMgrInstance->GetViewMatrix();
			constMap.viewproj = renderData.cameraMgrInstance->GetPerspectiveMatProjection();
			constMap.color = data.color.ConvertColorRateToXMFLOAT4();
			constMap.mat = constMap.world * constMap.view * constMap.viewproj;
		}
		else
		{
			constMap.world = baseMatWorldData.matWorld;
			constMap.view = XMMatrixIdentity();
			constMap.viewproj = renderData.cameraMgrInstance->orthographicMatProjection;
			constMap.color = data.color.ConvertColorRateToXMFLOAT4();
			constMap.mat = constMap.world * constMap.viewproj;
		}
		TransData(&constMap, constBufferHandle, typeid(constMap).name());
	}
	//�o�b�t�@�̓]��-----------------------------------------------------------------------------------------------------


	//�o�b�t�@���R�}���h���X�g�ɐς�-----------------------------------------------------------------------------------------------------
	SetConstBufferOnCmdList(data.pipelineName);
	//�o�b�t�@���R�}���h���X�g�ɐς�-----------------------------------------------------------------------------------------------------

	//�`�施��-----------------------------------------------------------------------------------------------------
	DrawCommand(drawCommandData);
	//�`�施��-----------------------------------------------------------------------------------------------------

	data.Record();

}
