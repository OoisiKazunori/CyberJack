#pragma once
#include"../Game/Interface/IOperationObject.h"
#include"../KazLibrary/Collision/CollisionManager.h"
#include"../KazLibrary/Sound/SoundManager.h"

/// <summary>
/// 敵の種類
/// </summary>
enum eEnemyType
{
	ENEMY_TYPE_NONE = -1,
	ENEMY_TYPE_NORMAL,
	ENEMY_TYPE_MOTHER,
	ENEMY_TYPE_MISILE,
	ENEMY_TYPE_MISILE_SPLINE,
	ENEMY_TYPE_KID
};

/// <summary>
/// 敵から他のOBJを生成する際に渡すデータ
/// </summary>
struct GenarateOtherObjData
{
	GenarateOtherObjData():enemyType(-1),initPos({})
	{
	}

	void Finalize()
	{
		enemyType = -1;
		initPos = {};
	}
	int enemyType;			//どの種類を登場させるか
	XMVECTOR initPos;		//登場させる座標
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
	EnemyData(XMVECTOR *HITBOX_POS_ADRESS, const float &RADIUS, IOperationObjectData *OPERATION_DATA) :
		hitBox({ HITBOX_POS_ADRESS ,RADIUS }),
		oprationObjData(OPERATION_DATA),
		timer(1),
		outOfStageFlag(false)
	{
	};

	Sphere hitBox;							//敵の当たり判定
	IOperationObjectData *oprationObjData;	//操作可能OBJが持つ基本データのアドレス
	GenarateOtherObjData genarateData;		//追加で敵を生成する際に必要な情報
	int timer;								//プレイヤーに当たるまでの時間
	bool outOfStageFlag;					//ステージの範囲外に出てしまった
};

/// <summary>
/// 敵の抽象クラス
/// </summary>
class IEnemy :public IOperationObject
{
public:

	/// <summary>
	/// 確実に初期化したいデータを入れる
	/// </summary>
	IEnemy();
	/// <summary>
	/// 仮想デストラクタ
	/// </summary>
	virtual ~IEnemy() {};

	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <param name="POS">初期座標</param>
	virtual void Init(const XMVECTOR &POS) = 0;

	/// <summary>
	/// 終了処理
	/// </summary>
	virtual void Finalize() = 0;
	/// <summary>
	/// 更新処理
	/// </summary>
	virtual void Update() = 0;
	/// <summary>
	/// 描画処理
	/// </summary>
	virtual void Draw() = 0;

	/// <summary>
	/// 敵を死亡させます
	/// </summary>
	void Dead();

	/// <summary>
	/// 死亡演出
	/// </summary>
	void DeadEffect(XMVECTOR *POS, XMVECTOR *ROTATION, float *ALPHA);

	
	/// <returns>敵の情報</returns>
	const unique_ptr<EnemyData> &GetData();

	void HitSound();
	void DeadSound();

	unique_ptr<EnemyData> iEnemy_EnemyStatusData;		//敵の状態を保存するデータ
	ObjModelRenderPtr iEnemy_ModelRender;				//敵の描画
	XMVECTOR lerpPos;

protected:
	bool initDeadSoundFlag;

private:
	int deadSoundHandle;
	int damageSoundHandle;

};
