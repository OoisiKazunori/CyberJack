#include "GoalBox.h"
#include"../KazLibrary/Input/KeyBoradInputManager.h"
#include"../KazLibrary/Helper/ResourceFilePass.h"
#include"../KazLibrary/Imgui/MyImgui.h"

GoalBox::GoalBox()
{
	model = std::make_unique<ObjModelRender>();
	model->data.pipelineName = PIPELINE_NAME_OBJ_MULTITEX;
	iOperationData.Init(8);
	prevHpNum = iOperationData.rockOnNum;

	model->data.handle = ObjResourceMgr::Instance()->LoadModel(KazFilePathName::GoalPath + "goal.obj");

	lerpPos = { 10.0f,0.0f,10.0f };
	lerpRota = { 0.0f,0.0f,0.0f };
	model->data.transform.pos = lerpPos;
	model->data.transform.rotation = lerpRota;

	//model->data.transform.scale = { 10.0f,10.0f,10.0f };

	lerpMoveRotaVel = 0.1f;
	lerpMoveVel = 0.1f;
	moveRotaVel = { 200.0f,200.0f,200.0f };
	moveVel = { 0.0f,0.0f,15.0f };


	effect.Init(&model->data.motherMat);

	//std::shared_ptr<XMMATRIX>data = std::make_shared<XMMATRIX>(model->data.motherMat);
}

void GoalBox::Init()
{
	iOperationData.Init(8);
	prevHpNum = iOperationData.rockOnNum;


	lerpPos = { 0.0f,0.0f,0.0f };
	lerpRota = { 0.0f,0.0f,0.0f };
	model->data.transform.pos = lerpPos;
	model->data.transform.rotation = lerpRota;

	effect.Init(&model->data.motherMat);

}

void GoalBox::Update()
{

	if (KeyBoradInputManager::Instance()->InputTrigger(DIK_M))
	{
		Hit();
	}
	if (KeyBoradInputManager::Instance()->InputTrigger(DIK_N))
	{
		Init();
	}
	bool hitFlag = iOperationData.rockOnNum != prevHpNum;
	//���������ۂ̋���
	if (hitFlag)
	{
		//���ɔ��
		lerpPos.m128_f32[2] = model->data.transform.pos.m128_f32[2] + moveVel.m128_f32[2];
		//��]������
		lerpRota = model->data.transform.rotation + moveRotaVel;


		effect.Appear();

	}

	//���W�̃��[�v
	{
		XMVECTOR distance = lerpPos - model->data.transform.pos;
		model->data.transform.pos += distance * lerpMoveVel;
	}
	//��]�̃��[�v
	{
		XMVECTOR distance = lerpRota - model->data.transform.rotation;
		model->data.transform.rotation += distance * lerpMoveRotaVel;
	}
	prevHpNum = iOperationData.rockOnNum;


	effect.Update();



	ImGui::Begin("Goal");
	ImGui::InputFloat("RotaX", &moveRotaVel.m128_f32[0]);
	ImGui::InputFloat("RotaY", &moveRotaVel.m128_f32[1]);
	ImGui::InputFloat("RotaZ", &moveRotaVel.m128_f32[2]);
	ImGui::InputFloat("Z", &moveVel.m128_f32[2]);
	ImGui::InputFloat("MoveLerp", &lerpMoveVel);
	ImGui::InputFloat("MoveRotaLerp", &lerpMoveRotaVel);
	ImGui::End();
}

void GoalBox::Draw()
{
	model->Draw();
}
