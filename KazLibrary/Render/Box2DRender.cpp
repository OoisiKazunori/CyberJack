#include "Box2DRender.h"

Box2DRender::Box2DRender()
{
	gpuBuffer = std::make_unique<CreateGpuBuffer>();

	vertByte = KazBufferHelper::GetBufferSize<BUFFER_SIZE>(vertices.size(), sizeof(SpriteVertex));
	//���_�o�b�t�@
	vertexBufferHandle = gpuBuffer->CreateBuffer
	(
		KazBufferHelper::SetVertexBufferData(vertByte)
	);
	constBufferHandle = CreateConstBuffer(sizeof(ConstBufferData), typeid(ConstBufferData).name(), GRAPHICS_RANGE_TYPE_CBV, GRAPHICS_PRAMTYPE_DRAW);

	drawInstanceCommandData =
		KazRenderHelper::SetDrawInstanceCommandData(D3D_PRIMITIVE_TOPOLOGY_LINELIST,
			KazBufferHelper::SetVertexBufferView(vertByte, vertByte, sizeof(vertices[0])),
			vertices.size(),
			1
		);
}

void Box2DRender::Draw()
{
	//�p�C�v���C���ݒ�-----------------------------------------------------------------------------------------------------
	renderData.pipelineMgr->SetPipeLineAndRootSignature(data.pipelineName);
	//�p�C�v���C���ݒ�-----------------------------------------------------------------------------------------------------

	//�ʒu�ύX-------------------------
	if (data.leftUpPosDirtyFlag.Dirty() || data.rightDownPosDirtyFlag.Dirty())
	{
		vertices[LEFT_UP].pos = KazMath::Vec3<float>(data.leftUpPos, 0.0f).ConvertXMFLOAT3();
		vertices[LEFT_DOWN].pos = KazMath::Vec3<float>(data.leftUpPos.x, data.rightDownPos.y, 0.0f).ConvertXMFLOAT3();
		vertices[RIGHT_UP].pos = KazMath::Vec3<float>(data.rightDownPos.x, data.leftUpPos.y, 0.0f).ConvertXMFLOAT3();
		vertices[RIGHT_DOWN].pos = KazMath::Vec3<float>(data.rightDownPos, 0.0f).ConvertXMFLOAT3();

		gpuBuffer->TransData(vertexBufferHandle, vertices.data(), vertByte);
	}
	//�ʒu�ύX-------------------------



	//�o�b�t�@�̓]��-----------------------------------------------------------------------------------------------------
	if (data.color.Dirty() || data.leftUpPosDirtyFlag.Dirty() || data.rightDownPosDirtyFlag.Dirty())
	{
		constMap.world = baseMatWorldData.matWorld;
		constMap.view = DirectX::XMMatrixIdentity();
		constMap.viewproj = renderData.cameraMgrInstance->GetOrthographicMatProjection();
		constMap.color = data.color.ConvertColorRateToXMFLOAT4();
		constMap.mat = constMap.world * constMap.viewproj;
		TransData(&constMap, constBufferHandle, typeid(constMap).name());
	}
	//�o�b�t�@�̓]��-----------------------------------------------------------------------------------------------------


	//�o�b�t�@���R�}���h���X�g�ɐς�-----------------------------------------------------------------------------------------------------
	SetConstBufferOnCmdList(data.pipelineName);
	//�o�b�t�@���R�}���h���X�g�ɐς�-----------------------------------------------------------------------------------------------------

	//�`�施��-----------------------------------------------------------------------------------------------------
	DrawIndexInstanceCommand(drawIndexInstanceCommandData);
	//�`�施��-----------------------------------------------------------------------------------------------------

	data.Record();
}
