#pragma once
#include"../KazLibrary/Render/KazRender.h"
#include"../KazLibrary/Collision/CollisionManager.h"

/// <summary>
/// 3D��ԏ�̃u���b�N��z�u����c�[��
/// </summary>
class MapChipPlacementTool
{
	static const int BLOCK_MAX_NUM = 6000;
	static const KazMath::Vec3<float> MAP_SIZE;
	typedef std::array<KazMath::Vec3<float>, 100> Z_ARRAY;
	typedef std::array<Z_ARRAY, 30> Y_ARRAY;
	typedef std::array<Y_ARRAY, 100> POS_ARRAY;

public:
	MapChipPlacementTool();
	void Init();
	void Input(bool PUT_FLAG, bool RELEASE_FLAG, const DirectX::XMFLOAT2 &MOUSE_POS);
	void Update();
	void Draw();

	const POS_ARRAY &GetPosArray()
	{
		return blockPosArray;
	};

private:
	//�u���b�N�̍��W�Ǘ� X100 Y30 Z100 �̌Œ蒷�z��
	POS_ARRAY blockPosArray;

	static const int xSize = 100;
	static const int ySize = 200;
	static const int matSize = xSize * ySize;
	std::array<std::array<KazMath::Transform3D, xSize>, ySize> defaultBlockPosArray;
	const float BLOCK_SIZE;
	Ray ray;

	//UI
	BoxPolygonRender selectR;//������I�����Ă��邩
	KazMath::Vec3<int> selectingIndex;
	KazMath::Vec3<int> putIndex;
	KazMath::Vec3<float> blockPutInOnceSize;

	//����
	enum MESH_DIR
	{
		NONE = -1,
		TOP,
		BUTTOM,
		FRONT,
		BACK,
		LEFT,
		RIGHT,
	};
	MESH_DIR CheckBlock(const KazMath::Vec3<float> &POS);
	bool inputFlag;
	bool releaseFlag;

	KazMath::Vec3<int> AdjIndex(MESH_DIR DIR)
	{
		switch (DIR)
		{
		case MapChipPlacementTool::TOP:
			return KazMath::Vec3<int>(0, 1, 0);
			break;
		case MapChipPlacementTool::BUTTOM:
			return KazMath::Vec3<int>(0, -1, 0);
			break;
		case MapChipPlacementTool::FRONT:
			return KazMath::Vec3<int>(0, 0, -1);
			break;
		case MapChipPlacementTool::BACK:
			return KazMath::Vec3<int>(0, 0, 1);
			break;
		case MapChipPlacementTool::LEFT:
			return KazMath::Vec3<int>(-1, 0, 0);
			break;
		case MapChipPlacementTool::RIGHT:
			return KazMath::Vec3<int>(1, 0, 0);
			break;
		default:
			break;
		}
		return KazMath::Vec3<int>(0, 0, 0);
	}



	struct MatData
	{
		DirectX::XMFLOAT4 color;
		DirectX::XMMATRIX mat;
	};
	BoxPolygonRenderPtr instanceFlameRender;
	BoxPolygonRenderPtr instanceBoxRender;
	RESOURCE_HANDLE instanceHandle;
	RESOURCE_HANDLE instanceBoxHandle;

	
	std::array<MatData, matSize>lMatData;
	std::array<MatData, BLOCK_MAX_NUM>blockMatData;
	int matIndex = 0;

	Sprite2DRender cursorR;

	const float REV_VALUE = -10000.0f;
};