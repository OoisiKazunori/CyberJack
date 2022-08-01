#pragma once
#include"../KazLibrary/Buffer/CreateGpuBuffer.h"
#include"../Game/Interface/IStage.h"
#include<array>
#include<vector>
#include"../KazLibrary/Render/KazRender.h"
#include"../Game/Event/LineFlashLight.h"

class RezStage :public IStage
{
public:
	RezStage();
	void Update()override;
	void Draw()override;

private:
	//グリッドの描画ーーー
	static const int LINE_MAX = 100;
	static const int LINE_Z_MAX = LINE_MAX / 2;
	static const int LINE_X_MAX = LINE_MAX / 2;

	std::array<LineRender, LINE_MAX> gridLineRender;
	std::array<std::array<KazMath::Transform3D, 2>, LINE_Z_MAX> gridFloorZLinePos;
	std::array<std::array<KazMath::Transform3D, 2>, LINE_X_MAX> gridFloorXLinePos;
	//グリッドの描画ーーー


	//檻の描画ーーー
	static const int WALL_LINE_MAX = 100;
	static const int WALL_LINE_TOP_MAX = LINE_MAX / 3;
	static const int WALL_LINE_LEFT_MAX = LINE_MAX / 3;
	static const int WALL_LINE_RIGHT_MAX = LINE_MAX / 3;

	std::array<std::array<KazMath::Transform3D, 2>, WALL_LINE_TOP_MAX> wallTopLinePos;
	std::array<std::array<KazMath::Transform3D, 2>, WALL_LINE_LEFT_MAX> wallLeftLinePos;
	std::array<std::array<KazMath::Transform3D, 2>, WALL_LINE_RIGHT_MAX> wallRightLinePos;
	std::array<LineRender, WALL_LINE_MAX> gridWallLineRender;

	std::array<std::array<KazMath::Transform3D, 2>, 10> zWallTopLinePos;
	std::array<std::array<KazMath::Transform3D, 2>, 10> zWallLeftLinePos;
	std::array<std::array<KazMath::Transform3D, 2>, 10> zWallRightLinePos;
	std::array<LineRender, 10 * 3> zGridWallLineRender;
	//檻の描画ーーー



	//真ん中線の描画ーーー
	std::array<LineRender, 4> gridCentralLineRender;
	std::array<std::array<LineRender, 4>, 20> gridCentralWallLineRender;
	//真ん中線の描画ーーー

	ObjModelRender model;

	//床にOBJ配置
	struct FloorObjData
	{
		std::array<BoxPolygonRender, 2> objRender;
		KazMath::Vec3<float>initScale;
	};
	std::array<FloorObjData, 50>floorObjectRender;

	float scaleRate;
	bool reversValueFlag;


	Sprite3DRender vaporWaveSunRender;
	LineFlashLight lineEffect;
	int timer;
	int maxTimer;
};