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

	fbxRender[LEFT].data.handle = FbxModelResourceMgr::Instance()->LoadModel(KazFilePathName::PlayerPath + "CH_Right_Back_Anim.fbx");
	fbxRender[RIGHT].data.handle = FbxModelResourceMgr::Instance()->LoadModel(KazFilePathName::PlayerPath + "CH_Left_Back_Anim.fbx");
	fbxRender[HEAD].data.handle = FbxModelResourceMgr::Instance()->LoadModel(KazFilePathName::PlayerPath + "CH_Model_Head.fbx");

	for (int i = 0; i < fbxRender.size(); ++i)
	{
		fbxRender[i].data.pipelineName = PIPELINE_NAME_FBX_RENDERTARGET_TWO;
		fbxRender[i].data.stopAnimationFlag = true;
		fbxRender[i].data.transform.scale = { 0.5f,0.5f,0.5f };
	}


	totalTime = FbxModelResourceMgr::Instance()->GetResourceData(fbxRender[0].data.handle.handle)->endTime[0] - FbxModelResourceMgr::Instance()->GetResourceData(fbxRender[0].data.handle.handle)->startTime[0];


	float lScale = 0.5f;
	minScale = { lScale ,lScale ,lScale };
	sinTimer = 0.0f;

	adjPos = { 0.0f,0.9f,0.3f };
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


	//----------HP����������v���C���[��Ԃ�����----------
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
	//----------HP����������v���C���[��Ԃ�����----------

	damageEffect.Update();
	damageWindow.Update();

	render->data.color.color.a += 255 / 120;
	if (255 <= render->data.color.color.a)
	{
		render->data.color.color.a = 255;
	}


	//ImGui::Begin("Head");
	//KazImGuiHelper::InputVec3("Pos", &adjPos);
	//KazImGuiHelper::InputVec3("Rota", &adjRota);
	//ImGui::End();

	fbxRender[HEAD].data.transform.rotation = adjRota;

	for (int i = 0; i < fbxRender.size() - 1; ++i)
	{
		fbxRender[i].data.transform.pos = render->data.transform.pos + KazMath::Vec3<float>(0.0f, 1.0f + sinf(KazMath::PI_2F / 120.0f * sinTimer) * 0.2f, 0.0f);
	}

	fbxRender[HEAD].data.transform.pos = render->data.transform.pos + adjPos + KazMath::Vec3<float>(0.0f, 1.0f + sinf(KazMath::PI_2F / 120.0f * sinTimer) * 0.2f, 0.0f);

	++sinTimer;
	++larpTime;
	if (30 <= larpTime)
	{
		float lScale = 0.55f;
		for (int i = 0; i < fbxRender.size(); ++i)
		{
			fbxRender[i].data.transform.scale = { lScale,lScale,lScale + 0.1f };
		}
		larpTime = 0;
	}

	for (int i = 0; i < fbxRender.size(); ++i)
	{
		KazMath::Larp(minScale, &fbxRender[i].data.transform.scale, 0.1f);
	}


	//�v���C���[�̃A�j���[�V��������
	float lNowRate = forceCameraRate + cameraRate.x * 0.5f;

	//������
	if (signbit(lNowRate))
	{
		leftFlag = true;
		rightFlag = false;
	}
	//�E����
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
		fbxRender[LEFT].currentTime = lNowTime;

		adjPos.x = 0.0f + -cameraRate.x * 0.2f;

		adjRota.x = 50.0f * -cameraRate.y;
		adjRota.y = -52.0f * -cameraRate.x;
		adjRota.z = 0.0f;
	}
	else if (rightFlag)
	{
		float lTotalTime = static_cast<float>(totalTime.Get());
		FbxLongLong convertTime = static_cast<FbxLongLong>(lTotalTime * lNowRate);
		FbxTime lNowTime = convertTime;
		fbxRender[RIGHT].currentTime = lNowTime;

		adjRota.x = 0.0f;
		adjRota.y = -270.0f + 60.0f * cameraRate.x;
		adjRota.z = 15.0f + 75.0f * -cameraRate.y;
	}

}

void Player::Draw()
{
	//render->Draw();

	if (leftFlag)
	{
		fbxRender[LEFT].Draw();
	}
	else if (rightFlag)
	{
		fbxRender[RIGHT].Draw();
	}
	fbxRender[HEAD].Draw();

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
