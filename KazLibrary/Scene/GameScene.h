#pragma once
#include"../Scene/SceneBase.h"
#include"../Render/BackGroundForDebug.h"
#include"../Render/KazRender.h"
#include<memory>
#include"../Player.h"
#include"../Cursor.h"
#include"../TestEnemy.h"
#include"../Game/Interface/IEnemy.h"
#include"../Game/Enemy/NormalEnemy.h"
#include"../Game/Enemy/KidEnemy.h"

struct ResponeData
{
	ResponeData() :layerLevel(-1), flame(-1), enemyType(-1), initPos({})
	{
	}
	int layerLevel;		//どのレイヤーレベルで現れるか
	short flame;		//敵が現れるフレーム数
	short enemyType;	//敵の種類
	XMVECTOR initPos;	//初期座標
};

enum eEnemyType
{
	ENEMY_TYPE_NONE = -1,
	ENEMY_TYPE_NORMAL,
	ENEMY_TYPE_KID
};

class GameScene :public SceneBase
{
public:
	GameScene();
	~GameScene();

	void Init();
	void Finalize();
	void Input();
	void Update();
	void Draw();

	int SceneChange();

private:
	//ゲームループ----------------------------------------------------------------
	int sceneNum;							//次何処のシーンに飛ぶか
	int gameFlame;							//1ステージの経過時間
	int gameStageLevel;						//現在のステージのレベル
	int gameLeyerLevel;						//現在のステージのレベル
	array<int, 10>changeLayerLevelMaxTime;	//次のレイヤーレベルに切り替わるフレーム数を保管する
	//ゲームループ----------------------------------------------------------------

	//デバック用-------------------------------------------------------------
	BackGroundForDebug bg;
	//デバック用-------------------------------------------------------------


	//カメラ----------------------------------------------------------------
	XMFLOAT3 debugCameraMove;
	XMFLOAT3 eyePos, targetPos;
	XMFLOAT2 angle;

	XMVECTOR baseEyePos;						//視点座標の基準値
	XMVECTOR baseTargetPos;						//注視点の基準値
	XMVECTOR nowTargerPos, trackingTargetPos;	//本来ポズ、現在ポズ
	XMVECTOR leftRightAngleVel;					//左右視点座標の向く角度
	XMVECTOR upDownAngleVel;					//上下視点座標の向く角度
	XMVECTOR trackLeftRightAngleVel;
	XMVECTOR trackUpDownAngleVel;
	//カメラ----------------------------------------------------------------

	//カメラ挙動の確認
	BoxPolygonRenderPtr besidePoly, verticlaPoly,cameraPoly;
	XMVECTOR centralPos;		//左右回転の中心座標
	XMVECTOR centralPos2;		//上下回転の中心座標
	float r;					//左右回転の円の大きさ
	float r2;					//上下回転の円の大きさ

	XMVECTOR cameraRotaVel;		//カメラを動かした移動量
	XMVECTOR forceCameraAngle;	//カメラを強制的に他の方向に向かせる際に使用する値

	//プレイヤーが操作するもの----------------------------------------------------------------
	Player player;
	Cursor cursor;
	//プレイヤーが操作するもの----------------------------------------------------------------

	//敵----------------------------------------------------------------
	array<unique_ptr<IEnemy>, 2>enemy;					//敵(サンプル)
	TestEnemy hitBox;									//敵(サンプル)
	array<array<unique_ptr<IEnemy>, 10>, 10> enemies;	//1ステージに生成する敵の総数
	array<int, 10> enemiesHandle;						//0から順番に初期化する際に必要
	array<int, 10> addEnemiesHandle;					//0から順番に追加で初期化する際に必要
	array<array<ResponeData, 10>, 50> responeData;		//敵を生成する際に必要な設定
	array<ResponeData, 50>addResponeData;				//敵を追加で生成する際に必要な設定をスタックしたもの
	//敵----------------------------------------------------------------
};