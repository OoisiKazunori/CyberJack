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
#include"../Game/LineEffect/LineLevel1.h"
#include"../Game/Stage/FirstStage.h"

struct ResponeData
{
	ResponeData() :layerLevel(-1), flame(-1), enemyType(-1), initPos({})
	{
	}
	int layerLevel;		//�ǂ̃��C���[���x���Ō���邩
	short flame;		//�G�������t���[����
	short enemyType;	//�G�̎��
	XMVECTOR initPos;	//�������W
};

struct LineEffectData
{
	bool usedFlag;
	int lineIndex;
	int enemyTypeIndex;
	int enemyIndex;

	void Reset()
	{
		usedFlag = false;
		lineIndex = -1;
		enemyTypeIndex = -1;
		enemyIndex = -1;
	}
};

enum eEnemyType
{
	ENEMY_TYPE_NONE = -1,
	ENEMY_TYPE_NORMAL,
	ENEMY_TYPE_MOTHER,
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
	//�Q�[�����[�v----------------------------------------------------------------
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
	//�J����----------------------------------------------------------------

	//�J���������̊m�F
	BoxPolygonRenderPtr besidePoly, verticlaPoly,cameraPoly;
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
	static const int LAYER_LEVEL_MAX = 10;				//���C���[���x���̍ő吔
	static const int ENEMY_NUM_MAX = 50;				//1���C���[���x���ɓo�ꂷ��G�̍ő吔

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
	BoxPolygonRenderPtr testEnemyPoly;			//���b�N�I������Ώە`��(�e�X�g)
	//�����o----------------------------------------------------------------

	ObjModelRenderPtr model;

	FirstStage stage;
	bool cameraChangeFlag;
};