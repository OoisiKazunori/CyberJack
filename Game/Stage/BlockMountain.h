#pragma once
#include"../KazLibrary/Render/KazRender.h"

/// <summary>
/// ‰š“Ê‚Ì‚ ‚é’·•ûŒ`‚ð•`‰æ‚µ‚Ü‚·
/// </summary>
class BlockMountain
{
public:
	BlockMountain();

	void Init();
	void Update();
	void Draw();

private:
	static const int BOX_MAX_NUM = 2000;
	RESOURCE_HANDLE instanceBufferHandle;
	RESOURCE_HANDLE bloomBufferHandle;
	BoxPolygonRenderPtr boxRender;

	struct MatData
	{
		DirectX::XMFLOAT4 color;
		DirectX::XMMATRIX mat;
	};

	struct BloomData
	{
		DirectX::XMFLOAT4 luminanceColor;
		DirectX::XMFLOAT4 fogData;
	};

	std::array<KazMath::Transform3D, BOX_MAX_NUM> boxDataArray;

	BloomData lBloomData;
	KazMath::Vec4<float>fogColor;
	float basePosY;
};

