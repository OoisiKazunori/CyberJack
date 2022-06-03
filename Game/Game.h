#pragma once
#include"../KazLibrary/Render/BackGroundForDebug.h"
#include"../KazLibrary/Render/KazRender.h"
#include<memory>
#include"../Game/Player.h"
#include"../Game/UI/Cursor.h"
#include"../Game/Debug/TestEnemy.h"
#include"../Game/Interface/IEnemy.h"
#include"../Game/Enemy/NormalEnemy.h"
#include"../Game/Enemy/KidEnemy.h"
#include"../Game/Enemy/SplineMisile.h"
#include"../Game/Enemy/NormalMisileEnemy.h"
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
#include"../Game/Event/MoiveEffect.h"
#include"../Game/UI/AnnounceStageUI.h"
#include"../Game/Event/EventSprite.h"
#include"../Game/Event/TutorialWindow.h"

struct ResponeData
{
	ResponeData() :layerLevel(-1), flame(-1), initPos({})
	{
	}
	int layerLevel;		//�ǂ̃��C���[���x���Ō���邩
	short flame;		//�G�������t���[����
	XMVECTOR initPos;	//�������W
	
};

struct LineEffectData
{
	bool usedFlag;
	XMVECTOR startPos;
	int lineIndex;
	int enemyTypeIndex;
	int enemyIndex;
	int eventType;

	LineEffectData() :startPos({}), usedFlag(false), lineIndex(-1), enemyTypeIndex(-1), enemyIndex(-1), eventType(-1)
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
	}
};

class Game
{

public:
	//�萔--------------------------
	static const int LAYER_LEVEL_MAX = 10;				//���C���[���x���̍ő吔
	static const int ENEMY_NUM_MAX = 100;				//1���C���[���x���ɓo�ꂷ��G�̍ő吔


	Game();
	~Game();
	void Init(const array<array<ResponeData, ENEMY_NUM_MAX>, LAYER_LEVEL_MAX> &RESPONE_DATA);
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
	int gameStageLevel;						//���݂̃X�e�[�W�̃��x��
	int gameLeyerLevel;						//���݂̃X�e�[�W�̃��x��
	array<int, 10>changeLayerLevelMaxTime;	//���̃��C���[���x���ɐ؂�ւ��t���[������ۊǂ���
	//�Q�[�����[�v----------------------------------------------------------------

	//�f�o�b�N�p-------------------------------------------------------------
	BackGroundForDebug bg;
	//�f�o�b�N�p-------------------------------------------------------------


	//�J����----------------------------------------------------------------
	XMFLOAT3 debugCameraMove;
	XMFLOAT3 eyePos, targetPos;
	XMFLOAT2 angle;

	XMVECTOR baseEyePos;						//���_���W�̊�l
	XMVECTOR baseTargetPos;						//�����_�̊�l
	XMVECTOR nowTargerPos, trackingTargetPos;	//�{���|�Y�A���݃|�Y
	XMVECTOR leftRightAngleVel;					//���E���_���W�̌����p�x
	XMVECTOR upDownAngleVel;					//�㉺���_���W�̌����p�x
	XMVECTOR trackLeftRightAngleVel;
	XMVECTOR trackUpDownAngleVel;
	XMVECTOR forceCameraDirVel;					//�J�����̑O�㍶�E�̌����������I�Ɏw�肷��
	XMFLOAT2 mulValue;							//�J�����̏㉺���E�̊|���銄��
	XMFLOAT2 mulValue2;							//�J�����̏㉺���E�̊|���銄��

	XMVECTOR honraiPlayerCameraPos;

	XMFLOAT3 layerLevelEyePos;
	XMFLOAT3 layerLevelTargetPos;
	XMFLOAT3 layerCameraMove;
	XMVECTOR subPotalSpritePos;

	//�J����----------------------------------------------------------------

	//�J���������̊m�F
	BoxPolygonRenderPtr besidePoly, verticlaPoly, cameraPoly;
	XMVECTOR centralPos;		//���E��]�̒��S���W
	XMVECTOR centralPos2;		//�㉺��]�̒��S���W
	float r;					//���E��]�̉~�̑傫��
	float r2;					//�㉺��]�̉~�̑傫��

	XMVECTOR forceCameraAngle;	//�J�����������I�ɑ��̕����Ɍ�������ۂɎg�p����l

	//�v���C���[�����삷�����----------------------------------------------------------------
	Player player;
	Cursor cursor;
	//�v���C���[�����삷�����----------------------------------------------------------------

	//�G----------------------------------------------------------------
	array<unique_ptr<IEnemy>, 2>enemy;					//�G(�T���v��)
	TestEnemy hitBox;									//�G(�T���v��)
	array<array<unique_ptr<IEnemy>, ENEMY_NUM_MAX>, LAYER_LEVEL_MAX> enemies;	//1�X�e�[�W�ɐ�������G�̑���
	array<int, 10> enemiesHandle;						//0���珇�Ԃɏ���������ۂɕK�v
	array<int, 10> addEnemiesHandle;					//0���珇�Ԃɒǉ��ŏ���������ۂɕK�v
	array<array<ResponeData, ENEMY_NUM_MAX>, LAYER_LEVEL_MAX> responeData;		//�G�𐶐�����ۂɕK�v�Ȑݒ�
	array<ResponeData, 50>addResponeData;				//�G��ǉ��Ő�������ۂɕK�v�Ȑݒ���X�^�b�N��������
	//�G----------------------------------------------------------------

	//�����o----------------------------------------------------------------
	array<LineLevel1, 8> lineLevel;			//���p�ɓG�ɐ���L�΂��Ă������o
	array<LineEffectData, 8>lineEffectArrayData;
	XMVECTOR testEnemyPos;			//���b�N�I������Ώۍ��W(�e�X�g)
	//�����o----------------------------------------------------------------

	ObjModelRenderPtr model;

	GoalBox goalBox;
	XMVECTOR appearGoalBoxPos;
	XMVECTOR responeGoalBoxPos;
	bool initAppearFlag;
	bool changeStageFlag;

	//��ʌ��ʏ���-----------------------
	Sprite2DRender mainRenderTarget;
	Sprite2DRender addRenderTarget;
	Sprite2DRender luminaceTex;
	short addHandle;
	short potalTexHandle;
	std::unique_ptr<GaussianBuler> buler;

	int stageNum;
	std::array<std::unique_ptr<IStage>, 3>stages;
	bool cameraChangeFlag;
	bool lineDebugFlag;

	std::vector<short> handles;



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


	TutorialWindow tutorialWindow;

};

