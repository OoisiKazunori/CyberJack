#pragma once
#include"../KazLibrary/DirectXCommon/Base.h"
#include"../KazLibrary/Collision/CollisionManager.h"

struct IOperationObjectData
{
	short rockOnNum;		//残りロックオン回数
	bool enableToHitFlag;	//判定が有効なのかどうか判断するフラグ
	bool initFlag;			//既に初期化が通ったか判断するフラグ


	IOperationObjectData()
	{
		rockOnNum = -1;
		initFlag = false;
		enableToHitFlag = false;
	}

	/// <summary>
	/// 操作可能OBJがもつ基本的なデータの初期化
	/// </summary>
	/// <param name="MAX_ROCKON_NUM">ロックオンできる回数</param>
	void Init(const short &MAX_ROCKON_NUM)
	{
		rockOnNum = MAX_ROCKON_NUM;
		enableToHitFlag = true;
		initFlag = true;
	};
};

/// <summary>
/// プレイヤーが干渉出来るOBJの基底クラス
/// </summary>
class IOperationObject
{
public:
	virtual ~IOperationObject() {};

	/// <summary>
	/// ロックオンの残り回数を減らします
	/// </summary>
	void Hit();

	/// <summary>
	/// 残りロックオン回数が0以下ならtrueを返す
	/// </summary>
	bool LockedOrNot();

	/// <summary>
	/// 当たり判定が有効で、残りロックオン数が1以上ならtrueを返す
	/// </summary>
	bool AliveOrNot();

	IOperationObjectData iOperationData;//操作可能OBJ全員が持つ基本データ


protected:
	const float limitLinePosZ = -200.0f;//当たり判定を有効にするライン


	/// <summary>
	/// 指定のZ座標より小さい場所より行こうとしたらfalseを返す
	/// </summary>
	/// <param name="POS_Z">対象のZ座標</param>
	bool EnableToHit(float POS_Z)
	{
		return limitLinePosZ<= POS_Z;
	};

};
