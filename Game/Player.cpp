#include "Player.h"
#include"../KazLibrary/Imgui/MyImgui.h"
#include"../KazLibrary/Sound/SoundManager.h"
#include"../KazLibrary/Helper/ResourceFilePass.h"

const int Player::COOL_MAX_TIME = 120;

Player::Player()
{
	render = std::make_unique<BoxPolygonRender>();
	render->data.pipelineName = PIPELINE_NAME_COLOR_MULTITEX;
	hp = -1;
	pos = {};

	damageSoundHandle = SoundManager::Instance()->LoadSoundMem(KazFilePathName::SoundPath + "PlayerDamage.wav");

	{
		RESOURCE_HANDLE lHandle = render->CreateConstBuffer(sizeof(DirectX::XMFLOAT4), typeid(DirectX::XMFLOAT4).name(), GRAPHICS_RANGE_TYPE_CBV, GRAPHICS_PRAMTYPE_DATA);
		DirectX::XMFLOAT4 lColor = { 0.0f,0.0f,0.0f,0.0f };
		render->TransData(&lColor, lHandle, typeid(DirectX::XMFLOAT4).name());
	}

	lRender.data.handle = FbxModelResourceMgr::Instance()->LoadModel(KazFilePathName::PlayerPath + "CH_Right_Back_Anim.fbx");
	rRender.data.handle = FbxModelResourceMgr::Instance()->LoadModel(KazFilePathName::PlayerPath + "CH_Left_Back_Anim.fbx");

	lRender.data.pipelineName = PIPELINE_NAME_FBX_RENDERTARGET_TWO_LIGHT;
	lRender.data.stopAnimationFlag = true;
	rRender.data.pipelineName = PIPELINE_NAME_FBX_RENDERTARGET_TWO_LIGHT;
	rRender.data.stopAnimationFlag = true;

	lRender.data.transform.scale = { 0.5f,0.5f,0.5f };
	rRender.data.transform.scale = { 0.5f,0.5f,0.5f };

	{
		RESOURCE_HANDLE lHandle = lRender.CreateConstBuffer(sizeof(DirectX::XMFLOAT3), typeid(DirectX::XMFLOAT3).name(), GRAPHICS_RANGE_TYPE_CBV, GRAPHICS_PRAMTYPE_DATA3);
		DirectX::XMFLOAT3 dir = { 0.0f,0.0f,1.0f };
		lRender.TransData(&dir, lHandle, typeid(DirectX::XMFLOAT3).name());
	}

	{
		RESOURCE_HANDLE lHandle = rRender.CreateConstBuffer(sizeof(DirectX::XMFLOAT3), typeid(DirectX::XMFLOAT3).name(), GRAPHICS_RANGE_TYPE_CBV, GRAPHICS_PRAMTYPE_DATA3);
		DirectX::XMFLOAT3 dir = { 0.0f,0.0f,1.0f };
		rRender.TransData(&dir, lHandle, typeid(DirectX::XMFLOAT3).name());
	}

	totalTime = FbxModelResourceMgr::Instance()->GetResourceData(lRender.data.handle.handle)->endTime[0] - FbxModelResourceMgr::Instance()->GetResourceData(lRender.data.handle.handle)->startTime[0];


	float lScale = 0.5f;
	minScale = { lScale ,lScale ,lScale };
	sinTimer = 0.0f;
}

void Player::Init(const KazMath::Vec3<float> &POS, bool DRAW_UI_FLAG, bool APPEAR_FLAG)
{
	pos = POS;
	render->data.transform.pos = pos;
	render->data.transform.scale = { 0.5f,1.3f,0.5f };
	render->data.transform.rotation = { 35.0f,0.0f,1.0f };
	render->data.color.color = { 255,255,255,0 };
	if (APPEAR_FLAG)
	{
		render->data.color.color.a = 0;
	}
	else
	{
		render->data.color.color.a = 255;
	}
	hp = 3;

	hpUi.Init(hp);
	prevHp = hp;
	redFlag = false;
	coolTimer = 0;
	coolTimeFlag = false;

	drawHpFlag = DRAW_UI_FLAG;

	larpTime = 0;
}

void Player::Finalize()
{
	damageWindow.Finalize();
}

void Player::Input()
{
}

void Player::Update()
{
	render->data.transform.pos = pos;
	hpUi.Update();


	//----------HPが減ったらプレイヤーを赤くする----------
	if (hp != prevHp)
	{
		SoundManager::Instance()->PlaySoundMem(damageSoundHandle, 1);
		damageEffect.Init(pos);
		damageWindow.Init(pos + KazMath::Vec3<float>(WIN_X / 2.0f, WIN_Y / 2.0f + 200.0f, 0.0f));
		redFlag = true;
	}
	prevHp = hp;

	if (redFlag)
	{
		++coolTimer;

		if (COOL_MAX_TIME <= coolTimer)
		{
			redFlag = false;
			coolTimeFlag = false;
		}
	}
	else
	{
		coolTimer = 0;
	}

	if (redFlag)
	{
		render->data.color.color.x = 255;
		render->data.color.color.y = 0;
		render->data.color.color.z = 0;
	}
	else
	{
		render->data.color.color.x = 255;
		render->data.color.color.y = 255;
		render->data.color.color.z = 255;
	}
	//----------HPが減ったらプレイヤーを赤くする----------

	damageEffect.Update();
	damageWindow.Update();

	render->data.color.color.a += 255 / 120;
	if (255 <= render->data.color.color.a)
	{
		render->data.color.color.a = 255;
	}

	lRender.data.transform.pos = render->data.transform.pos + KazMath::Vec3<float>(0.0f, 1.0f+sinf(KazMath::PI_2F / 120.0f * sinTimer) * 0.2f, 0.0f);
	rRender.data.transform.pos = render->data.transform.pos + KazMath::Vec3<float>(0.0f, 1.0f+sinf(KazMath::PI_2F / 120.0f * sinTimer) * 0.2f, 0.0f);

	++sinTimer;
	++larpTime;
	if (30 <= larpTime)
	{
		float lScale = 0.55f;
		lRender.data.transform.scale = { lScale,lScale,lScale + 0.1f };
		rRender.data.transform.scale = { lScale,lScale,lScale + 0.1f };
		larpTime = 0;
	}


	KazMath::Larp(minScale, &lRender.data.transform.scale, 0.1f);
	KazMath::Larp(minScale, &rRender.data.transform.scale, 0.1f);


	//プレイヤーのアニメーション制御
	float lNowRate = cameraRate.x * 0.5f;

	//左向き
	if (signbit(lNowRate))
	{
		leftFlag = true;
		rightFlag = false;
	}
	//右向き
	else
	{
		leftFlag = false;
		rightFlag = true;
	}


	if (leftFlag)
	{
		float lTotalTime = static_cast<float>(totalTime.Get());
		FbxLongLong convertTime = static_cast<FbxLongLong>(lTotalTime * -lNowRate);
		FbxTime lNowTime = convertTime;
		lRender.currentTime = lNowTime;
	}
	else if (rightFlag)
	{
		float lTotalTime = static_cast<float>(totalTime.Get());
		FbxLongLong convertTime = static_cast<FbxLongLong>(lTotalTime * lNowRate);
		FbxTime lNowTime = convertTime;
		rRender.currentTime = lNowTime;
	}

}

void Player::Draw()
{
	//render->Draw();

	if (leftFlag)
	{
		lRender.Draw();
	}
	else if (rightFlag)
	{
		rRender.Draw();
	}


	damageEffect.Draw();
	damageWindow.Draw();

	if (drawHpFlag)
	{
		hpUi.Draw();
	}
}

void Player::Hit()
{
	if (!coolTimeFlag)
	{
		--hp;
		hpUi.Sub();
	}
	coolTimeFlag = true;
}

bool Player::IsAlive()
{
	return 0 < hp;
}
