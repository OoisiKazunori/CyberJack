#include "IOperationObject.h"
#include"../KazLibrary/Helper/ResourceFilePass.h"

IOperationObject::IOperationObject()
{
	lockOnWindowRender.data.handleData = TextureResourceMgr::Instance()->LoadGraph(KazFilePathName::LinePath + "LockOn.png");
	lockOnWindowRender.data.transform.scale = { 0.2f,0.2f };
	//lockOnWindowRender.data.billBoardFlag = true;
	lockOnWindowRender.data.pipelineName = PIPELINE_NAME_SPRITE_Z_ALWAYS;
}

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
	//ˆê’èŽžŠÔ‚ÌŠÔŠu‚ÅŽw’è‚µ‚½‰ñ”•ª‘Ì—Í‚ðŒ¸‚ç‚·
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
	//‰ñ”•ªI‚¦‚½‚çI—¹‚·‚é
	if (reduceHpNum <= 0)
	{
		startToReduceHpFlag = false;
		return 2;
	}
	return 0;
}

void IOperationObject::LockOnWindow(const KazMath::Vec3<float> &POS)
{
	if (iOperationData.lockOnFlag && iOperationData.enableToHitFlag)
	{
		KazMath::Vec3<float>screenPos =
			KazMath::ConvertWorldPosToScreenPos(POS, CameraMgr::Instance()->GetViewMatrix(0), CameraMgr::Instance()->GetPerspectiveMatProjection());

		lockOnWindowRender.data.transform.pos = KazMath::Vec2<float>(screenPos.x, screenPos.y);
		lockOnWindowRender.Draw();
	}
}
