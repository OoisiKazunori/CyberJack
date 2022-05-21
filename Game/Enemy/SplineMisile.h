#pragma once
#include"../KazLibrary/Render/KazRender.h"
#include"../Game/Interface/IEnemy.h"

class SplineMisile :public IEnemy
{
public:
	SplineMisile();

	void Init(const XMVECTOR &POS);
	void Finalize();
	void Update();
	void Draw();

private:
	int maxTime;		//合計時間
	int pointTime;		//制御点間を移動する時間
	int nowTime;		//現在の時間

	//スプライン曲線----------------------------------------------
	std::vector<XMVECTOR> points;
	int startIndex;		//どの制御点同士を移動しているか
	float timeRate;		//制御点間の時間
	XMVECTOR position;	//現在地
	BoxPolygonRender splineBox;
	std::array<BoxPolygonRender, 3> pointsRender;
	//スプライン曲線----------------------------------------------

};

