#pragma once
#include"../KazLibrary/DirectXCommon/Base.h"
#include"../KazLibrary/Collision/CollisionManager.h"
class IOperationObject
{
public:
	virtual ~IOperationObject() {};

	void Hit()
	{
		--iOprationObject_rockOnNum;
	};

	short iOprationObject_rockOnNum;
	Sphere iOprationObject_hitBox;
};
