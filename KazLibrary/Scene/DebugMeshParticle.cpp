#include "DebugMeshParticle.h"
#include"../Camera/CameraMgr.h"
#include"../Input/KeyBoradInputManager.h"
#include"../Loader/FbxModelResourceMgr.h"
#include"../Helper/ResourceFilePass.h"
#include"../RenderTarget/RenderTargetStatus.h"
#include"../KazLibrary/Imgui/MyImgui.h"
#include"../Game/Stage/BlockParticleStage.h"
#include"../Game/Stage/FirstStage.h"
#include"../Game/Stage/RezStage.h"


#include"../KazLibrary/Input/ControllerInputManager.h"

DebugMeshParticleScene::DebugMeshParticleScene()
{
}

DebugMeshParticleScene::~DebugMeshParticleScene()
{
}

void DebugMeshParticleScene::Init()
{
	cameraWork.Init();
}

void DebugMeshParticleScene::Finalize()
{
}

void DebugMeshParticleScene::Input()
{
}

void DebugMeshParticleScene::Update()
{
	eyePos = cameraWork.GetEyePos();
	targetPos = cameraWork.GetTargetPos();
	CameraMgr::Instance()->CameraSetting(60.0f, 100000.0f, 0);
	CameraMgr::Instance()->Camera(eyePos, targetPos, { 0.0f,1.0f,0.0f });

	KazMath::Vec3<float>lPl(0.0f, 0.0f, 0.0f);
	cameraWork.Update(KazMath::Vec2<float>(0.0f, 0.0f), &lPl, true);


	//デバック用のGUI
	ImGui::Begin("MeshParticle");
	if (ImGui::TreeNode("TrianglePosArray"))
	{
		KazImGuiHelper::InputVec3("Pos1", &triangelPosArray[0]);
		KazImGuiHelper::InputVec3("Pos2", &triangelPosArray[1]);
		KazImGuiHelper::InputVec3("Pos3", &triangelPosArray[2]);
		ImGui::TreePop();
	}
	initFlag = ImGui::Button("SetParticle");
	ImGui::SliderInt("ParticleNum", &drawParticleNum, 0, PARTICLE_MAX_NUM);
	ImGui::End();

	//パーティクルの計算処理
	if (initFlag)
	{


		for (int i = 0; i < particle.size(); ++i)
		{
		}
		initFlag = false;
	}


	triangelLine[0].data.startPos = triangelPosArray[0];
	triangelLine[0].data.endPos = triangelPosArray[1];
	triangelLine[1].data.startPos = triangelPosArray[1];
	triangelLine[1].data.endPos = triangelPosArray[2];
	triangelLine[2].data.startPos = triangelPosArray[2];
	triangelLine[2].data.endPos = triangelPosArray[0];


}

void DebugMeshParticleScene::Draw()
{
	RenderTargetStatus::Instance()->SetDoubleBufferFlame();
	RenderTargetStatus::Instance()->ClearDoubuleBuffer(BG_COLOR);

	//三角形描画
	for (int i = 0; i < triangelLine.size(); ++i)
	{
		triangelLine[i].Draw();
	}

	for (int i = 0; i < drawParticleNum; ++i)
	{
		particle[i].Draw();
	}

	RenderTargetStatus::Instance()->SwapResourceBarrier();

}

int DebugMeshParticleScene::SceneChange()
{
	return SCENE_NONE;
}
