#pragma once
#include"../KazLibrary/DirectXCommon/Base.h"
#include"../KazLibrary/Render/KazRender.h"
#include"../KazLibrary/Collision/CollisionManager.h"

class TestEnemy
{
public:
	TestEnemy();
	void Init(const XMVECTOR &POS,const int &HP);
	void Finalize();
	void Update();
	void Draw();

	void Hit();
	void Dead();

	/// <summary>
	/// 死亡フラグが立っておらず、残りロックオン数が1以上ならtrueが返ってくる
	/// </summary>
	bool AliveOrNot();
	
	/// <summary>
	/// hpが0以下ならtrueを返す
	/// </summary>
	bool LockedOrNot();

	Sphere sphere;
private:
	//状態----------------------------------------------------------------
	short hp;				//何回ロックオンされるか
	bool deadFlag;			//死亡しているか
	bool hitFlag;			//ロックオン後にリリースされた判定
	XMFLOAT4 hitColor;		//当たり判定を取った際の色
	XMFLOAT4 normalColor;	//何も判定を取っていない時の色
	//状態----------------------------------------------------------------


	//描画----------------------------------------------------------------
	BoxPolygonRenderPtr render;		//立方体ポリゴン描画
	//描画----------------------------------------------------------------
};

