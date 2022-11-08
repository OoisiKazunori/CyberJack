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
	rendertarget = std::make_unique<GameRenderTarget>(KazMath::Color(0, 0, 0, 255));
	mainRenderTarget.data.handleData = rendertarget->GetGameRenderTargetHandle();
	mainRenderTarget.data.pipelineName = PIPELINE_NAME_SPRITE_NOBLEND;
	mainRenderTarget.data.transform.pos = { WIN_X / 2.0f,WIN_Y / 2.0f };

	for (int i = 0; i < meshEmitter.size(); ++i)
	{
		meshEmitter[i] = std::make_unique<MeshParticleEmitter>(5);
	}
}

DebugMeshParticleScene::~DebugMeshParticleScene()
{
}

void DebugMeshParticleScene::Init()
{
	cameraWork.Init();

	triangelPosArray[0] = { -30.0f,0.0f,0.0f };
	triangelPosArray[1] = { 30.0f,0.0f,0.0f };
	triangelPosArray[2] = { 0.0f,30.0f,0.0f };


	for (int i = 0; i < meshEmitter.size(); ++i)
	{
		meshEmitter[i]->Init(&motherMat);
	}
}

void DebugMeshParticleScene::Finalize()
{
	rendertarget->Finalize();
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


	//�f�o�b�N�p��GUI
	ImGui::Begin("MeshParticle");
	ImGui::Checkbox("CheckCPUParticle", &cpuCheckParticleFlag);
	ImGui::Checkbox("CheckGPUParticle", &gpuCheckParticleFlag);
	if (cpuCheckParticleFlag)
	{
		if (ImGui::TreeNode("TrianglePosArray"))
		{
			KazImGuiHelper::InputVec3("Pos1", &triangelPosArray[0]);
			KazImGuiHelper::InputVec3("Pos2", &triangelPosArray[1]);
			KazImGuiHelper::InputVec3("Pos3", &triangelPosArray[2]);
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Particle"))
		{
			ImGui::SliderInt("ParticleNum", &drawParticleNum, 0, PARTICLE_MAX_NUM);
			ImGui::SliderInt("ParticleBias", &particleBias, 0, PARTICLE_MAX_BIAS);
			ImGui::TreePop();
		}
		initFlag = ImGui::Button("SetParticle");
	}
	else if (gpuCheckParticleFlag)
	{
		ImGui::SliderInt("EnemyIndex", &meshIndex, 5, 7);
		KazImGuiHelper::InputTransform3D("Mother", &motherTransform);
		if (meshIndex != prevMeshIndex)
		{
			//meshEmitter.reset();
			//meshEmitter = std::make_unique<MeshParticleEmitter>(meshIndex);
			//meshEmitter->Init(&motherMat);
			prevMeshIndex = meshIndex;
		}
	}
	ImGui::End();


	if (cpuCheckParticleFlag)
	{
		triangelLine[0].data.startPos = triangelPosArray[0];
		triangelLine[0].data.endPos = triangelPosArray[1];
		triangelLine[1].data.startPos = triangelPosArray[1];
		triangelLine[1].data.endPos = triangelPosArray[2];
		triangelLine[2].data.startPos = triangelPosArray[2];
		triangelLine[2].data.endPos = triangelPosArray[0];

		KazMath::Vec3<float>lCentralPos = {};
		//�@���x�N�g���̎Z�o
		for (int i = 0; i < triangelPosArray.size(); ++i)
		{
			lCentralPos += triangelPosArray[i];

			clossTriangelLine[i].data.startPos = triangelLine[i].data.startPos + (triangelLine[i].data.endPos - triangelLine[i].data.startPos) / 2.0f;
			clossTriangelLine[i].data.colorData.color = { 255,0,0,255 };
		}
		KazMath::Vec3<float>lTriangleCentralPos = lCentralPos / 3.0f;
		for (int i = 0; i < clossTriangelLine.size(); ++i)
		{
			clossTriangelLine[i].data.endPos = lTriangleCentralPos;
		}


		//�p�[�e�B�N���̌v�Z����
		if (initFlag)
		{
			int particleNum = 0;

			//��̃��C������Ŏg�p�ł���p�[�e�B�N�������肷��
			const int L_MAX_PARTICLE = static_cast<int>(particle.size()) / static_cast<int>(triangelPosArray.size());
			for (int rayIndex = 0; rayIndex < triangelLine.size(); ++rayIndex)
			{
				KazMath::Vec3<float>lDistance = triangelLine[rayIndex].data.endPos - triangelLine[rayIndex].data.startPos;

				//�ȉ��̍��ڂ��v�Z���A�z�u����
				for (int i = 0; i < L_MAX_PARTICLE; ++i)
				{
					//1.���C�ɉ��������W�𗐐��Ō��肷��
					float lRate = KazMath::Rand(1.0f, 0.0f);
					KazMath::Vec3<float>lStartPos = triangelLine[rayIndex].data.startPos + lDistance * lRate;

					//2.1�Ō��߂����W���n�_�A�d�S���W���I�_�Ƃ��A���̒��������߂�
					KazMath::Vec3<float>lResultDistance = lTriangleCentralPos - lStartPos;


					//���������قǃG�b�W���ӂɕ΂点��
					const int L_RANDOM_NUMBER_BIAS = particleBias;

					//3.������߂������̓��A�G�b�W���ӂɃp�[�e�B�N����z�u���邩�A�����͈̔͂ō��W�����߂邩�����Ō��߂�
					if (KazMath::Rand(PARTICLE_MAX_BIAS, 0) <= L_RANDOM_NUMBER_BIAS)
					{
						//4-1.�G�b�W���ӂɃp�[�e�B�N����z�u����ꍇ�́A���������l�������l���痐���łǂ̏ꏊ�ɔz�u���邩���߂�
						particle[particleNum].data.transform.pos = lStartPos + lResultDistance * KazMath::Rand(0.1f, 0.0f);
					}
					else
					{
						//4-2.�����͈̔͂ō��W�����߂�ꍇ�͒����̒l�łǂ̏ꏊ�ɔz�u���邩���߂�
						particle[particleNum].data.transform.pos = lStartPos + lResultDistance * KazMath::Rand(1.0f, 0.0f);
					}
					particle[particleNum].data.transform.scale = { 0.1f,0.1f,0.1f };
					++particleNum;
				}
			}

			for (int i = particleNum; i < particle.size(); ++i)
			{
				particle[i].data.transform.scale = { 0.01f,0.01f,0.01f };
			}

			initFlag = false;
		}
	}
	else if (gpuCheckParticleFlag)
	{
		motherMat = KazMath::CaluWorld(motherTransform, { 0,1,0 }, { 0,0,1 });

		for (int i = 0; i < meshEmitter.size(); ++i)
		{
			meshEmitter[i]->Update();
		}
	}
}

void DebugMeshParticleScene::Draw()
{
	RenderTargetStatus::Instance()->SetDoubleBufferFlame();
	RenderTargetStatus::Instance()->ClearDoubuleBuffer({ 0.0f,0.0f,0.0f });

	rendertarget->SetRenderTarget();

	if (cpuCheckParticleFlag)
	{
		//�O�p�`�`��
		for (int i = 0; i < triangelLine.size(); ++i)
		{
			triangelLine[i].Draw();
		}

		for (int i = 0; i < drawParticleNum; ++i)
		{
			particle[i].Draw();
		}
		for (int i = 0; i < clossTriangelLine.size(); ++i)
		{
			clossTriangelLine[i].Draw();
		}
	}
	else if (gpuCheckParticleFlag)
	{
		for (int i = 0; i < meshEmitter.size(); ++i)
		{
			meshEmitter[i]->Draw();
		}
	}

	rendertarget->Draw();
	//debugDraw.Draw();
	mainRenderTarget.Draw();
}

int DebugMeshParticleScene::SceneChange()
{
	if (KeyBoradInputManager::Instance()->InputTrigger(DIK_0))
	{
		return 0;
	}
	else
	{
		return SCENE_NONE;
	}
}
