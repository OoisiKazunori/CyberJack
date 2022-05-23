#include "BoxPolygonRender.h"

BoxPolygonRender::BoxPolygonRender(bool INSTANCE_FLAG, int INSTANCE_NUM)
{
	gpuBuffer.reset(new CreateGpuBuffer);
	positionDirtyFlag.reset(new DirtySet(data.transform.pos));
	scaleDirtyFlag.reset(new DirtySet(data.transform.scale));
	rotationDirtyFlag.reset(new DirtySet(data.transform.rotation));
	cameraViewDirtyFlag.reset(new DirtySet(renderData.cameraMgrInstance->view));
	cameraProjectionDirtyFlag.reset(new DirtySet(renderData.cameraMgrInstance->perspectiveMat));
	cameraBillBoardDirtyFlag.reset(new DirtySet(renderData.cameraMgrInstance->billBoard));

	colorDirtyFlag.reset(new DirtySet(data.color));

	float size = 1.0f;

	Vertex vertices[] =
	{
		//x,y,z	�@��	u,v
	//�O
	{{-size, -size, -size},{}, {0.0f,1.0f}},		//����
	{{-size,  size, -size},{}, {0.0f,0.0f}},		//����
	{{ size, -size, -size}, {}, {1.0f,1.0f}},		//�E��
	{{ size,  size, -size}, {}, {1.0f,0.0f}},		//�E��

	//��
	{{-size,-size,  size}, {}, {0.0f,1.0f}},		//����
	{{-size, size,  size}, {},{0.0f,0.0f}},		//����
	{{ size,-size,  size},{},{1.0f,1.0f}},		//�E��
	{{ size, size,  size},{}, {1.0f,0.0f}},		//�E��

	//��
	{{-size,-size, -size},{}, {0.0f,1.0f}},		//����
	{{-size,-size,  size}, {}, {0.0f,0.0f}},		//����
	{{-size, size, -size}, {}, {1.0f,1.0f}},		//�E��
	{{-size, size,  size}, {}, {1.0f,0.0f}},		//�E��

	//�E
	{{size,-size, -size},{}, {0.0f,1.0f}},		//����
	{{size,-size,  size}, {}, {0.0f,0.0f}},		//����
	{{size, size, -size}, {}, {1.0f,1.0f}},		//�E��
	{{size, size,  size}, {}, {1.0f,0.0f}},		//�E��

	//��
	{{ size, -size, size}, {}, {0.0f,1.0f}},		//����
	{{ size, -size,-size}, {}, {0.0f,0.0f}},		//����
	{{-size, -size, size}, {}, {1.0f,1.0f}},		//�E��
	{{-size, -size,-size}, {}, {1.0f,0.0f}},	//�E��

	//��
	{{ size, size, size}, {}, {0.0f,1.0f}},			//����
	{{ size, size,-size}, {}, {0.0f,0.0f}},			//����
	{{-size, size, size}, {}, {1.0f,1.0f}},			//�E��
	{{-size, size,-size}, {}, {1.0f,0.0f}}			//�E��
	};

	unsigned short indices[] =
	{
		//�O
		0,1,2,		//�O�p�`1��
		2,1,3,		//�O�p�`2��
		//��
		4,6,5,		//�O�p�`3��
		6,7,5,		//�O�p�`4��
		//��
		8,9,10,	//�O�p�`1��
		10,9,11,	//�O�p�`2��
		//�E
		12,14,13,	//�O�p�`1��
		13,14,15,//�O�p�`2��
		//��
		16,18,17,	//�O�p�`1��
		17,18,19,	//�O�p�`2��
		//��
		20,21,22,	//�O�p�`3��
		22,21,23	//�O�p�`4��
	};

	UINT VertLEN = sizeof(vertices) / sizeof(vertices[0]);
	UINT IndexLEN = sizeof(indices) / sizeof(indices[0]);
	UINT VertByte = VertLEN * sizeof(Vertex);
	UINT IndexByte = IndexLEN * sizeof(USHORT);


	//indiNum = indices.size();
	indicesNum = IndexLEN;


	//�@���x�N�g��
	for (int i = 0; i < IndexLEN / 3; i++)
	{
		USHORT index[3];

		//�O�p�`�̃C���f�b�N�X�����o���āA�ꎞ�I�ȕϐ��ɓ����
		index[0] = indices[i * 3 + 0];
		index[1] = indices[i * 3 + 1];
		index[2] = indices[i * 3 + 2];

		//�O�p�`���\�z���钸�_���W���x�N�g���ɑ��
		XMVECTOR p0 = XMLoadFloat3(&vertices[index[0]].pos);
		XMVECTOR p1 = XMLoadFloat3(&vertices[index[1]].pos);
		XMVECTOR p2 = XMLoadFloat3(&vertices[index[2]].pos);

		//p0-p1,p0-p2�x�N�g�����v�Z(�x�N�g���̌��Z)
		XMVECTOR v1 = XMVectorSubtract(p1, p0);
		XMVECTOR v2 = XMVectorSubtract(p2, p0);

		//�O�ς͗������琂���ȃx�N�g��
		XMVECTOR normal = XMVector3Cross(v1, v2);

		//���K��(������1�ɂ���)
		normal = XMVector3Normalize(normal);

		//���߂��@���𒸓_�f�[�^����
		XMStoreFloat3(&vertices[index[0]].normal, normal);
		XMStoreFloat3(&vertices[index[1]].normal, normal);
		XMStoreFloat3(&vertices[index[2]].normal, normal);
	}


	short vertBufferHandle = gpuBuffer->CreateBuffer(KazBufferHelper::SetVertexBufferData(VertByte));
	short indexBufferHandle = gpuBuffer->CreateBuffer(KazBufferHelper::SetIndexBufferData(IndexByte));


	instanceFlag = INSTANCE_FLAG;
	if (!instanceFlag)
	{
		//�萔�o�b�t�@
		constBufferHandle = CreateConstBuffer(sizeof(ConstBufferData), typeid(ConstBufferData).name(), GRAPHICS_RANGE_TYPE_CBV, GRAPHICS_PRAMTYPE_DRAW);
	}
	instanceNum = INSTANCE_NUM;


	array<Vertex, 24> vertVec;
	array<unsigned short, 36> indiVec;
	for (int i = 0; i < 24; i++)
	{
		vertVec[i].pos = vertices[i].pos;
		vertVec[i].uv = vertices[i].uv;
		vertVec[i].normal = vertices[i].normal;
	}
	for (int i = 0; i < 36; i++)
	{
		indiVec[i] = indices[i];
	}

	gpuBuffer->TransData(vertBufferHandle, vertVec.data(), VertByte);
	gpuBuffer->TransData(indexBufferHandle, indiVec.data(), IndexByte);

	vertBufferView = KazBufferHelper::SetVertexBufferView(gpuBuffer->GetGpuAddress(vertBufferHandle), VertByte, sizeof(vertices[0]));
	indexBufferView = KazBufferHelper::SetIndexBufferView(gpuBuffer->GetGpuAddress(indexBufferHandle), IndexByte);

}

BoxPolygonRender::~BoxPolygonRender()
{
}

void BoxPolygonRender::Draw()
{
	//�p�C�v���C���ݒ�-----------------------------------------------------------------------------------------------------
	pipeline = static_cast<PipeLineNames>(data.pipelineName);
	renderData.pipelineMgr->SetPipeLineAndRootSignature(pipeline);
	//�p�C�v���C���ݒ�-----------------------------------------------------------------------------------------------------




	//DirtyFlag���m-----------------------------------------------------------------------------------------------------	
	bool matFlag = cameraViewDirtyFlag->FloatDirty() || cameraProjectionDirtyFlag->FloatDirty() || cameraBillBoardDirtyFlag->FloatDirty();
	bool matrixDirtyFlag = positionDirtyFlag->FloatDirty() || scaleDirtyFlag->FloatDirty() || rotationDirtyFlag->FloatDirty();
	bool colorFlag = colorDirtyFlag->FloatDirty();
	//DirtyFlag���m-----------------------------------------------------------------------------------------------------


	if (!instanceFlag)
	{
		//�s��v�Z-----------------------------------------------------------------------------------------------------
		if (matrixDirtyFlag || matFlag || true)
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
			baseMatWorldData.matWorld *= data.motherMat;

			//�e�s����|����
			motherMatrix = baseMatWorldData.matWorld;
		}
		//�s��v�Z-----------------------------------------------------------------------------------------------------



		//�o�b�t�@�̓]��-----------------------------------------------------------------------------------------------------
		//�s��
		if (matrixDirtyFlag || matFlag || colorFlag | true)
		{
			ConstBufferData constMap;
			constMap.world = baseMatWorldData.matWorld;
			constMap.view = renderData.cameraMgrInstance->GetViewMatrix(data.cameraIndex);
			constMap.viewproj = renderData.cameraMgrInstance->GetPerspectiveMatProjection();
			constMap.color = KazRenderHelper::SendColorDataToGPU(data.color);
			constMap.mat = constMap.world * constMap.view * constMap.viewproj;
			TransData(&constMap, constBufferHandle, typeid(ConstBufferData).name());
		}
		//�o�b�t�@�̓]��-----------------------------------------------------------------------------------------------------
	}

	//�o�b�t�@���R�}���h���X�g�ɐς�-----------------------------------------------------------------------------------------------------
	SetConstBufferOnCmdList(pipeline);
	//�o�b�t�@���R�}���h���X�g�ɐς�-----------------------------------------------------------------------------------------------------

	//�`�施��-----------------------------------------------------------------------------------------------------
	renderData.cmdListInstance->cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	renderData.cmdListInstance->cmdList->IASetVertexBuffers(0, 1, &vertBufferView);
	renderData.cmdListInstance->cmdList->IASetIndexBuffer(&indexBufferView);
	renderData.cmdListInstance->cmdList->DrawIndexedInstanced(indicesNum, instanceNum, 0, 0, 0);
	//�`�施��-----------------------------------------------------------------------------------------------------



	//DirtyFlag�̍X�V-----------------------------------------------------------------------------------------------------
	positionDirtyFlag->Record();
	scaleDirtyFlag->Record();
	rotationDirtyFlag->Record();

	cameraBillBoardDirtyFlag->Record();
	cameraProjectionDirtyFlag->Record();
	cameraViewDirtyFlag->Record();
	colorDirtyFlag->Record();
	//DirtyFlag�̍X�V-----------------------------------------------------------------------------------------------------
}

XMMATRIX BoxPolygonRender::GetMotherMatrix()
{
	return motherMatrix;
}
