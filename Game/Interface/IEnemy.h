#pragma once
#include"../Game/Interface/IOperationObject.h"
#include"../KazLibrary/Collision/CollisionManager.h"
#include"../KazLibrary/Sound/SoundManager.h"

/// <summary>
/// �G�̎��
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
/// �G���瑼��OBJ�𐶐�����ۂɓn���f�[�^
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
	int enemyType;			//�ǂ̎�ނ�o�ꂳ���邩
	XMVECTOR initPos;		//�o�ꂳ������W
};

/// <summary>
/// �Q�[�����ɓo�ꂷ��G�̃f�[�^
/// </summary>
struct EnemyData
{
	/// <summary>
	/// �G�̏������������܂�
	/// </summary>
	/// <param name="HITBOX_POS_ADRESS">�����蔻��p�̍��W�A�h���X</param>
	/// <param name="RADIUS">�����蔻��̑傫��</param>
	/// <param name="MAX_ROCKON_NUM">���b�N�I���񐔂̃A�h���X</param>
	EnemyData(XMVECTOR *HITBOX_POS_ADRESS, const float &RADIUS, IOperationObjectData *OPERATION_DATA) :
		hitBox({ HITBOX_POS_ADRESS ,RADIUS }),
		oprationObjData(OPERATION_DATA),
		timer(1),
		outOfStageFlag(false)
	{
	};

	Sphere hitBox;							//�G�̓����蔻��
	IOperationObjectData *oprationObjData;	//����\OBJ������{�f�[�^�̃A�h���X
	GenarateOtherObjData genarateData;		//�ǉ��œG�𐶐�����ۂɕK�v�ȏ��
	int timer;								//�v���C���[�ɓ�����܂ł̎���
	bool outOfStageFlag;					//�X�e�[�W�͈̔͊O�ɏo�Ă��܂���
};

/// <summary>
/// �G�̒��ۃN���X
/// </summary>
class IEnemy :public IOperationObject
{
public:

	/// <summary>
	/// �m���ɏ������������f�[�^������
	/// </summary>
	IEnemy();
	/// <summary>
	/// ���z�f�X�g���N�^
	/// </summary>
	virtual ~IEnemy() {};

	/// <summary>
	/// ����������
	/// </summary>
	/// <param name="POS">�������W</param>
	virtual void Init(const XMVECTOR &POS) = 0;

	/// <summary>
	/// �I������
	/// </summary>
	virtual void Finalize() = 0;
	/// <summary>
	/// �X�V����
	/// </summary>
	virtual void Update() = 0;
	/// <summary>
	/// �`�揈��
	/// </summary>
	virtual void Draw() = 0;

	/// <summary>
	/// �G�����S�����܂�
	/// </summary>
	void Dead();

	/// <summary>
	/// ���S���o
	/// </summary>
	void DeadEffect(XMVECTOR *POS, XMVECTOR *ROTATION, float *ALPHA);

	
	/// <returns>�G�̏��</returns>
	const unique_ptr<EnemyData> &GetData();

	void HitSound();
	void DeadSound();

	unique_ptr<EnemyData> iEnemy_EnemyStatusData;		//�G�̏�Ԃ�ۑ�����f�[�^
	ObjModelRenderPtr iEnemy_ModelRender;				//�G�̕`��
	XMVECTOR lerpPos;

protected:
	bool initDeadSoundFlag;

private:
	int deadSoundHandle;
	int damageSoundHandle;

};
