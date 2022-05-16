#include "IOperationObject.h"

void IOperationObject::Hit()
{
	--iOperationData.rockOnNum;
}

bool IOperationObject::LockedOrNot()
{
	return iOperationData.rockOnNum <= 0;
}

bool IOperationObject::IsAlive()
{
	return iOperationData.enableToHitFlag && !LockedOrNot();
}

int IOperationObject::ReduceHp(int COUNT)
{
	if (!startToReduceHpFlag)
	{
		reduceHpNum = COUNT;
		startToReduceHpFlag = true;
	}
	//��莞�Ԃ̊Ԋu�Ŏw�肵���񐔕��̗͂����炷
	if (startToReduceHpFlag)
	{
		++timer;
		if (5 <= timer)
		{
			--iOperationData.hp;
			--reduceHpNum;
			timer = 0;
			return 1;
		}
	}
	//�񐔕��I������I������
	if (reduceHpNum <= 0)
	{
		startToReduceHpFlag = false;
		return 2;
	}
	return 0;
}