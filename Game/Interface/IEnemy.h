#pragma once
#include"../Game/Interface/IOperationObject.h"
#include"../KazLibrary/Collision/CollisionManager.h"
#include"../KazLibrary/Sound/SoundManager.h"
#include"../KazLibrary/Imgui/MyImgui.h"
#include"../Enemy/EnemyData.h"

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
	virtual void Init(const EnemyGenerateData &GENERATE_DATA, bool DEMO_FLAG = false) = 0;

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
	void DeadEffect(KazMath::Vec3<float> *POS, KazMath::Vec3<float> *ROTATION, int *ALPHA);


	/// <returns>�G�̏��</returns>
	const std::unique_ptr<EnemyData> &GetData();

	void HitSound();
	void DeadSound();

	//�U�����m�F����p�̊֐�
	virtual void DebugShot() { debugShotFlag = true; };

	std::unique_ptr<EnemyData> iEnemy_EnemyStatusData;		//�G�̏�Ԃ�ۑ�����f�[�^
	ObjModelRenderPtr iEnemy_ModelRender;				//�G�̕`��
	KazMath::Vec3<float> lerpPos;

protected:
	bool initDeadSoundFlag;
	bool demoFlag;
	bool debugShotFlag;
private:
	int deadSoundHandle;
	int damageSoundHandle;


};
