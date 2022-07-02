#include "BoxPolygonRender.h"

BoxPolygonRender::BoxPolygonRender(bool INSTANCE_FLAG, int INSTANCE_NUM)
{
	gpuBuffer = std::make_unique<CreateGpuBuffer>();

	int a = INSTANCE_NUM;
	a = 0;
	const float lSize = 1.0f;

	Vertex lVertices[] =
	{
		//x,y,z	�@��	u,v
	//�O
	{{-lSize, -lSize, -lSize},{}, {0.0f,1.0f}},		//����
	{{-lSize,  lSize, -lSize},{}, {0.0f,0.0f}},		//����
	{{ lSize, -lSize, -lSize}, {}, {1.0f,1.0f}},		//�E��
	{{ lSize,  lSize, -lSize}, {}, {1.0f,0.0f}},		//�E��

	//��
	{{-lSize,-lSize,  lSize}, {}, {0.0f,1.0f}},		//����
	{{-lSize, lSize,  lSize}, {},{0.0f,0.0f}},		//����
	{{ lSize,-lSize,  lSize},{},{1.0f,1.0f}},		//�E��
	{{ lSize, lSize,  lSize},{}, {1.0f,0.0f}},		//�E��

	//��
	{{-lSize,-lSize, -lSize},{}, {0.0f,1.0f}},		//����
	{{-lSize,-lSize,  lSize}, {}, {0.0f,0.0f}},		//����
	{{-lSize, lSize, -lSize}, {}, {1.0f,1.0f}},		//�E��
	{{-lSize, lSize,  lSize}, {}, {1.0f,0.0f}},		//�E��

	//�E
	{{lSize,-lSize, -lSize},{}, {0.0f,1.0f}},		//����
	{{lSize,-lSize,  lSize}, {}, {0.0f,0.0f}},		//����
	{{lSize, lSize, -lSize}, {}, {1.0f,1.0f}},		//�E��
	{{lSize, lSize,  lSize}, {}, {1.0f,0.0f}},		//�E��

	//��
	{{ lSize, -lSize, lSize}, {}, {0.0f,1.0f}},		//����
	{{ lSize, -lSize,-lSize}, {}, {0.0f,0.0f}},		//����
	{{-lSize, -lSize, lSize}, {}, {1.0f,1.0f}},		//�E��
	{{-lSize, -lSize,-lSize}, {}, {1.0f,0.0f}},	//�E��

	//��
	{{ lSize, lSize, lSize}, {}, {0.0f,1.0f}},			//����
	{{ lSize, lSize,-lSize}, {}, {0.0f,0.0f}},			//����
	{{-lSize, lSize, lSize}, {}, {1.0f,1.0f}},			//�E��
	{{-lSize, lSize,-lSize}, {}, {1.0f,0.0f}}			//�E��
	};

	const unsigned short lIndices[] =
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

	UINT lIndexLEN = sizeof(lIndices) / sizeof(lIndices[0]);

	UINT lVertByte = KazBufferHelper::GetBufferSize<BUFFER_SIZE>(sizeof(lVertices) / sizeof(lVertices[0]), sizeof(Vertex));
	UINT lIndexByte = KazBufferHelper::GetBufferSize<BUFFER_SIZE>(lIndexLEN, sizeof(Vertex));

	//�@���x�N�g��
	for (unsigned int i = 0; i < lIndexLEN / 3; i++)
	{
		USHORT index[3];

		//�O�p�`�̃C���f�b�N�X�����o���āA�ꎞ�I�ȕϐ��ɓ����
		index[0] = lIndices[i * 3 + 0];
		index[1] = lIndices[i * 3 + 1];
		index[2] = lIndices[i * 3 + 2];

		//�O�p�`���\�z���钸�_���W���x�N�g���ɑ��
		XMVECTOR p0 = XMLoadFloat3(&lVertices[index[0]].pos);
		XMVECTOR p1 = XMLoadFloat3(&lVertices[index[1]].pos);
		XMVECTOR p2 = XMLoadFloat3(&lVertices[index[2]].pos);

		//p0-p1,p0-p2�x�N�g�����v�Z(�x�N�g���̌��Z)
		XMVECTOR v1 = XMVectorSubtract(p1, p0);
		XMVECTOR v2 = XMVectorSubtract(p2, p0);

		//�O�ς͗������琂���ȃx�N�g��
		XMVECTOR normal = XMVector3Cross(v1, v2);

		//���K��(������1�ɂ���)
		normal = XMVector3Normalize(normal);

		//���߂��@���𒸓_�f�[�^����
		XMStoreFloat3(&lVertices[index[0]].normal, normal);
		XMStoreFloat3(&lVertices[index[1]].normal, normal);
		XMStoreFloat3(&lVertices[index[2]].normal, normal);
	}


	RESOURCE_HANDLE lVertBufferHandle = gpuBuffer->CreateBuffer(KazBufferHelper::SetVertexBufferData(lVertByte));
	RESOURCE_HANDLE lIndexBufferHandle = gpuBuffer->CreateBuffer(KazBufferHelper::SetIndexBufferData(lIndexByte));


	instanceFlag = INSTANCE_FLAG;
	if (!instanceFlag)
	{
		//�萔�o�b�t�@
		constBufferHandle = CreateConstBuffer(sizeof(ConstBufferData), typeid(ConstBufferData).name(), GRAPHICS_RANGE_TYPE_CBV, GRAPHICS_PRAMTYPE_DRAW);
	}


	array<Vertex, 24> vertVec;
	array<unsigned short, 36> indiVec;
	for (int i = 0; i < 24; i++)
	{
		vertVec[i].pos = lVertices[i].pos;
		vertVec[i].uv = lVertices[i].uv;
		vertVec[i].normal = lVertices[i].normal;
	}
	for (int i = 0; i < 36; i++)
	{
		indiVec[i] = lIndices[i];
	}

	gpuBuffer->TransData(lVertBufferHandle, vertVec.data(), lVertByte);
	gpuBuffer->TransData(lIndexBufferHandle, indiVec.data(), lIndexByte);


	drawCommandData = KazRenderHelper::SetDrawCommandData(
		D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
		KazBufferHelper::SetVertexBufferView(gpuBuffer->GetGpuAddress(lVertBufferHandle), lVertByte, sizeof(lVertices[0])),
		KazBufferHelper::SetIndexBufferView(gpuBuffer->GetGpuAddress(lIndexBufferHandle), lIndexByte),
		lIndexLEN,
		INSTANCE_NUM
	);
}

BoxPolygonRender::~BoxPolygonRender()
{
}

void BoxPolygonRender::Draw()
{
	//�p�C�v���C���ݒ�-----------------------------------------------------------------------------------------------------
	renderData.pipelineMgr->SetPipeLineAndRootSignature(data.pipelineName);
	//�p�C�v���C���ݒ�-----------------------------------------------------------------------------------------------------


	if (!instanceFlag)
	{
		//�s��v�Z-----------------------------------------------------------------------------------------------------
		if (data.transform.Dirty())
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
		if (data.transform.Dirty() || renderData.cameraMgrInstance->ViewDirty() || data.color.Dirty())
		{
			ConstBufferData constMap;
			constMap.world = baseMatWorldData.matWorld;
			constMap.view = renderData.cameraMgrInstance->GetViewMatrix(data.cameraIndex);
			constMap.viewproj = renderData.cameraMgrInstance->GetPerspectiveMatProjection();
			constMap.color = data.color.ConvertColorRateToXMFLOAT4();
			constMap.mat = constMap.world * constMap.view * constMap.viewproj;
			TransData(&constMap, constBufferHandle, typeid(ConstBufferData).name());
		}
		//�o�b�t�@�̓]��-----------------------------------------------------------------------------------------------------
	}

	//�o�b�t�@���R�}���h���X�g�ɐς�-----------------------------------------------------------------------------------------------------
	SetConstBufferOnCmdList(data.pipelineName);
	//�o�b�t�@���R�}���h���X�g�ɐς�-----------------------------------------------------------------------------------------------------

	//�`�施��-------------------------
	DrawCommand(drawCommandData);
	//�`�施��-------------------------

	data.Record();
}

XMMATRIX BoxPolygonRender::GetMotherMatrix()
{
	return motherMatrix;
}
