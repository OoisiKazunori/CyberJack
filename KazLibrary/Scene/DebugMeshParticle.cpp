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


	triangelLine[0].data.startPos = triangelPosArray[0];
	triangelLine[0].data.endPos = triangelPosArray[1];
	triangelLine[1].data.startPos = triangelPosArray[1];
	triangelLine[1].data.endPos = triangelPosArray[2];
	triangelLine[2].data.startPos = triangelPosArray[2];
	triangelLine[2].data.endPos = triangelPosArray[0];


	//パーティクルの計算処理
	if (initFlag)
	{
		int particleNum = 0;
		for (int rayIndex = 0; rayIndex < triangelLine.size(); ++rayIndex)
		{
			//一つのレイ当たりで使用できるパーティクルを決定する
			const int L_MAX_PARTICLE = static_cast<int>(particle.size()) / static_cast<int>(triangelPosArray.size());

			KazMath::Vec3<float>lDistance = triangelLine[rayIndex].data.endPos - triangelLine[rayIndex].data.startPos;

			//以下の項目を計算し、配置する
			for (int i = 0; i < L_MAX_PARTICLE; ++i)
			{
				//1.レイに沿った座標を乱数で決定する
				float lRate = KazMath::Rand(1.0f, 0.0f);
				KazMath::Vec3<float>lStartPos = triangelLine[rayIndex].data.startPos + lDistance * lRate;

				//2.指定した座標から垂直にレイを飛ばす。レイは二方向に飛ばし、別のレイと当たった方向に伸ばす


				//3.1で決めた座標を始点、別のレイが当たった座標を終点とし、その長さを求める
				KazMath::Vec3<float>lResultDistance;



				//4.先程決めた長さの内、エッジ周辺にパーティクルを配置するか、長さの範囲で座標を決めるか乱数で決める

				//数が多いほどエッジ周辺に偏らせる
				const int L_RANDOM_NUMBER_BIAS = 80;

				//5-1.エッジ周辺にパーティクルを配置する場合は、長さを3割った値から乱数でどの場所に配置するか決める
				if (L_RANDOM_NUMBER_BIAS <= KazMath::Rand(100, 0))
				{
					particle[particleNum].data.transform.pos = lStartPos + (lResultDistance / 3.0) * KazMath::Rand(1.0f, 0.0f);
				}
				//5-2.長さの範囲で座標を決める場合は長さの値でどの場所に配置するか決める
				else
				{
					particle[particleNum].data.transform.pos = lStartPos + lResultDistance * KazMath::Rand(1.0f, 0.0f);
				}
				++particleNum;
			}
		}
		initFlag = false;
	}

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
