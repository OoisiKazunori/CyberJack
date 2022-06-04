#pragma once
#include"../KazLibrary/DirectXCommon/Base.h"
#include"../Game/Interface/IOperationObject.h"
#include"../KazLibrary/Render/KazRender.h"
#include"../Game/Event/GoalBoxEffect.h"
#include"../Game/Event/GoalBoxPortalEffect.h"
#include<array>

class GoalBox :public IOperationObject
{
public:
	GoalBox();

	void Init(const XMVECTOR &POS);
	void Update();
	void Draw();
	
	void Appear(const XMVECTOR &POS);

	//ゴールエフェクト
	GoalBoxEffect lightEffect;
	GoalBoxPortalEffect portalEffect;
	Sphere hitBox;
	bool releaseFlag;
	bool startPortalEffectFlag;

private:
	static const int HP = 8;

	//移動-----------------------
	XMVECTOR lerpRota;//角度のラープ
	XMVECTOR lerpScale;//角度のラープ
	XMVECTOR lerpPos; //座標のラープ
	XMVECTOR moveVel; //当たった際にどれくらい移動するか
	XMVECTOR moveRotaVel;//当たった際にどれくらいまわるか
	float lerpMoveVel;		//ラープの速度
	float lerpMoveRotaVel;	//ラープの速度

	XMVECTOR goCenterPos;//全弾ヒット時にゴール地点に向かう


	int prevHpNum;//前フレームの体力
	//描画-----------------------
	ObjModelRenderPtr model;

	float addRota;
	float addVel;
	bool hitFlag;

	int intervalTimer;

};