#pragma once
#include"../KazLibrary/Buffer/CreateGpuBuffer.h"
#include"../Game/Interface/IStage.h"
#include<array>
#include<vector>
#include"../Game/Debug/ParameterMgr.h"
#include"../Game/Effect/GalacticParticle.h"
#include"../Game/Effect/TextureParticle.h"
#include"../Game/Effect/SplineParticle.h"

class BlockParticleStage :public IStage
{
public:
	BlockParticleStage();
	~BlockParticleStage();
	void Update()override;
	void Draw()override;

private:
	//バッファ
	std::unique_ptr<CreateGpuBuffer> buffers;
	RESOURCE_HANDLE vertexBufferHandle, indexBufferHandle,
		outputInitBufferHandle, outputBufferHandle,
		particleDataHandle, drawCommandHandle, counterBufferHandle,
		commonInitBufferHandle, commonBufferHandle;
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView;
	D3D12_INDEX_BUFFER_VIEW indexBufferView;
	BufferMemorySize computeMemSize;
	RESOURCE_HANDLE outputInitViewHandle, outputViewHandle, particleDataViewHandle;
	//バッファ

	static const int PARTICLE_MAX_NUM = 4000;
	static const int PER_USE_PARTICLE_MAX_NUM = 20;
	static const int DRAW_CALL = 1;

	struct IndirectCommand
	{
		D3D12_GPU_VIRTUAL_ADDRESS uav;
		D3D12_DRAW_INDEXED_ARGUMENTS drawArguments;
	};

	struct ParticleData
	{
		DirectX::XMFLOAT4 pos;
	};

	struct OutputInitData
	{
		DirectX::XMFLOAT4 pos;
		DirectX::XMFLOAT4 color;
	};

	struct OutputData
	{
		DirectX::XMMATRIX mat;
		DirectX::XMFLOAT4 color;
	};

	struct CommonData
	{
		DirectX::XMMATRIX cameraMat;
		DirectX::XMMATRIX projectionMat;
		DirectX::XMMATRIX billboardMat;
		DirectX::XMFLOAT4 vertices[8];
	};


	struct CommonMoveData
	{
		DirectX::XMMATRIX viewProjectionMat;
		DirectX::XMMATRIX scaleRotateBillboardMat;
		DirectX::XMFLOAT2 flash;
	};

	Microsoft::WRL::ComPtr<ID3D12CommandSignature> commandSig;
	CommonMoveData constBufferData;
	int num;

	ParameterMgr blockFileMgr;


	GalacticParticle galacticParticle;

	bool highFlag, prepareFlag;
	int prepareTimer;

	int flashTimer;

	static const int FLOOR_PARTICLE_MAX_NUM = 20;
	std::array<std::unique_ptr<TextureParticle>, FLOOR_PARTICLE_MAX_NUM> floorParticleModel;
	std::array<KazMath::Transform3D, FLOOR_PARTICLE_MAX_NUM> floorParticleTransform;
	std::array<DirectX::XMMATRIX, FLOOR_PARTICLE_MAX_NUM> floorParticleMotherMat;

	static const int PILLAR_PARTICLE_MAX_NUM = 20;
	static const float PILLAR_PARTICLE_INTERVAL_NUM;
	std::array<std::unique_ptr<TextureParticle>, PILLAR_PARTICLE_MAX_NUM> pillarParticleModel;
	std::array<KazMath::Transform3D, PILLAR_PARTICLE_MAX_NUM> pillarParticleTransform;
	std::array<DirectX::XMMATRIX, PILLAR_PARTICLE_MAX_NUM> pillarParticleMotherMat;
	RESOURCE_HANDLE pillarHandle;

	KazMath::Vec2<float>flash;
	
	std::array<std::unique_ptr<SplineParticle>, 2> splineParticle;

	RESOURCE_HANDLE floorResourceHandle;
	KazMath::Vec3<float>v;




	std::vector<VertexUv>GetPlaneData(RESOURCE_HANDLE HANDLE)
	{
		std::array<Vertex, 4>vertices;
		std::array<USHORT, 6> indices;
		indices = KazRenderHelper::InitIndciesForPlanePolygon();
		KazRenderHelper::InitVerticesPos(&vertices[0].pos, &vertices[1].pos, &vertices[2].pos, &vertices[3].pos, { 0.5f,0.5f });
		KazRenderHelper::InitUvPos(&vertices[0].uv, &vertices[1].uv, &vertices[2].uv, &vertices[3].uv);

		KazMath::Vec2<int>lTexSize
		(
			static_cast<int>(TextureResourceMgr::Instance()->GetTextureSize(HANDLE).Width),
			static_cast<int>(TextureResourceMgr::Instance()->GetTextureSize(HANDLE).Height)
		);

		KazMath::Vec2<float> lLeftUp, lRightDown;
		lLeftUp = { 0.0f,0.0f };
		lRightDown = { 1.0f,1.0f };
		//サイズ変更
		std::array<KazMath::Vec2<float>, 4>lVert;
		lVert = KazRenderHelper::ChangePlaneScale(lLeftUp, lRightDown, { 1.0f,1.0f }, { 0.5f,0.5f }, lTexSize);


		std::array<int, 6>indiData;
		indiData[0] = 0;
		indiData[1] = 1;
		indiData[2] = 2;
		indiData[3] = 1;
		indiData[4] = 3;
		indiData[5] = 2;


		std::vector<VertexUv>lVertArray;

		for (int i = 0; i < indices.size(); ++i)
		{
			int index = indiData[i];
			VertexUv lData;
			lData.pos = { lVert[index].ConvertXMFLOAT2().x,lVert[index].ConvertXMFLOAT2().y,0.0f };
			lData.uv = vertices[index].uv;
			lVertArray.push_back(lData);
		}

		lVertArray[0].uv = { 1.0f,0.0f };
		lVertArray[1].uv = { 0.0f,0.0f };
		lVertArray[2].uv = { 0.0f,1.0f };
		lVertArray[3].uv = { 1.0f,0.0f };
		lVertArray[4].uv = { 0.0f,1.0f };
		lVertArray[5].uv = { 1.0f,1.0f };

		return lVertArray;
	}

};

