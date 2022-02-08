#pragma once
#include"../DirectXCommon/Base.h"
#include"../Render/IRender.h"
#include"../Math/KazMath.h"
#include"../Helper/DirtyFlag.h"
#include"../Render/RenderData.h"
#include"../Loader/ObjResourceMgr.h"
#include<unordered_map>

class ObjModelRender :public IRender
{
public:
	ObjModelRender(bool INSTANCE_FLAG = false, int INSTANCE_NUM = 1);
	~ObjModelRender();

	void Draw();


	Obj3DData data;
private:

	PipeLineNames pipeline;
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView;
	D3D12_INDEX_BUFFER_VIEW indexBufferView;

	UINT VertByte;
	UINT IndexByte;

	XMMATRIX motherMat;
	short vertexBufferHandle;
	short indexBufferHandle;

	ObjResourceData modelData;
	array<short, 2> constBufferHandle;




	unique_ptr<DirtySet> positionDirtyFlag, scaleDirtyFlag, rotationDirtyFlag;
	unique_ptr<DirtySet> upVectorDirtyFlag;
	unique_ptr<DirtyFlag<short>> objHandleDirtyFlag;

	unique_ptr<DirtySet> cameraViewDirtyFlag;
	unique_ptr<DirtySet> cameraProjectionDirtyFlag;

	bool instanceFlag;
	int instanceNum;


	unordered_map<unsigned short, vector<unsigned short>> smoothData;
	void AddSmoothData(const unsigned short &INDEX_POSITION,const unsigned short &INDEX_VERTEX)
	{
		smoothData[INDEX_POSITION].emplace_back(INDEX_VERTEX);
	};
	void CaluculateSmoothedVertexNormals(vector<Vertex> &DATA)
	{
		auto itr = smoothData.begin();
		for (; itr != smoothData.end(); ++itr)
		{
			vector<unsigned short> &v = itr->second;
			XMVECTOR normal = {};
			for (unsigned short index : v)
			{
				unsigned short tmp = index;
				normal += XMVectorSet(DATA[tmp].normal.x, DATA[tmp].normal.y, DATA[tmp].normal.z, 0);
			}
			normal = XMVector3Normalize(normal / (float)v.size());

			for (unsigned short index : v)
			{
				DATA[index].normal = { normal.m128_f32[0],normal.m128_f32[1],normal.m128_f32[2] };
			}
		}
	}
};

