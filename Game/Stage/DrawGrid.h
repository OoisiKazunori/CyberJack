#pragma once
#include"../KazLibrary/Buffer/CreateGpuBuffer.h"
#include"../Game/Interface/IStage.h"
#include<array>
#include<vector>
#include"../KazLibrary/Render/KazRender.h"
#include"../Game/Event/LineFlashLight.h"

class DrawGrid
{
public:
	DrawGrid();
	void Init(bool USE_FOR_FLOOR_FLAG, float SPACE);
	void Update();
	void Draw();

private:
	//グリッドの描画ーーー
	static const int LINE_MAX = 100;
	static const int LINE_Z_MAX = LINE_MAX / 2;
	static const int LINE_X_MAX = LINE_MAX / 2;

	std::array<LineRender, LINE_MAX> gridLineRender;
	std::array<std::array<KazMath::Transform3D, 2>, LINE_Z_MAX> gridFloorZLinePos;
	std::array<std::array<KazMath::Transform3D, 2>, LINE_X_MAX> gridFloorXLinePos;

	int timer;
	int maxTimer;
	int lightEffectIndex;
	std::array<std::array<KazMath::Vec3<float>, 5>, LINE_MAX> lightEffectGridFloorLinePos;
	std::array<std::array<LineFlashLight, 5>, LINE_MAX> lightEffectArray;
	std::array<bool, LINE_MAX> lightEffectInitFlagArray;
	//グリッドの描画ーーー

	bool reversValueFlag;
	float scaleRate;


	void InitFloor(float SPACE);
	void InitWall(float SPACE);

};

