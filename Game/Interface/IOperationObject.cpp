#include "IOperationObject.h"

void IOperationObject::Hit()
{
	--iOperationData.rockOnNum;
}

bool IOperationObject::LockedOrNot()
{
	return iOperationData.rockOnNum <= 0;
}

bool IOperationObject::AliveOrNot()
{
	return iOperationData.enableToHitFlag && !LockedOrNot();
};