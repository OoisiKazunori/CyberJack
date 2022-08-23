#pragma once
#include"../KazLibrary/Render/BackGroundForDebug.h"
#include"../KazLibrary/Render/KazRender.h"
#include<memory>
#include"../Game/Player.h"
#include"../Game/UI/Cursor.h"
#include"../Game/LineEffect/LineLevel1.h"
#include"../Game/Event/GoalBox.h"
#include"../KazLibrary/RenderTarget/RenderTargetStatus.h"
#include"../KazLibrary/RenderTarget/GaussianBuler.h"
#include"../KazLibrary/Render/PolygonRender.h"
#include"../KazLibrary/Scene/SceneBase.h"
#include"../Game/Interface/IStage.h"
#include"../Game/Stage/FirstStage.h"
#include"../Game/Stage/SecondStage.h"
#include"../Game/Stage/ThridStage.h"
#include"../Game/Stage/GpuParticleStage.h"
#include"../Game/Stage/RezStage.h"
#include"../Game/Stage/BlockParticleStage.h"

#include"../Game/Event/MoiveEffect.h"
#include"../Game/UI/AnnounceStageUI.h"
#include"../Game/Event/EventSprite.h"
#include"../Game/Event/TutorialWindow.h"
#include"../Game/Enemy/HitEnemyEffect.h"
#include"../Game/Event/PortalEffect.h"
#include"../Game/Effect/IHitEffectEmitter.h"
#include"../Game/Effect/HitEffectPattern1Emitter.h"
#include"../Game/Effect/HitEffectPattern2Emitter.h"
#include"../Game/Effect/HitEffectPattern3Emitter.h"
#include"../Game/Helper/EnemyHelper.h"

#include"../Game/Effect/MeshParticleEmitter.h"

struct LineEffectData
{
	bool usedFlag;
	KazMath::Vec3<float> startPos;
	int lineIndex;
	int enemyTypeIndex;
	int enemyIndex;
	int eventType;
	bool hitFlag;

	LineEffectData() :startPos({}), usedFlag(false), lineIndex(-1), enemyTypeIndex(-1), enemyIndex(-1), eventType(-1), hitFlag(false)
	{
	}

	void Reset()
	{
		startPos = {};
		usedFlag = false;
		lineIndex = -1;
		enemyTypeIndex = -1;
		enemyIndex = -1;
		eventType = -1;
		hitFlag = false;
	}
};

class Game
{

public:
	//定数--------------------------


	Game();
	~Game();
	void Init(const std::array<std::array<ResponeData,KazEnemyHelper::ENEMY_NUM_MAX>, KazEnemyHelper::LAYER_LEVEL_MAX> &RESPONE_DATA);
	void Finalize();
	void Input();
	void Update();
	void Draw();

	int SceneChange();

private:
	//ゲームループ----------------------------------------------------------------
	bool gameStartFlag;						//ゲーム開始を知らせるフラグ
	int sceneNum;							//次何処のシーンに飛ぶか
	int gameFlame;							//1ステージの経過時間
	int gameSpeed;							//1ステージで進む時間のスピード
	int gameStageLevel;						//現在のステージのレベル
	int gameLeyerLevel;						//現在のステージのレベル
	array<int, 10>changeLayerLevelMaxTime;	//次のレイヤーレベルに切り替わるフレーム数を保管する
	//ゲームループ----------------------------------------------------------------

	//デバック用-------------------------------------------------------------
	BackGroundForDebug bg;
	//デバック用-------------------------------------------------------------


	//カメラ----------------------------------------------------------------
	KazMath::Vec3<float> debugCameraMove;
	KazMath::Vec3<float> eyePos, targetPos;
	KazMath::Vec2<float> angle;

	KazMath::Vec3<float> baseEyePos;						//視点座標の基準値
	KazMath::Vec3<float> baseTargetPos;						//注視点の基準値
	KazMath::Vec3<float> nowTargerPos, trackingTargetPos;	//本来ポズ、現在ポズ
	KazMath::Vec2<float> leftRightAngleVel;					//左右視点座標の向く角度
	KazMath::Vec2<float> upDownAngleVel;					//上下視点座標の向く角度
	KazMath::Vec2<float> trackLeftRightAngleVel;
	KazMath::Vec2<float> trackUpDownAngleVel;
	KazMath::Vec2<float> forceCameraDirVel;					//カメラの前後左右の向きを強制的に指定する
	KazMath::Vec2<float> mulValue;							//カメラの上下左右の掛ける割合
	KazMath::Vec2<float> mulValue2;							//カメラの上下左右の掛ける割合

	KazMath::Vec3<float> honraiPlayerCameraPos;

	KazMath::Vec3<float> layerLevelEyePos;
	KazMath::Vec3<float> layerLevelTargetPos;
	KazMath::Vec3<float> layerCameraMove;

	KazMath::Vec3<float>cameraVel;

	const float FORCE_CAMERA_FRONT = -90.0f;
	const float FORCE_CAMERA_BACK = -270.0f;
	const float FORCE_CAMERA_LEFT = 0.0f;
	const float FORCE_CAMERA_RIGHT = -180.0f;

	enum CameraDir
	{
		CAMERA_FRONT,
		CAMERA_BACK,
		CAMERA_LEFT,
		CAMERA_RIGHT,
	};
	struct ForceCameraData
	{
		int flame;
		CameraDir dir;
		ForceCameraData() :flame(-1)
		{
		};
	};
	std::array<std::array<ForceCameraData, 10>, 3>cameraMoveArray;

	//カメラ----------------------------------------------------------------

	//カメラ挙動の確認
	BoxPolygonRenderPtr besidePoly, verticlaPoly, cameraPoly;
	KazMath::Vec3<float> centralPos;		//左右回転の中心座標
	KazMath::Vec3<float> centralPos2;		//上下回転の中心座標
	float r;					//左右回転の円の大きさ
	float r2;					//上下回転の円の大きさ

	KazMath::Vec3<float> forceCameraAngle;	//カメラを強制的に他の方向に向かせる際に使用する値

	//プレイヤーが操作するもの----------------------------------------------------------------
	Player player;
	Cursor cursor;
	//プレイヤーが操作するもの----------------------------------------------------------------

	//敵----------------------------------------------------------------
	array<unique_ptr<IEnemy>, 2>enemy;					//敵(サンプル)
	array<array<unique_ptr<IEnemy>, KazEnemyHelper::ENEMY_NUM_MAX>, KazEnemyHelper::LAYER_LEVEL_MAX> enemies;	//1ステージに生成する敵の総数
	array<array<BoxPolygonRender, KazEnemyHelper::ENEMY_NUM_MAX>, KazEnemyHelper::LAYER_LEVEL_MAX> enemyHitBox;
	array<int, 10> enemiesHandle;						//0から順番に初期化する際に必要
	array<int, 10> addEnemiesHandle;					//0から順番に追加で初期化する際に必要
	array<array<ResponeData, KazEnemyHelper::ENEMY_NUM_MAX>, KazEnemyHelper::LAYER_LEVEL_MAX> responeData;		//敵を生成する際に必要な設定
	array<ResponeData, 50>addResponeData;				//敵を追加で生成する際に必要な設定をスタックしたもの
	bool isEnemyNotMoveFlag;
	int notMoveTimer;
	const int CHANGE_GMAE_FLAME_SPEED_MAX_TIME = 2;	//敵が居なくなってからゲーム内時間の進むスピードが速まるまでの間隔
	//敵----------------------------------------------------------------

	//線演出----------------------------------------------------------------
	static const int LINE_MAX_ARRAY_NUM = 16;
	array<LineLevel1, LINE_MAX_ARRAY_NUM> lineLevel;			//直角に敵に線を伸ばしていく演出
	array<LineEffectData, LINE_MAX_ARRAY_NUM>lineEffectArrayData;
	KazMath::Vec3<float> testEnemyPos;			//ロックオンする対象座標(テスト)
	//線演出----------------------------------------------------------------

	//攻撃演出-------------------------
	std::array<HitEnemyEffect, 30> hitEffect;
	//攻撃演出-------------------------

	ObjModelRenderPtr model;

	GoalBox goalBox;
	KazMath::Vec3<float> appearGoalBoxPos;
	KazMath::Vec3<float> responeGoalBoxPos;
	bool initAppearFlag;
	bool changeStageFlag;

	//画面効果準備-----------------------
	Sprite2DRender mainRenderTarget;
	Sprite2DRender addRenderTarget;
	Sprite2DRender luminaceTex;
	RESOURCE_HANDLE addHandle;
	RESOURCE_HANDLE potalTexHandle;
	std::array<std::unique_ptr<GaussianBuler>, 4> buler;

	int stageNum;
	std::array<std::unique_ptr<IStage>, 3>stages;
	bool cameraChangeFlag;
	bool lineDebugFlag;

	std::vector<RESOURCE_HANDLE> handles;



	std::array<BoxPolygonRender, 8> lineStartPoly;


	MoiveEffect movieEffect;//カメラが固定になって演出を描画する


	//ゲームオーバー画面----------------------------------------------
	Sprite2DRender gameOverTex;
	Sprite2DRender pressAButtonTex;
	Sprite2DRender blackTex;
	bool gameOverFlag;
	bool readyToBlackOutFlag;
	bool readyToBlackOutToGoTitleFlag;
	int flashTimer;
	bool flashFlag;
	//ゲームオーバー画面----------------------------------------------


	//タイトル画面----------------------------------------------
	Sprite3DRender titleLogoTex;
	float baseTitlePosY;
	EventSprite doneSprite;
	bool startFlag;

	float titleT;
	bool titleTReversFlag;
	//タイトル画面----------------------------------------------

	//UI--------------------------------------
	AnnounceStageUI stageUI;


	int bgmSoundHandle, lockSoundHandle;
	int doneSoundHandle;

	TutorialWindow tutorialWindow;

	KazMath::Vec3<float>tPos;

	CircleRender circle;
	PortalEffect portal;

	bool smokeFlag;
	std::array<Sprite2DRender,4> smokeR;

	int emittNum;
	std::array<std::unique_ptr<IHitEffectEmitter>, 3>emitters;

	std::array<std::array<std::unique_ptr<IHitEffectEmitter>, 10>, 3>deadEffectEmitter;


	//エフェクト--------------------------------------
	std::array<RocketEffect, 30>rocketEffect;
	int rocketIndex;
	//エフェクト--------------------------------------

	BoxPolygonRender box;

	MeshParticleEmitter meshEmitter;
};
