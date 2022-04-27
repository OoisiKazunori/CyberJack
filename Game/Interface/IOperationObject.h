#pragma once
#include"../KazLibrary/DirectXCommon/Base.h"
#include"../KazLibrary/Collision/CollisionManager.h"

struct IOperationObjectData
{
	short rockOnNum;		//�c�胍�b�N�I����
	bool enableToHitFlag;	//���肪�L���Ȃ̂��ǂ������f����t���O
	bool initFlag;			//���ɏ��������ʂ��������f����t���O


	IOperationObjectData()
	{
		rockOnNum = -1;
		initFlag = false;
		enableToHitFlag = false;
	}

	/// <summary>
	/// ����\OBJ������{�I�ȃf�[�^�̏�����
	/// </summary>
	/// <param name="MAX_ROCKON_NUM">���b�N�I���ł����</param>
	void Init(const short &MAX_ROCKON_NUM)
	{
		rockOnNum = MAX_ROCKON_NUM;
		enableToHitFlag = true;
		initFlag = true;
	};
};

/// <summary>
/// �v���C���[�����o����OBJ�̊��N���X
/// </summary>
class IOperationObject
{
public:
	virtual ~IOperationObject() {};

	/// <summary>
	/// ���b�N�I���̎c��񐔂����炵�܂�
	/// </summary>
	void Hit();

	/// <summary>
	/// �c�胍�b�N�I���񐔂�0�ȉ��Ȃ�true��Ԃ�
	/// </summary>
	bool LockedOrNot();

	/// <summary>
	/// �����蔻�肪�L���ŁA�c�胍�b�N�I������1�ȏ�Ȃ�true��Ԃ�
	/// </summary>
	bool AliveOrNot();

	IOperationObjectData iOperationData;//����\OBJ�S��������{�f�[�^


protected:
	const float limitLinePosZ = -200.0f;//�����蔻���L���ɂ��郉�C��


	/// <summary>
	/// �w���Z���W��菬�����ꏊ���s�����Ƃ�����false��Ԃ�
	/// </summary>
	/// <param name="POS_Z">�Ώۂ�Z���W</param>
	bool EnableToHit(float POS_Z)
	{
		return limitLinePosZ<= POS_Z;
	};

};
