#pragma once
#include"../Game/Interface/IOperationObject.h"
#include"../KazLibrary/Collision/CollisionManager.h"
#include"../KazLibrary/Math/KazMath.h"

/// <summary>
/// 敵の種類
/// </summary>
enum eEnemyType
{
	ENEMY_TYPE_NONE = -1,
	ENEMY_TYPE_NORMAL,
	ENEMY_TYPE_MISILE,
	ENEMY_TYPE_MISILE_SPLINE,
	ENEMY_TYPE_MOTHER,
	ENEMY_TYPE_KID,
	ENEMY_TYPE_BATTLESHIP,
	ENEMY_TYPE_BATTLESHIP_MISILE
};

enum BattleShipMisileEnum
{
	BATTLESHIP_MISILE_SHOT_LEFT,
	BATTLESHIP_MISILE_SHOT_RIGHT,
};

struct BattleShipMisileData
{
	//左右どちらに飛ぶか
	BattleShipMisileEnum shotEnum;
};

struct BattleShipData
{
	//ミサイルを撃つかどうか
	bool isShotFlag;

	BattleShipData() :isShotFlag(false)
	{};
};

/// <summary>
/// 敵を生成する際に必要な情報を詰め込む
/// </summary>
struct EnemyGenerateData
{
	KazMath::Vec3<float> initPos;	//初期座標
	float speed;
	BattleShipMisileData battleShipEnemyData;
	BattleShipData battleShipData;

	EnemyGenerateData() :speed(-1.5f)
	{
	};
};

struct ResponeData
{
	ResponeData() :layerLevel(-1), flame(-1)
	{
	}
	int layerLevel;		//どのレイヤーレベルで現れるか
	int flame;		//敵が現れるフレーム数
	EnemyGenerateData generateData;
};


/// <summary>
/// 敵から他のOBJを生成する際に渡すデータ
/// </summary>
struct GenarateOtherObjData
{
	GenarateOtherObjData() :enemyType(-1)
	{
	}

	void Finalize()
	{
		enemyType = -1;
	}
	int enemyType;			//どの種類を登場させるか
	EnemyGenerateData enemyGenerateData;
};

/// <summary>
/// ゲーム内に登場する敵のデータ
/// </summary>
struct EnemyData
{
	/// <summary>
	/// 敵の情報を初期化します
	/// </summary>
	/// <param name="HITBOX_POS_ADRESS">当たり判定用の座標アドレス</param>
	/// <param name="RADIUS">当たり判定の大きさ</param>
	/// <param name="MAX_ROCKON_NUM">ロックオン回数のアドレス</param>
	EnemyData(KazMath::Vec3<float> *HITBOX_POS_ADRESS, const float &RADIUS, IOperationObjectData *OPERATION_DATA) :
		hitBox({ HITBOX_POS_ADRESS ,RADIUS }),
		oprationObjData(OPERATION_DATA),
		timer(1),
		outOfStageFlag(false),
		radius(0.0f),
		startFlag(false)
	{
	};

	Sphere hitBox;							//敵の当たり判定
	IOperationObjectData *oprationObjData;	//操作可能OBJが持つ基本データのアドレス
	GenarateOtherObjData genarateData;		//追加で敵を生成する際に必要な情報
	int timer;								//プレイヤーに当たるまでの時間
	bool outOfStageFlag;					//ステージの範囲外に出てしまった
	float radius;
	bool startFlag;
};