#include "ClassScene.h"
#include"../Camera/CameraMgr.h"
#include"../Input/KeyBoradInputManager.h"
#include"../Math/KazMath.h"
#include"../Imgui/MyImgui.h"

ClassScene::ClassScene()
{
	boxRender = std::make_unique<BoxPolygonRender>();
	distanceRender = std::make_unique<LineRender>();
	landingPointRender = std::make_unique<BoxPolygonRender>();

	dirtyFlags[0] = std::make_unique<DirtySet>(initialVelocity);
	dirtyFlags[1] = std::make_unique<DirtySet>(valueOfAcceleration);
}

ClassScene::~ClassScene()
{
}

void ClassScene::Init()
{
	eyePos = { 61.9449921,18.0000000,-81.9450226 };
	targetPos = { 61.9449921,18.0000000,-76.9450226 };
	valueOfAcceleration = 1.0f;
	initialVelocity = 1.0f;
}

void ClassScene::Finalize()
{
}

void ClassScene::Input()
{

	KeyBoradInputManager *input = KeyBoradInputManager::Instance();
	//	ControllerInputManager *inputController = ControllerInputManager::Instance();

#pragma region カメラ操作
	debugCameraMove = { 0,0,0 };
	float debugSpeed = 1;
	//�J�����ړ�
	if (input->InputState(DIK_D))
	{
		debugCameraMove.x = -debugSpeed;
	}
	if (input->InputState(DIK_A))
	{
		debugCameraMove.x = debugSpeed;
	}
	if (input->InputState(DIK_W))
	{
		//y�Ȃ̂ɑO�ɐi��....
		debugCameraMove.y = debugSpeed;
	}
	if (input->InputState(DIK_S))
	{
		debugCameraMove.y = -debugSpeed;
	}


	//�J�����p�x
	if (input->InputState(DIK_RIGHTARROW))
	{
		angle.x += debugSpeed;
	}
	if (input->InputState(DIK_LEFTARROW))
	{
		angle.x += -debugSpeed;
	}

	if (input->InputState(DIK_UPARROW))
	{
		angle.y += debugSpeed;
	}
	if (input->InputState(DIK_DOWNARROW))
	{
		angle.y += -debugSpeed;
	}

#pragma endregion
}

void ClassScene::Update()
{

	ImGui::Begin("MT4_01");
	ImGui::Checkbox("Start", &startFlag);
	ImGui::SliderFloat("Acceleration", &valueOfAcceleration, 1.0f, 20.0f);
	ImGui::SliderFloat("initialVelocity", &initialVelocity, 1.0f, 10.0f);
	ImGui::Text("Result");
	ImGui::Text("ReachingDistance:%f", landingPointRender->data.transform.pos.m128_f32[0]);
	ImGui::Text("CurrentTime:%f", time);
	ImGui::Text("ArrivalTime:%f", landingTime);
	ImGui::End();

	if (dirtyFlags[0]->FloatDirty() || dirtyFlags[1]->FloatDirty())
	{
		startFlag = false;
	}

	CameraMgr::Instance()->Camera(eyePos, targetPos, { 0.0f,1.0f,0.0f });


	//開始
	if (startFlag && time < landingTime - 0.1f)
	{
		time += landingTime / 60.0f;
	}
	//終了
	else if (!startFlag)
	{
		time = 0.0f;
	}
	//着地
	else
	{
		time = landingTime;
	}



	//水平方向で使う情報
	caluDistance = KazPhysics::CalucurateHorizontalDistance2(valueOfAcceleration, time, initialVelocity);
	//垂直方向で使う情報
	caluDistance2 = KazPhysics::CalucurateVerticalDistance(valueOfAcceleration, time, initialVelocity);


	
	XMFLOAT3 basePos(0.0f, 0.0f, 0.0f);
	//n秒後の位置-----------------------
	boxRender->data.transform.pos =
	{
		basePos.x + static_cast<float>(0.0 + caluDistance),
		basePos.y + static_cast<float>(0.0 + caluDistance2),
		basePos.z
	};
	//n秒後の位置-----------------------


	//開始地点-----------------------
	distanceRender->data.color = { 255.0f,0.0f,0.0f,255.0f };
	distanceRender->data.startPos =
	{
		basePos.x,
		basePos.y,
		basePos.z
	};
	//開始地点-----------------------


	//終点の計算-----------------------

	int timer = 0;
	int yCount = 0;
	XMFLOAT2 distance;
	while (1)
	{
		++timer;

		//水平方向で使う情報
		distance.x = KazPhysics::CalucurateHorizontalDistance2(valueOfAcceleration, timer, initialVelocity);
		//垂直方向で使う情報
		distance.y = KazPhysics::CalucurateVerticalDistance(valueOfAcceleration, timer, initialVelocity);

		if (distance.y <= 0.0f)
		{
			landingTime = timer;
			break;
		}
	}
	//終点の計算-----------------------

	//終点の設定-----------------------
	distanceRender->data.endPos =
	{
		basePos.x + static_cast<float>(distance.x),
		basePos.y,
		basePos.z
	};
	landingPointRender->data.transform.pos = distanceRender->data.endPos;
	//終点の設定-----------------------


	dirtyFlags[0]->Record();
	dirtyFlags[1]->Record();
}

void ClassScene::Draw()
{
	bg.Draw();
	boxRender->Draw();
	distanceRender->Draw();
	landingPointRender->Draw();
}

int ClassScene::SceneChange()
{
	return SCENE_NONE;
}
