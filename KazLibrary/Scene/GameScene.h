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
	array<unique_ptr<IEnemy>, 2>enemy;					//�G(�T���v��)
	TestEnemy hitBox;									//�G(�T���v��)
	array<array<unique_ptr<IEnemy>, 10>, 10> enemies;	//1�X�e�[�W�ɐ�������G�̑���
	array<int, 10> enemiesHandle;						//0���珇�Ԃɏ���������ۂɕK�v
	array<int, 10> addEnemiesHandle;					//0���珇�Ԃɒǉ��ŏ���������ۂɕK�v
	array<array<ResponeData, 10>, 50> responeData;		//�G�𐶐�����ۂɕK�v�Ȑݒ�
	array<ResponeData, 50>addResponeData;				//�G��ǉ��Ő�������ۂɕK�v�Ȑݒ���X�^�b�N��������
	//�G----------------------------------------------------------------



	//�����o----------------------------------------------------------------
	LineLevel1 lineLevel;			//���p�ɓG�ɐ���L�΂��Ă������o
	XMVECTOR testEnemyPos;			//���b�N�I������Ώۍ��W(�e�X�g)
	BoxPolygonRenderPtr testEnemyPoly;			//���b�N�I������Ώە`��(�e�X�g)
	//�����o----------------------------------------------------------------
};