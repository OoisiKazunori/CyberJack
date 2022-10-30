#pragma once
#include"../KazLibrary/Render/KazRender.h"

struct InitBlockMountainData
{
	KazMath::Vec3<float> centralPos;
	KazMath::Vec3<float> lightDir;
	float yScale;
	int xRange;
	float fogDesinty;

};

/// <summary>
/// ‰š“Ê‚Ì‚ ‚é’·•ûŒ`‚ð•`‰æ‚µ‚Ü‚·
/// </summary>
class BlockMountain
{
public:
	BlockMountain(const InitBlockMountainData &INIT_DATA);
	void Update();
	void Draw();

private:
	int boxMaxNum;
	RESOURCE_HANDLE instanceBufferHandle;
	RESOURCE_HANDLE objectBufferHandle;
	BoxPolygonRenderPtr boxRender;

	struct MatData
	{
		DirectX::XMFLOAT4 color;
		DirectX::XMMATRIX mat;
	};

	struct ObjectData
	{
		DirectX::XMFLOAT4 luminanceColor;
		DirectX::XMFLOAT4 fogData;
		DirectX::XMFLOAT3 lightDir;
	};

	std::vector<KazMath::Transform3D> boxDataArray;

	ObjectData objectData;
	KazMath::Vec4<float>fogColor;
};

