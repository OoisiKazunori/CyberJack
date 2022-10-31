#pragma once
#include"../Game/Interface/IOperationObject.h"
#include"../KazLibrary/Collision/CollisionManager.h"
#include"../KazLibrary/Sound/SoundManager.h"
#include"../KazLibrary/Imgui/MyImgui.h"
#include"../Enemy/EnemyData.h"

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
	virtual void Init(const EnemyGenerateData &GENERATE_DATA, bool DEMO_FLAG = false) = 0;

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
	void DeadEffect(KazMath::Vec3<float> *POS, KazMath::Vec3<float> *ROTATION, int *ALPHA);

	//死亡判定と処理
	bool ProcessingOfDeath(EnemyDeathType TYPE);
	bool ProcessingOfDeathFbx(EnemyDeathType TYPE);


	/// <returns>敵の情報</returns>
	const std::unique_ptr<EnemyData> &GetData();

	void DeadSound();
	void ShotSound();
	void InitModel(const KazMath::Transform3D &TRANSFORM, const std::string &MODEL_FILEPASS, float HITBOX_RADIUS, bool FBX_OR_OBJ_FLAG, bool REV_UV_FLAG = false);

	//攻撃を確認する用の関数
	virtual void DebugShot() { debugShotFlag = true; };

	void SetLight(const KazMath::Vec3<float> LIGHT_DIR)
	{
		DirectX::XMFLOAT3 dir = LIGHT_DIR.ConvertXMFLOAT3();
		iEnemy_FbxModelRender->TransData(&dir, lightHandle, typeid(DirectX::XMFLOAT3).name());
	};

	std::unique_ptr<EnemyData> iEnemy_EnemyStatusData;		//敵の状態を保存するデータ
	ObjModelRenderPtr iEnemy_ObjModelRender;				//敵の描画
	FbxModelRenderPtr iEnemy_FbxModelRender;				//敵の描画
	KazMath::Vec3<float> lerpPos;

protected:
	bool initDeadSoundFlag;
	bool demoFlag;
	bool debugShotFlag;
private:
	int deadSoundHandle;
	int shotSoundHandle;

	DirtyFlag<short>hpDirtyFlag;
	bool hitFlag;

	RESOURCE_HANDLE lightHandle;

};
