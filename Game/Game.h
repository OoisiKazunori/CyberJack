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
	//�萔--------------------------


	Game();
	~Game();
	void Init(const std::array<std::array<ResponeData,KazEnemyHelper::ENEMY_NUM_MAX>, KazEnemyHelper::LAYER_LEVEL_MAX> &RESPONE_DATA);
	void Finalize();
	void Input();
	void Update();
	void Draw();

	int SceneChange();

private:
	//�Q�[�����[�v----------------------------------------------------------------
	bool gameStartFlag;						//�Q�[���J�n��m�点��t���O
	int sceneNum;							//�������̃V�[���ɔ�Ԃ�
	int gameFlame;							//1�X�e�[�W�̌o�ߎ���
	int gameSpeed;							//1�X�e�[�W�Ői�ގ��Ԃ̃X�s�[�h
	int gameStageLevel;						//���݂̃X�e�[�W�̃��x��
	int gameLeyerLevel;						//���݂̃X�e�[�W�̃��x��
	array<int, 10>changeLayerLevelMaxTime;	//���̃��C���[���x���ɐ؂�ւ��t���[������ۊǂ���
	//�Q�[�����[�v----------------------------------------------------------------

	//�f�o�b�N�p-------------------------------------------------------------
	BackGroundForDebug bg;
	//�f�o�b�N�p-------------------------------------------------------------


	//�J����----------------------------------------------------------------
	KazMath::Vec3<float> debugCameraMove;
	KazMath::Vec3<float> eyePos, targetPos;
	KazMath::Vec2<float> angle;

	KazMath::Vec3<float> baseEyePos;						//���_���W�̊�l
	KazMath::Vec3<float> baseTargetPos;						//�����_�̊�l
	KazMath::Vec3<float> nowTargerPos, trackingTargetPos;	//�{���|�Y�A���݃|�Y
	KazMath::Vec2<float> leftRightAngleVel;					//���E���_���W�̌����p�x
	KazMath::Vec2<float> upDownAngleVel;					//�㉺���_���W�̌����p�x
	KazMath::Vec2<float> trackLeftRightAngleVel;
	KazMath::Vec2<float> trackUpDownAngleVel;
	KazMath::Vec2<float> forceCameraDirVel;					//�J�����̑O�㍶�E�̌����������I�Ɏw�肷��
	KazMath::Vec2<float> mulValue;							//�J�����̏㉺���E�̊|���銄��
	KazMath::Vec2<float> mulValue2;							//�J�����̏㉺���E�̊|���銄��

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

	//�J����----------------------------------------------------------------

	//�J���������̊m�F
	BoxPolygonRenderPtr besidePoly, verticlaPoly, cameraPoly;
	KazMath::Vec3<float> centralPos;		//���E��]�̒��S���W
	KazMath::Vec3<float> centralPos2;		//�㉺��]�̒��S���W
	float r;					//���E��]�̉~�̑傫��
	float r2;					//�㉺��]�̉~�̑傫��

	KazMath::Vec3<float> forceCameraAngle;	//�J�����������I�ɑ��̕����Ɍ�������ۂɎg�p����l

	//�v���C���[�����삷�����----------------------------------------------------------------
	Player player;
	Cursor cursor;
	//�v���C���[�����삷�����----------------------------------------------------------------

	//�G----------------------------------------------------------------
	array<unique_ptr<IEnemy>, 2>enemy;					//�G(�T���v��)
	array<array<unique_ptr<IEnemy>, KazEnemyHelper::ENEMY_NUM_MAX>, KazEnemyHelper::LAYER_LEVEL_MAX> enemies;	//1�X�e�[�W�ɐ�������G�̑���
	array<array<BoxPolygonRender, KazEnemyHelper::ENEMY_NUM_MAX>, KazEnemyHelper::LAYER_LEVEL_MAX> enemyHitBox;
	array<int, 10> enemiesHandle;						//0���珇�Ԃɏ���������ۂɕK�v
	array<int, 10> addEnemiesHandle;					//0���珇�Ԃɒǉ��ŏ���������ۂɕK�v
	array<array<ResponeData, KazEnemyHelper::ENEMY_NUM_MAX>, KazEnemyHelper::LAYER_LEVEL_MAX> responeData;		//�G�𐶐�����ۂɕK�v�Ȑݒ�
	array<ResponeData, 50>addResponeData;				//�G��ǉ��Ő�������ۂɕK�v�Ȑݒ���X�^�b�N��������
	bool isEnemyNotMoveFlag;
	int notMoveTimer;
	const int CHANGE_GMAE_FLAME_SPEED_MAX_TIME = 2;	//�G�����Ȃ��Ȃ��Ă���Q�[�������Ԃ̐i�ރX�s�[�h�����܂�܂ł̊Ԋu
	//�G----------------------------------------------------------------

	//�����o----------------------------------------------------------------
	static const int LINE_MAX_ARRAY_NUM = 16;
	array<LineLevel1, LINE_MAX_ARRAY_NUM> lineLevel;			//���p�ɓG�ɐ���L�΂��Ă������o
	array<LineEffectData, LINE_MAX_ARRAY_NUM>lineEffectArrayData;
	KazMath::Vec3<float> testEnemyPos;			//���b�N�I������Ώۍ��W(�e�X�g)
	//�����o----------------------------------------------------------------

	//�U�����o-------------------------
	std::array<HitEnemyEffect, 30> hitEffect;
	//�U�����o-------------------------

	ObjModelRenderPtr model;

	GoalBox goalBox;
	KazMath::Vec3<float> appearGoalBoxPos;
	KazMath::Vec3<float> responeGoalBoxPos;
	bool initAppearFlag;
	bool changeStageFlag;

	//��ʌ��ʏ���-----------------------
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


	MoiveEffect movieEffect;//�J�������Œ�ɂȂ��ĉ��o��`�悷��


	//�Q�[���I�[�o�[���----------------------------------------------
	Sprite2DRender gameOverTex;
	Sprite2DRender pressAButtonTex;
	Sprite2DRender blackTex;
	bool gameOverFlag;
	bool readyToBlackOutFlag;
	bool readyToBlackOutToGoTitleFlag;
	int flashTimer;
	bool flashFlag;
	//�Q�[���I�[�o�[���----------------------------------------------


	//�^�C�g�����----------------------------------------------
	Sprite3DRender titleLogoTex;
	float baseTitlePosY;
	EventSprite doneSprite;
	bool startFlag;

	float titleT;
	bool titleTReversFlag;
	//�^�C�g�����----------------------------------------------

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


	//�G�t�F�N�g--------------------------------------
	std::array<RocketEffect, 30>rocketEffect;
	int rocketIndex;
	//�G�t�F�N�g--------------------------------------

	BoxPolygonRender box;

	MeshParticleEmitter meshEmitter;
};
