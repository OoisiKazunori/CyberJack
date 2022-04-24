#pragma once
#include"../KazLibrary/DirectXCommon/Base.h"
#include"../KazLibrary/Collision/CollisionManager.h"
#include"../KazLibrary/Render/KazRender.h"

class Cursor
{
public:
	Cursor();

	void Init();
	void Input(bool UP_FLAG, bool DOWN_FLAG, bool LEFT_FLAG, bool RIGHT_FLAG, bool DONE_FLAG, bool RELEASE_FLAG, const XMVECTOR &ANGLE);
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
	XMVECTOR prevCursorPos;
	XMVECTOR vel;
	Ray hitBox;
	XMINT2 stopFlag;
	bool releaseFlag;


	XMVECTOR honraiCameraMoveValue;//上下左右にカメラをどれくらい動かすかの値を保存する(本来ポズ)
	XMVECTOR cameraMoveValue;//上下左右にカメラをどれくらい動かすかの値を保存する
	XMVECTOR dontMoveCameraStartPos;//カメラの無操作開始地点
	XMVECTOR dontMoveCameraEndPos;//カメラの無操作終了地点

	XMVECTOR limitValue;//カメラの端の値

	static XMVECTOR NO_MOVE_DISTANCE;//XYそれぞれどこまでの範囲ならカーソル動かしてもカメラに影響が出ないか

private:
	//ロックオン数----------------------------------------------------------------
	static const int LOCKON_MAX_NUM = 8;	//最大ロックオン数
	int lockOnNum;							//現在のロックオン数
	bool notEnableLockOnFlag;
	int enableLockOnTimer;

	//移動----------------------------------------------------------------
	bool upFlag, downFlag, leftFlag, rightFlag,doneFlag;
	XMVECTOR stickAngle;
	float deadLine;
	float baseSpeed;
	XMFLOAT2 speed;

	//入力演出-----------------------

	struct CursorEffectData
	{
		bool initFlag;
		Sprite2DRenderPtr cursorEffectTex;

		CursorEffectData() :cursorEffectTex(std::make_unique<Sprite2DRender>()), initFlag(false)
		{
		}
	};

	std::array<CursorEffectData, 10> cursorEffectTex;





	//画像----------------------------------------------------------------
	Sprite2DRenderPtr cursorFlameTex, numberTex;	//スクリーン座標の描画
	array<short, LOCKON_MAX_NUM + 1> numberHandle;			//数字用の画像ハンドル
	short flameHandle;
	XMVECTOR baseScale;

	XMVECTOR oldPos;
	XMVECTOR oldVel;


	//ノックバック挙動-----------------------
	XMVECTOR knockBackVal;	//反転移動用の移動量
	static XMFLOAT2 KOCKBACK_MAX_VALUE;		//ノックバックの最大量(値が決まったらconstにする)
	static XMFLOAT2 KOCKBACK_VELOCITY;		//ノックバックの加算量(値が決まったらconstにする)
	XMINT2 noTochFlag;
	XMINT2 noTockTimer;
	//ノックバック挙動-----------------------
};

