#include "MeshParticleLoader.h"
#include "../KazLibrary/Loader/FbxModelResourceMgr.h"
MeshParticleLoader::MeshParticleLoader()
{
}

InitMeshParticleData MeshParticleLoader::Load(const std::string &MODEL_NAME, bool REV_FLAG, const DirectX::XMMATRIX *MOTHER_MAT, const MeshParticleLoadData &MESH_PARTICLE_DATA, RESOURCE_HANDLE TEX_HANDLE)
{
	//重複確認
	for (int i = 0; i < handleNameArray.size(); i++)
	{
		if (handleNameArray[i] == MODEL_NAME)
		{
			InitMeshParticleData lMeshData;
			lMeshData.vertData = meshBuffer[i].GetBufferData(CreateMeshBuffer::DATA_VERT);
			lMeshData.uvData = meshBuffer[i].GetBufferData(CreateMeshBuffer::DATA_UV);
			lMeshData.motherMat = MOTHER_MAT;

			RESOURCE_HANDLE lModelHandle = FbxModelResourceMgr::Instance()->LoadModel(MODEL_NAME, REV_FLAG);
			if (FbxModelResourceMgr::Instance()->GetResourceData(lModelHandle)->textureHandle.size() != 0)
			{
				lMeshData.textureHandle = FbxModelResourceMgr::Instance()->GetResourceData(lModelHandle)->textureHandle[0];
			}
			if (TEX_HANDLE != -1)
			{
				lMeshData.textureHandle = TEX_HANDLE;
			}

			lMeshData.triagnleData =
			{
				FbxModelResourceMgr::Instance()->GetResourceData(i)->vertNum,
				MESH_PARTICLE_DATA.bias,
				MESH_PARTICLE_DATA.perTriangleNum,
				MESH_PARTICLE_DATA.faceCountNum
			};


			return lMeshData;
		}
	}

	RESOURCE_HANDLE lModelHandle = FbxModelResourceMgr::Instance()->LoadModel(MODEL_NAME, REV_FLAG);
	if (lModelHandle == -1)
	{
		return InitMeshParticleData();
	}
	int lNowHandle = handle.GetHandle();

	//新しいモデル情報の用意
	handleNameArray.push_back(MODEL_NAME);
	meshBuffer.push_back(CreateMeshBuffer(lModelHandle));

	InitMeshParticleData lMeshData;
	lMeshData.vertData = meshBuffer[lNowHandle].GetBufferData(CreateMeshBuffer::DATA_VERT);
	lMeshData.uvData = meshBuffer[lNowHandle].GetBufferData(CreateMeshBuffer::DATA_UV);
	lMeshData.motherMat = MOTHER_MAT;
	lMeshData.triagnleData =
	{
		FbxModelResourceMgr::Instance()->GetResourceData(lModelHandle)->vertNum,
		MESH_PARTICLE_DATA.bias,
		MESH_PARTICLE_DATA.perTriangleNum,
		MESH_PARTICLE_DATA.faceCountNum
	};


	if (FbxModelResourceMgr::Instance()->GetResourceData(lModelHandle)->textureHandle.size() != 0)
	{
		lMeshData.textureHandle = FbxModelResourceMgr::Instance()->GetResourceData(lModelHandle)->textureHandle[0];
	}
	if (TEX_HANDLE != -1)
	{
		lMeshData.textureHandle = TEX_HANDLE;
	}

	return lMeshData;
}
