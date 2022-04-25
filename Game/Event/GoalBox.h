#pragma once
#include"../KazLibrary/DirectXCommon/Base.h"
#include"../Game/Interface/IOperationObject.h"
#include"../KazLibrary/Render/KazRender.h"
#include"../Game/Event/GoalBoxEffect.h"
#include<array>

class GoalBox :public IOperationObject
{
public:
	GoalBox();

	void Init();
	void Update();
	void Draw();

	//ゴールエフェクト
	std::array<GoalBoxEffect, 2> effect;
private:
	//移動-----------------------
	XMVECTOR lerpRota;//角度のラープ
	XMVECTOR lerpPos; //座標のラープ
	XMVECTOR moveVel; //当たった際にどれくらい移動するか
	XMVECTOR moveRotaVel;//当たった際にどれくらいまわるか
	float lerpMoveVel;		//ラープの速度
	float lerpMoveRotaVel;	//ラープの速度

	int prevHpNum;//前フレームの体力
	//描画-----------------------
	ObjModelRenderPtr model;



};