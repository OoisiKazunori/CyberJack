#pragma once
#include"../KazLibrary/DirectXCommon/Base.h"
#include"../KazLibrary/Collision/CollisionManager.h"
#include"../KazLibrary/Render/KazRender.h"

class Cursor
{
public:
	Cursor();

	void Init();
	void Input(bool UP_FLAG, bool DOWN_FLAG, bool LEFT_FLAG, bool RIGHT_FLAG, bool DONE_FLAG,bool RELEASE_FLAG);
	void Update();
	void Draw();

	/// <summary>
	/// ロックオン数が最大値未満であること、ロックオンが有効でロックオンの入力がされている
	/// 時にtrueが変える
	/// </summary>
	/// <returns></returns>
	bool LockOn();
	bool Release();

	void Count();
	const int &GetCount();

	const XMVECTOR &GetValue();

	XMVECTOR honraiCursorPos;
	XMVECTOR cursorPos;
	XMVECTOR vel;
	Ray hitBox;
	XMINT2 stopFlag;
private:
	//ロックオン数----------------------------------------------------------------
	static const int LOCKON_MAX_NUM = 8;	//最大ロックオン数
	int lockOnNum;							//現在のロックオン数
	bool notEnableLockOnFlag;
	int enableLockOnTimer;

	bool releaseFlag;

	//移動----------------------------------------------------------------
	bool upFlag, downFlag, leftFlag, rightFlag,doneFlag;


	//画像----------------------------------------------------------------
	Sprite2DRenderPtr cursorFlameTex, numberTex;	//スクリーン座標の描画
	array<short, LOCKON_MAX_NUM + 1> numberHandle;			//数字用の画像ハンドル
	short flameHandle;
	
	float speed;
	XMVECTOR knockBackVal;	//反転移動用の移動量
	XMVECTOR oldPos;
	XMVECTOR oldVel;
};

