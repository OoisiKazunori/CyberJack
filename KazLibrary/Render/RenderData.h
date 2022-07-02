#pragma once
#include"../Math/KazMath.h"
#include"../Pipeline/GraphicsPipeLineMgr.h"

struct IData
{
protected:
	void *address;
public:
	string GetName()
	{
		return typeid(address).name();
	}

	int cameraIndex = 0;
};

struct AddTextureData
{
	array<RESOURCE_HANDLE, 10> handle;
	array<GraphicsRootParamType, 10> paramType;

	AddTextureData()
	{
		for (int i = 0; i < handle.size(); ++i)
		{
			handle[i] = -1;
			paramType[i] = GRAPHICS_PRAMTYPE_NONE;
		}
	}
};

struct FlipData
{
	bool x;
	bool y;

	FlipData() :x(false), y(false), xDirtyFlag(&x, false), yDirtyFlag(&y, false)
	{
	}

	void Record()
	{
		xDirtyFlag.Record();
		yDirtyFlag.Record();
	}

	DirtyFlag<bool>xDirtyFlag;
	DirtyFlag<bool>yDirtyFlag;
};

struct ResourceHandle
{
	RESOURCE_HANDLE handle;
	DirtyFlag<RESOURCE_HANDLE>flag;

	ResourceHandle() :handle(-1), flag(&handle)
	{}

	void operator=(RESOURCE_HANDLE HANDLE)
	{
		handle = HANDLE;
	}
	bool operator==(RESOURCE_HANDLE HANDLE)
	{
		return handle == HANDLE;
	}
};


struct Sprite2DData :public IData
{
	KazMath::Transform2D transform;
	ResourceHandle handleData;
	ResourceHandle animationHandle;
	FlipData flip;
	PipeLineNames pipelineName;

	KazMath::Color color;
	AddTextureData addHandle;

	Sprite2DData() :color(0, 0, 0, 255), pipelineName(PIPELINE_NAME_SPRITE)
	{
		address = this;
	}

	void Record()
	{
		transform.Record();
		handleData.flag.Record();
		animationHandle.flag.Record();
		flip.Record();
		color.Record();
	};
};


struct Sprite3DData :public IData
{
	KazMath::Transform3D transform;
	ResourceHandle handleData;
	ResourceHandle animationHandle;
	FlipData flip;
	bool billBoardFlag;
	XMMATRIX motherMat;
	PipeLineNames pipelineName;
	KazMath::Color color;


	Sprite3DData() :pipelineName(PIPELINE_NAME_SPRITE), billBoardDirtyFlag(&billBoardFlag), color(0, 0, 0, 255)
	{
		address = this;
		motherMat = XMMatrixIdentity();
	}

	void Record()
	{
		transform.Record();
		handleData.flag.Record();
		animationHandle.flag.Record();
		billBoardDirtyFlag.Record();
		color.Record();
		flip.Record();
	};

	DirtyFlag<bool>billBoardDirtyFlag;
};

struct Pera3DData :public IData
{
	KazMath::Transform3D transform;
	bool billBoardFlag;
	XMMATRIX motherMat;
	PipeLineNames pipelineName;
	KazMath::Color color;

	DirtyFlag<bool>billBoardDirtyFlag;
	Pera3DData() :pipelineName(PIPELINE_NAME_SPRITE), color(0, 0, 0, 255), billBoardDirtyFlag(&billBoardFlag)
	{
		address = this;
		motherMat = XMMatrixIdentity();
	}
	void Record()
	{
		transform.Record();
		color.Record();
		billBoardDirtyFlag.Record();
	}
};

struct Obj3DData :public IData
{
	KazMath::Transform3D transform;
	ResourceHandle handle;
	KazMath::Vec3<float> upVector;
	KazMath::Vec3<float> frontVector;
	XMMATRIX motherMat;
	PipeLineNames pipelineName;
	KazMath::Color color;
	bool removeMaterialFlag;
	AddTextureData addHandle;

	DirtyFlag<KazMath::Vec3<float>>upVecDirtyFlag;
	DirtyFlag<KazMath::Vec3<float>>frontVecDirtyFlag;


	Obj3DData() :pipelineName(PIPELINE_NAME_OBJ), color(0, 0, 0, 255), motherMat(XMMatrixIdentity()), upVecDirtyFlag(&upVector), frontVecDirtyFlag(&frontVector)
	{
		address = this;
		upVector = { 0,1,0 };
		frontVector = { 0,0,1 };
		removeMaterialFlag = false;
	}

	void Record()
	{
		transform.Record();
		handle.flag.Record();
		color.Record();
		upVecDirtyFlag.Record();
		frontVecDirtyFlag.Record();
	}
};

struct LineDrawData :public IData
{
	KazMath::Vec3<float> startPos;
	KazMath::Vec3<float> endPos;
	KazMath::Color color;
	XMMATRIX motherMat;
	PipeLineNames pipelineName;

	DirtyFlag<KazMath::Vec3<float>> startPosDirtyFlag;
	DirtyFlag<KazMath::Vec3<float>> endPosDirtyFlag;

	LineDrawData() :pipelineName(PIPELINE_NAME_LINE), color(255, 255, 255, 255), startPosDirtyFlag(&startPos), endPosDirtyFlag(&endPos)
	{
		address = this;
		motherMat = XMMatrixIdentity();
	}
	void Record()
	{
		startPosDirtyFlag.Record();
		endPosDirtyFlag.Record();
		color.Record();
	}
};

struct PolygonDrawData :public IData
{
	KazMath::Transform3D transform;
	KazMath::Color color;
	XMMATRIX motherMat;
	PipeLineNames pipelineName;

	PolygonDrawData():color(255, 255, 255, 255), pipelineName(PIPELINE_NAME_COLOR), motherMat(XMMatrixIdentity())
	{
		address = this;
	}

	void Record()
	{
		transform.Record();
		color.Record();
	};

};

struct FbxModelData :public IData
{
	KazMath::Transform3D transform;
	ResourceHandle handle;
	XMMATRIX motherMat;
	bool isPlay;
	int animationNumber;
	PipeLineNames pipelineName;


	FbxModelData()
	{
		address = this;
		pipelineName = PIPELINE_NAME_FBX;
		animationNumber = 0;
		motherMat = XMMatrixIdentity();
	}

	void Record()
	{
		transform.Record();
		handle.flag.Record();
	}
};

struct CircleDrawData :public IData
{
	KazMath::Transform3D transform;
	KazMath::Color color;
	float radius;
	PipeLineNames pipelineName;
	bool change3DFlag;

	CircleDrawData() :color(255, 255, 255, 255), radius(0.0f), pipelineName(PIPELINE_NAME_COLOR_NOCARING), change3DFlag(false), change3DDirtyFlag(&change3DFlag), radiusDirtyFlag(&radius)
	{
		address = this;
	}

	void Record()
	{
		transform.Record();
		color.Record();
		radiusDirtyFlag.Record();
		change3DDirtyFlag.Record();
	};
	DirtyFlag<float> radiusDirtyFlag;
	DirtyFlag<bool> change3DDirtyFlag;
};
