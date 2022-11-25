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
#include<cmath>
#include<iostream>
#include<vector>

#include"../KazLibrary/Input/ControllerInputManager.h"

void AddVert(std::vector<VertexUv> &DATA, std::array<Vertex, 4>VERT_DATA, std::array<KazMath::Vec2<float>, 4>TEX_VERT_DATA, int INDEX)
{
	VertexUv lData;
	lData.pos = { TEX_VERT_DATA[INDEX].ConvertXMFLOAT2().x,TEX_VERT_DATA[INDEX].ConvertXMFLOAT2().y,0.0f };
	lData.uv = VERT_DATA[INDEX].uv;
	DATA.push_back(lData);
};

DebugMeshParticleScene::DebugMeshParticleScene()
{
	rendertarget = std::make_unique<GameRenderTarget>(KazMath::Color(0, 0, 0, 255));
	mainRenderTarget.data.handleData = rendertarget->GetGameRenderTargetHandle();
	mainRenderTarget.data.pipelineName = PIPELINE_NAME_SPRITE_NOBLEND;
	mainRenderTarget.data.transform.pos = { WIN_X / 2.0f,WIN_Y / 2.0f };


	filePass[0] = KazFilePathName::EnemyPath + "BattleShip/" + "BattleshipEnemy_Head_anim.fbx";
	filePass[1] = KazFilePathName::EnemyPath + "Bike/" + "BikeEnemy_anim.fbx";
	filePass[2] = KazFilePathName::EnemyPath + "MisileEnemy/" + "Gunner_Switch_anim.fbx";
	filePass[3] = KazFilePathName::EnemyPath + "Summon/" + "SummonEnemy_anim.fbx";
	filePass[4] = KazFilePathName::EnemyPath + "Move/" + "MoveEnemy_Model.obj";
	filePass[5] = KazFilePathName::EnemyPath + "PopEnemy/" + "PopEnemy_Model.obj";

	for (int i = 0; i < meshEmitter.size(); ++i)
	{
		std::string lPass;
		lPass += filePass[i][filePass[i].size() - 3];
		lPass += filePass[i][filePass[i].size() - 2];
		lPass += filePass[i][filePass[i].size() - 1];

		if (lPass == "obj")
		{
			RESOURCE_HANDLE lHandle = ObjResourceMgr::Instance()->LoadModel(filePass[i]);
			meshEmitter[i] = std::make_unique<MeshParticleEmitter>(ObjResourceMgr::Instance()->GetResourceData(lHandle).vertices);
		}
		else
		{
			RESOURCE_HANDLE lHandle = FbxModelResourceMgr::Instance()->LoadModel(filePass[i]);
			meshEmitter[i] = std::make_unique<MeshParticleEmitter>(FbxModelResourceMgr::Instance()->GetResourceData(lHandle)->vertData);
		}
	}
	meshIndex = 0;
	model.data.handle = FbxModelResourceMgr::Instance()->LoadModel(filePass[meshIndex]);
	model.data.pipelineName = PIPELINE_NAME_COLOR_WIREFLAME;
	model.data.removeMaterialFlag = true;
	model.data.colorData.color = { 255,255,255,255 };
	model.data.transform.scale = { 1.0f,1.0f,1.0f };


	std::array<Vertex, 4>vertices;
	std::array<USHORT, 6> indices;
	indices = KazRenderHelper::InitIndciesForPlanePolygon();
	KazRenderHelper::InitVerticesPos(&vertices[0].pos, &vertices[1].pos, &vertices[2].pos, &vertices[3].pos, { 0.5f,0.5f });
	KazRenderHelper::InitUvPos(&vertices[0].uv, &vertices[1].uv, &vertices[2].uv, &vertices[3].uv);

	handle = TextureResourceMgr::Instance()->LoadGraph(KazFilePathName::TestPath + "tex.png");

	KazMath::Vec2<int>lTexSize
	(
		static_cast<int>(TextureResourceMgr::Instance()->GetTextureSize(handle).Width),
		static_cast<int>(TextureResourceMgr::Instance()->GetTextureSize(handle).Height))
		;

	KazMath::Vec2<float> lLeftUp, lRightDown;
	lLeftUp = { 0.0f,0.0f };
	lRightDown = { 1.0f,1.0f };
	//サイズ変更
	std::array<KazMath::Vec2<float>, 4>lVert;
	lVert = KazRenderHelper::ChangePlaneScale(lLeftUp, lRightDown, { 1.0f,1.0f }, { 0.5f,0.5f }, lTexSize);


	std::vector<VertexUv>lVertArray;
	AddVert(lVertArray, vertices, lVert, 0);
	AddVert(lVertArray, vertices, lVert, 1);
	AddVert(lVertArray, vertices, lVert, 2);//問題の箇所
	AddVert(lVertArray, vertices, lVert, 2);
	AddVert(lVertArray, vertices, lVert, 1);
	AddVert(lVertArray, vertices, lVert, 3);//問題の箇所

	//重複ありの三角形
	texParticle = std::make_unique<TextureParticle>(lVertArray, 1.0f);
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

	prevDeadParticleFlag = false;
	deadParticleFlag = false;

	texParticle->Init();
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


	//デバック用のGUI
	ImGui::Begin("MeshParticle");
	ImGui::Checkbox("CheckCPUParticle", &cpuCheckParticleFlag);
	ImGui::Checkbox("CheckGPUParticle", &gpuCheckParticleFlag);
	ImGui::Checkbox("CheckPerlinNoise", &perlinNoizeFlag);
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
		ImGui::SliderInt("EnemyIndex", &meshIndex, 0, MESH_MAX_NUM - 1);
		KazImGuiHelper::InputTransform3D("Mother", &model.data.transform);
		if (meshIndex != prevMeshIndex)
		{
			model.data.handle = FbxModelResourceMgr::Instance()->LoadModel(filePass[meshIndex]);
			prevMeshIndex = meshIndex;
		}
		ImGui::SliderInt("FlameAlpha", &model.data.colorData.color.a, 0, 255);
		ImGui::Checkbox("Dead", &deadParticleFlag);
		if (deadParticleFlag != prevDeadParticleFlag)
		{
			deadParticle.reset();
			deadParticle = std::make_unique<DeadParticle>(meshEmitter[meshIndex]->GetAddress(), meshEmitter[meshIndex]->GetVertNum());
			prevDeadParticleFlag = deadParticleFlag;
		}

	}
	else if (perlinNoizeFlag)
	{
		initNoizeFlag = ImGui::Button("Init");
		ImGui::SliderFloat("UV_X", &uv.x, 0, 10);
		ImGui::SliderFloat("UV_Y", &uv.y, 0, 10);
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
		//法線ベクトルの算出
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


		//パーティクルの計算処理
		if (initFlag)
		{
			int particleNum = 0;

			float triangleArea = CalucurateTriangleArea(triangelLine[0].data.startPos, triangelLine[1].data.startPos, triangelLine[2].data.startPos);


			//一つのレイ当たりで使用できるパーティクルを決定する
			const int L_MAX_PARTICLE = static_cast<int>(particle.size()) / static_cast<int>(triangelPosArray.size());
			for (int rayIndex = 0; rayIndex < triangelLine.size(); ++rayIndex)
			{
				KazMath::Vec3<float>lDistance = triangelLine[rayIndex].data.endPos - triangelLine[rayIndex].data.startPos;

				//以下の項目を計算し、配置する
				for (int i = 0; i < L_MAX_PARTICLE; ++i)
				{
					//1.レイに沿った座標を乱数で決定する
					float lRate = KazMath::Rand(1.0f, 0.0f);
					KazMath::Vec3<float>lStartPos = triangelLine[rayIndex].data.startPos + lDistance * lRate;

					//2.1で決めた座標を始点、重心座標を終点とし、その長さを求める
					KazMath::Vec3<float>lResultDistance = lTriangleCentralPos - lStartPos;


					//数が多いほどエッジ周辺に偏らせる
					const int L_RANDOM_NUMBER_BIAS = particleBias;

					//3.先程決めた長さの内、エッジ周辺にパーティクルを配置するか、長さの範囲で座標を決めるか乱数で決める
					if (KazMath::Rand(PARTICLE_MAX_BIAS, 0) <= L_RANDOM_NUMBER_BIAS)
					{
						//4-1.エッジ周辺にパーティクルを配置する場合は、長さを一定値割った値から乱数でどの場所に配置するか決める
						particle[particleNum].data.transform.pos = lStartPos + lResultDistance * KazMath::Rand(0.1f, 0.0f);
					}
					else
					{
						//4-2.長さの範囲で座標を決める場合は長さの値でどの場所に配置するか決める
						particle[particleNum].data.transform.pos = lStartPos + lResultDistance * KazMath::Rand(1.0f, 0.0f);
					}

					//UV--------------------
					float u = CalucurateUVW(triangelLine[0].data.startPos, triangelLine[0].data.endPos, particle[particleNum].data.transform.pos, triangleArea);
					float v = CalucurateUVW(triangelLine[1].data.startPos, triangelLine[1].data.endPos, particle[particleNum].data.transform.pos, triangleArea);
					float w = CalucurateUVW(triangelLine[2].data.startPos, triangelLine[2].data.endPos, particle[particleNum].data.transform.pos, triangleArea);

					KazMath::Vec3<float> uvw;
					KazMath::Vec2<float> uVec = KazMath::Vec2<float>(0.0f, 0.0f) * u;
					KazMath::Vec2<float> vVec = KazMath::Vec2<float>(0.0f, 1.0f) * v;
					KazMath::Vec2<float> wVec = KazMath::Vec2<float>(1.0f, 0.0f) * w;
					//UV--------------------


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
		motherMat = model.motherMat;

		if (!deadParticleFlag)
		{
			meshEmitter[meshIndex]->Update();
		}
		else
		{
			deadParticle->Update();
		}

	}
	else if (perlinNoizeFlag)
	{
		if (initNoizeFlag)
		{
			for (int x = 0; x < perlinDebugBox.size(); ++x)
			{
				for (int y = 0; y < perlinDebugBox[x].size(); ++y)
				{
					float lX = static_cast<float>(x);
					float lZ = static_cast<float>(y);
					float lY = PerlinNoize({ lX / 10.0f,lZ / 10.0f }, 1) * 0.5f + 0.5f;
					perlinDebugBox[y][x].data.transform.pos =
					{
						lY * 3.0f,
						0.0f,
						lY * 3.0f };

					noiseData[y][x].x = lY;
					noiseData[y][x].y = lY;

					perlinDebugBox[y][x].data.color.color =
					{
						static_cast<int>(lY * 255.0f),
						static_cast<int>(lY * 255.0f),
						static_cast<int>(lY * 255.0f),
						255
					};
				}
			}
			initNoizeFlag = false;
		}

		if (uv.x != prevUv.x ||
			uv.y != prevUv.y)
		{
			KazMath::Vec2<float>lUV = { uv.x / 10.0f,uv.y / 10.0f };
			float lX = PerlinNoize(lUV, 1) * 0.5f + 0.5f;
			float lY = PerlinNoize(lUV, 1) * 0.5f + 0.5f;

			moveNoiseBlock.data.transform.pos = {};
			moveNoiseBlock.data.transform.pos.x = lX * 100.0f;
			moveNoiseBlock.data.transform.pos.y = 5.0f;
			moveNoiseBlock.data.transform.pos.z = lY * 100.0f;
			prevUv = uv;
		}
		KazMath::Vec3<float>vel = CurlNoise(moveNoiseBlock.data.transform.pos, 1);
		moveNoiseBlock.data.transform.pos += vel;
	}


	texParticle->Update(handle);

}

void DebugMeshParticleScene::Draw()
{
	RenderTargetStatus::Instance()->SetDoubleBufferFlame();
	RenderTargetStatus::Instance()->ClearDoubuleBuffer({ 0.0f,0.0f,0.0f });

	rendertarget->SetRenderTarget();

	if (cpuCheckParticleFlag)
	{
		//三角形描画
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
		if (!deadParticleFlag)
		{
			meshEmitter[meshIndex]->Draw();
		}
		else
		{
			deadParticle->Draw();
		}
		model.Draw();
	}
	else if (perlinNoizeFlag)
	{
		for (int x = 0; x < perlinDebugBox.size(); ++x)
		{
			for (int y = 0; y < perlinDebugBox[x].size(); ++y)
			{
				perlinDebugBox[y][x].Draw();
			}
		}

		moveNoiseBlock.Draw();
	}

	texParticle->Draw();

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

float DebugMeshParticleScene::PerlinNoize(const KazMath::Vec2<float> UV, int SEED)
{
	KazMath::Vec2<float> p = { std::floor(UV.x),std::floor(UV.y) };
	KazMath::Vec2<float> f = { Frac(UV.x),Frac(UV.y) };

	int lS = SEED;
	lS = 0;

	KazMath::Vec2<float>dot1(KazMath::Rand(1.0f, -1.0f), KazMath::Rand(1.0f, -1.0f));
	float w00 = dot1.Dot(f);

	KazMath::Vec2<float>dot2(KazMath::Rand(1.0f, -1.0f), KazMath::Rand(1.0f, -1.0f));
	float w10 = dot1.Dot(f);

	KazMath::Vec2<float>dot3(KazMath::Rand(1.0f, -1.0f), KazMath::Rand(1.0f, -1.0f));
	float w01 = dot1.Dot(f);

	KazMath::Vec2<float>dot4(KazMath::Rand(1.0f, -1.0f), KazMath::Rand(1.0f, -1.0f));
	float w11 = dot1.Dot(f);

	KazMath::Vec2<float> u = f * f * (KazMath::Vec2<float>(3.0f, 3.0f) - (f * 2.0f));


	float l1 = Larp(w00, w10, u.x);
	float l2 = Larp(w01, w11, u.x);

	return Larp(l1, l2, u.y);
}