#pragma once
#include"../Game/Interface/IOperationObject.h"
#include"../KazLibrary/Collision/CollisionManager.h"

/// <summary>
/// ゲーム内に登場する敵のデータ
/// </summary>
struct EnemyData
{
	/// <summary>
	/// 敵の情報を初期化します
	/// </summary>
	/// <param name="HITBOX_DATA">当たり判定のアドレス</param>
	/// <param name="MAX_ROCKON_NUM">ロックオン回数のアドレス</param>
	EnemyData(const Sphere &HITBOX_DATA, const short &MAX_ROCKON_NUM):
		hitBox(std::make_shared<Sphere>(HITBOX_DATA)), rockOnNum(std::make_shared<short>(MAX_ROCKON_NUM)),
		deadFlag(false), initFlag(false)
	{
	};

	shared_ptr<Sphere> hitBox;		//敵の当たり判定
	bool deadFlag;					//敵の死亡フラグ
	bool initFlag;					//敵の生成フラグ
	shared_ptr<short> rockOnNum;	//ロックオンできる回数
};

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

	/// <returns>敵の情報</returns>
	const unique_ptr<EnemyData> &GetData();


	unique_ptr<EnemyData> iEnemy_EnemyStatusData;		//敵の状態を保存するデータ
	BoxPolygonRenderPtr iEnemy_ModelRender;
};
