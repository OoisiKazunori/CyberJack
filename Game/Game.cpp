#include "Game.h"
#include"../KazLibrary/Input/KeyBoradInputManager.h"
#include"../KazLibrary/Input/ControllerInputManager.h"
#include"../KazLibrary/Imgui/MyImgui.h"
#include"../KazLibrary/Loader/ObjResourceMgr.h"
#include"../KazLibrary/Helper/ResourceFilePass.h"
#include"../Game/Debug/ParameterMgr.h"
#include"../KazLibrary/Math/KazMath.h"

Game::Game()
{
	besidePoly = std::make_unique<BoxPolygonRender>();
	verticlaPoly = std::make_unique<BoxPolygonRender>();
	cameraPoly = std::make_unique<BoxPolygonRender>();

	model = std::make_unique<ObjModelRender>();
	model->data.pipelineName = PIPELINE_NAME_COLOR_WIREFLAME;
	model->data.transform.pos = { 0.0f,0.0f,20.0f };
	model->data.transform.scale = { 5.0f,5.0f,5.0f };
	model->data.handle = ObjResourceMgr::Instance()->LoadModel(KazFilePathName::TestPath + "hamster.obj");
	model->data.color = { 255.0f,0.0f,0.0f,255.0f };


	//mainRenderTarget.data.handle = RenderTargetStatus::Instance()->CreateRenderTarget({ WIN_X,WIN_Y }, BG_COLOR, DXGI_FORMAT_R8G8B8A8_UNORM);
	mainRenderTarget.data.transform.pos = { WIN_X / 2.0f,WIN_Y / 2.0f };
	mainRenderTarget.data.pipelineName = PIPELINE_NAME_SPRITE_NOBLEND;

	std::vector<MultiRenderTargetData> data;
	data.push_back(MultiRenderTargetData());
	data.push_back(MultiRenderTargetData());
	data[0].graphSize = { WIN_X,WIN_Y };
	data[0].backGroundColor = BG_COLOR;
	data[1].graphSize = { WIN_X,WIN_Y };
	data[1].backGroundColor = { 0.0f,0.0f,0.0f };

	handles =
		RenderTargetStatus::Instance()->CreateMultiRenderTarget(data, DXGI_FORMAT_R8G8B8A8_UNORM);
	mainRenderTarget.data.handle = handles[0];

	addHandle = RenderTargetStatus::Instance()->CreateRenderTarget({ WIN_X,WIN_Y }, XMFLOAT3(0.0f, 0.0f, 0.0f), DXGI_FORMAT_R8G8B8A8_UNORM);
	addRenderTarget.data.transform.pos = { WIN_X / 2.0f,WIN_Y / 2.0f };
	addRenderTarget.data.pipelineName = PIPELINE_NAME_ADDBLEND;

	buler = std::make_unique<GaussianBuler>(XMFLOAT2(WIN_X, WIN_Y), XMFLOAT3(0.0f, 0.0f, 0.0f));



	luminaceTex.data.pipelineName = PIPELINE_NAME_SPRITE_LUMI;
	luminaceTex.data.handle = handles[0];
	luminaceTex.data.addHandle.handle[0] = handles[1];
	luminaceTex.data.addHandle.paramType[0] = GRAPHICS_PRAMTYPE_TEX2;
	luminaceTex.data.transform.pos = { WIN_X / 2.0f,WIN_Y / 2.0f };


	std::array<SpriteVertex, 4>vert;
	XMVECTOR adjPos = { 50.0f,0.0f,0.0f };
	vert[0].pos = XMFLOAT3(-800.0f, 0.0f, 700.0f);
	vert[0].uv = { 0.0f,0.0f };
	vert[1].pos = XMFLOAT3(800.0f, 0.0f, 700.0f);
	vert[1].uv = { 1.0f,0.0f };
	vert[2].pos = XMFLOAT3(-800.0f, 0.0f, -700.0f);
	vert[2].uv = { 0.0f,1.0f };
	vert[3].pos = XMFLOAT3(800.0f, 0.0f, -700.0f);
	vert[3].uv = { 1.0f,1.0f };
	topPolygon = std::make_unique<PolygonRender>(vert);
	initPFlag = false;

	int handle = topPolygon->CreateConstBuffer(sizeof(GradationData), typeid(GradationData).name(), GRAPHICS_RANGE_TYPE_CBV, GRAPHICS_PRAMTYPE_DATA);
	topPolygon->data.pipelineName = PIPELINE_NAME_SPRITE_GRADATION;

	GradationData gradData;
	gradData.endColor = XMFLOAT4(0.24f, 0.09f, 0.62f, 1.0f);
	gradData.firstColor = XMFLOAT4(0.24f, 0.09f, 0.62f, 1.0f);
	topPolygon->TransData(&gradData, handle, typeid(gradData).name());

}

void Game::Init()
{
	player.Init(KazMath::Transform3D().pos);
	cursor.Init();

	hitBox.Init({ 0.0f,0.0f,30.0f }, 1);

	sceneNum = SCENE_NONE;


	BufferMemorySize enemySize = { 0,ENEMY_NUM_MAX };
	BufferMemorySize eventSize = { ENEMY_NUM_MAX,ENEMY_NUM_MAX + 50 };

	float z = 300.0f;

#pragma region 敵の配置レベル1
	int respoineIndex = 0;
	int layerLevel = 0;
	int time = 60;
	//2-----------------------------------------------------------------
	responeData[0][respoineIndex].enemyType = ENEMY_TYPE_NORMAL;
	responeData[0][respoineIndex].layerLevel = 0;
	responeData[0][respoineIndex].flame = time * 3;
	responeData[0][respoineIndex].initPos = { 20.0f,10.0f,z };
	++respoineIndex;

	responeData[0][respoineIndex].enemyType = ENEMY_TYPE_NORMAL;
	responeData[0][respoineIndex].layerLevel = 0;
	responeData[0][respoineIndex].flame = time * 3;
	responeData[0][respoineIndex].initPos = { 60.0f,10.0f,z };
	++respoineIndex;
	//2-----------------------------------------------------------------

	//2-----------------------------------------------------------------
	responeData[0][respoineIndex].enemyType = ENEMY_TYPE_NORMAL;
	responeData[0][respoineIndex].layerLevel = 0;
	responeData[0][respoineIndex].flame = time * 6;
	responeData[0][respoineIndex].initPos = { -20.0f,10.0f,z };
	++respoineIndex;

	responeData[0][respoineIndex].enemyType = ENEMY_TYPE_NORMAL;
	responeData[0][respoineIndex].layerLevel = 0;
	responeData[0][respoineIndex].flame = time * 6;
	responeData[0][respoineIndex].initPos = { -60.0f,10.0f,z };
	++respoineIndex;
	//2-----------------------------------------------------------------


	//4-----------------------------------------------------------------
	responeData[0][respoineIndex].enemyType = ENEMY_TYPE_NORMAL;
	responeData[0][respoineIndex].layerLevel = 0;
	responeData[0][respoineIndex].flame = time * 9;
	responeData[0][respoineIndex].initPos = { 20.0f,10.0f,z };
	++respoineIndex;

	responeData[0][respoineIndex].enemyType = ENEMY_TYPE_NORMAL;
	responeData[0][respoineIndex].layerLevel = 0;
	responeData[0][respoineIndex].flame = time * 9;
	responeData[0][respoineIndex].initPos = { 40.0f,10.0f,z };
	++respoineIndex;

	responeData[0][respoineIndex].enemyType = ENEMY_TYPE_NORMAL;
	responeData[0][respoineIndex].layerLevel = 0;
	responeData[0][respoineIndex].flame = time * 9;
	responeData[0][respoineIndex].initPos = { 60.0f,10.0f,z };
	++respoineIndex;

	responeData[0][respoineIndex].enemyType = ENEMY_TYPE_NORMAL;
	responeData[0][respoineIndex].layerLevel = 0;
	responeData[0][respoineIndex].flame = time * 9;
	responeData[0][respoineIndex].initPos = { 80.0f,10.0f,z };
	++respoineIndex;
	//4-----------------------------------------------------------------






	//4-----------------------------------------------------------------
	responeData[0][respoineIndex].enemyType = ENEMY_TYPE_NORMAL;
	responeData[0][respoineIndex].layerLevel = 0;
	responeData[0][respoineIndex].flame = time * 12;
	responeData[0][respoineIndex].initPos = { -20.0f,10.0f,z };
	++respoineIndex;

	responeData[0][respoineIndex].enemyType = ENEMY_TYPE_NORMAL;
	responeData[0][respoineIndex].layerLevel = 0;
	responeData[0][respoineIndex].flame = time * 12;
	responeData[0][respoineIndex].initPos = { -40.0f,10.0f,z };
	++respoineIndex;

	responeData[0][respoineIndex].enemyType = ENEMY_TYPE_NORMAL;
	responeData[0][respoineIndex].layerLevel = 0;
	responeData[0][respoineIndex].flame = time * 12;
	responeData[0][respoineIndex].initPos = { -60.0f,10.0f,z };
	++respoineIndex;

	responeData[0][respoineIndex].enemyType = ENEMY_TYPE_NORMAL;
	responeData[0][respoineIndex].layerLevel = 0;
	responeData[0][respoineIndex].flame = time * 12;
	responeData[0][respoineIndex].initPos = { -80.0f,10.0f,z };
	++respoineIndex;
	//4-----------------------------------------------------------------


	//4-----------------------------------------------------------------
	responeData[0][respoineIndex].enemyType = ENEMY_TYPE_NORMAL;
	responeData[0][respoineIndex].layerLevel = 0;
	responeData[0][respoineIndex].flame = time * 15;
	responeData[0][respoineIndex].initPos = { 20.0f,10.0f,z };
	++respoineIndex;

	responeData[0][respoineIndex].enemyType = ENEMY_TYPE_NORMAL;
	responeData[0][respoineIndex].layerLevel = 0;
	responeData[0][respoineIndex].flame = time * 15;
	responeData[0][respoineIndex].initPos = { 40.0f,10.0f,z };
	++respoineIndex;

	responeData[0][respoineIndex].enemyType = ENEMY_TYPE_NORMAL;
	responeData[0][respoineIndex].layerLevel = 0;
	responeData[0][respoineIndex].flame = time * 15;
	responeData[0][respoineIndex].initPos = { 60.0f,10.0f,z };
	++respoineIndex;

	responeData[0][respoineIndex].enemyType = ENEMY_TYPE_NORMAL;
	responeData[0][respoineIndex].layerLevel = 0;
	responeData[0][respoineIndex].flame = time * 15;
	responeData[0][respoineIndex].initPos = { 80.0f,10.0f,z };
	++respoineIndex;
	//4-----------------------------------------------------------------


	//4-----------------------------------------------------------------
	responeData[0][respoineIndex].enemyType = ENEMY_TYPE_NORMAL;
	responeData[0][respoineIndex].layerLevel = 0;
	responeData[0][respoineIndex].flame = time * 18;
	responeData[0][respoineIndex].initPos = { -20.0f,10.0f,z };
	++respoineIndex;

	responeData[0][respoineIndex].enemyType = ENEMY_TYPE_NORMAL;
	responeData[0][respoineIndex].layerLevel = 0;
	responeData[0][respoineIndex].flame = time * 18;
	responeData[0][respoineIndex].initPos = { -40.0f,10.0f,z };
	++respoineIndex;

	responeData[0][respoineIndex].enemyType = ENEMY_TYPE_NORMAL;
	responeData[0][respoineIndex].layerLevel = 0;
	responeData[0][respoineIndex].flame = time * 18;
	responeData[0][respoineIndex].initPos = { -60.0f,10.0f,z };
	++respoineIndex;

	responeData[0][respoineIndex].enemyType = ENEMY_TYPE_NORMAL;
	responeData[0][respoineIndex].layerLevel = 0;
	responeData[0][respoineIndex].flame = time * 18;
	responeData[0][respoineIndex].initPos = { -80.0f,10.0f,z };
	++respoineIndex;
	//4-----------------------------------------------------------------

#pragma endregion

#pragma region 敵の配置レベル2
	respoineIndex = 0;
	++layerLevel;
	//5-----------------------------------------------------------------
	responeData[0][respoineIndex].enemyType = ENEMY_TYPE_NORMAL;
	responeData[0][respoineIndex].layerLevel = layerLevel;
	responeData[0][respoineIndex].flame = time * 3;
	responeData[0][respoineIndex].initPos = { -40.0f,10.0f,z };
	++respoineIndex;

	responeData[0][respoineIndex].enemyType = ENEMY_TYPE_NORMAL;
	responeData[0][respoineIndex].layerLevel = layerLevel;
	responeData[0][respoineIndex].flame = time * 3;
	responeData[0][respoineIndex].initPos = { -20.0f,10.0f,z };
	++respoineIndex;

	responeData[0][respoineIndex].enemyType = ENEMY_TYPE_NORMAL;
	responeData[0][respoineIndex].layerLevel = layerLevel;
	responeData[0][respoineIndex].flame = time * 3;
	responeData[0][respoineIndex].initPos = { 0.0f,10.0f,z };
	++respoineIndex;

	responeData[0][respoineIndex].enemyType = ENEMY_TYPE_NORMAL;
	responeData[0][respoineIndex].layerLevel = layerLevel;
	responeData[0][respoineIndex].flame = time * 3;
	responeData[0][respoineIndex].initPos = { 20.0f,10.0f,z };
	++respoineIndex;

	responeData[0][respoineIndex].enemyType = ENEMY_TYPE_NORMAL;
	responeData[0][respoineIndex].layerLevel = layerLevel;
	responeData[0][respoineIndex].flame = time * 3;
	responeData[0][respoineIndex].initPos = { 40.0f,10.0f,z };
	++respoineIndex;
	//5-----------------------------------------------------------------

	//2-----------------------------------------------------------------
	responeData[0][respoineIndex].enemyType = ENEMY_TYPE_NORMAL;
	responeData[0][respoineIndex].layerLevel = layerLevel;
	responeData[0][respoineIndex].flame = time * 6;
	responeData[0][respoineIndex].initPos = { -20.0f,-10.0f,z };
	++respoineIndex;

	responeData[0][respoineIndex].enemyType = ENEMY_TYPE_NORMAL;
	responeData[0][respoineIndex].layerLevel = layerLevel;
	responeData[0][respoineIndex].flame = time * 6;
	responeData[0][respoineIndex].initPos = { -40.0f,-10.0f,z };
	++respoineIndex;
	//2-----------------------------------------------------------------


	//2-----------------------------------------------------------------
	responeData[0][respoineIndex].enemyType = ENEMY_TYPE_NORMAL;
	responeData[0][respoineIndex].layerLevel = layerLevel;
	responeData[0][respoineIndex].flame = time * 8;
	responeData[0][respoineIndex].initPos = { 20.0f,-10.0f,z };
	++respoineIndex;

	responeData[0][respoineIndex].enemyType = ENEMY_TYPE_NORMAL;
	responeData[0][respoineIndex].layerLevel = layerLevel;
	responeData[0][respoineIndex].flame = time * 8;
	responeData[0][respoineIndex].initPos = { 40.0f,-10.0f,z };
	++respoineIndex;
	//2-----------------------------------------------------------------

	//3-----------------------------------------------------------------
	responeData[0][respoineIndex].enemyType = ENEMY_TYPE_NORMAL;
	responeData[0][respoineIndex].layerLevel = layerLevel;
	responeData[0][respoineIndex].flame = time * 11;
	responeData[0][respoineIndex].initPos = { -20.0f,20.0f,z };
	++respoineIndex;

	responeData[0][respoineIndex].enemyType = ENEMY_TYPE_NORMAL;
	responeData[0][respoineIndex].layerLevel = layerLevel;
	responeData[0][respoineIndex].flame = time * 11;
	responeData[0][respoineIndex].initPos = { -40.0f,20.0f,z };
	++respoineIndex;

	responeData[0][respoineIndex].enemyType = ENEMY_TYPE_NORMAL;
	responeData[0][respoineIndex].layerLevel = layerLevel;
	responeData[0][respoineIndex].flame = time * 11;
	responeData[0][respoineIndex].initPos = { -60.0f,20.0f,z };
	++respoineIndex;
	//3-----------------------------------------------------------------


	//3-----------------------------------------------------------------
	responeData[0][respoineIndex].enemyType = ENEMY_TYPE_NORMAL;
	responeData[0][respoineIndex].layerLevel = layerLevel;
	responeData[0][respoineIndex].flame = time * 14;
	responeData[0][respoineIndex].initPos = { 20.0f,-10.0f,z };
	++respoineIndex;

	responeData[0][respoineIndex].enemyType = ENEMY_TYPE_NORMAL;
	responeData[0][respoineIndex].layerLevel = layerLevel;
	responeData[0][respoineIndex].flame = time * 14;
	responeData[0][respoineIndex].initPos = { 40.0f,-10.0f,z };
	++respoineIndex;

	responeData[0][respoineIndex].enemyType = ENEMY_TYPE_NORMAL;
	responeData[0][respoineIndex].layerLevel = layerLevel;
	responeData[0][respoineIndex].flame = time * 14;
	responeData[0][respoineIndex].initPos = { 60.0f,-10.0f,z };
	++respoineIndex;
	//3-----------------------------------------------------------------


	//2-----------------------------------------------------------------
	responeData[0][respoineIndex].enemyType = ENEMY_TYPE_NORMAL;
	responeData[0][respoineIndex].layerLevel = layerLevel;
	responeData[0][respoineIndex].flame = time * 17;
	responeData[0][respoineIndex].initPos = { 10.0f,20.0f,z };
	++respoineIndex;

	responeData[0][respoineIndex].enemyType = ENEMY_TYPE_NORMAL;
	responeData[0][respoineIndex].layerLevel = layerLevel;
	responeData[0][respoineIndex].flame = time * 17;
	responeData[0][respoineIndex].initPos = { -10.0f,20.0f,z };
	++respoineIndex;
	//2-----------------------------------------------------------------


	//2-----------------------------------------------------------------
	responeData[0][respoineIndex].enemyType = ENEMY_TYPE_NORMAL;
	responeData[0][respoineIndex].layerLevel = layerLevel;
	responeData[0][respoineIndex].flame = time * 19;
	responeData[0][respoineIndex].initPos = { 10.0f,-10.0f,z };
	++respoineIndex;

	responeData[0][respoineIndex].enemyType = ENEMY_TYPE_NORMAL;
	responeData[0][respoineIndex].layerLevel = layerLevel;
	responeData[0][respoineIndex].flame = time * 19;
	responeData[0][respoineIndex].initPos = { -10.0f,-10.0f,z };
	++respoineIndex;
	//2-----------------------------------------------------------------


	//2-----------------------------------------------------------------
	responeData[0][respoineIndex].enemyType = ENEMY_TYPE_NORMAL;
	responeData[0][respoineIndex].layerLevel = layerLevel;
	responeData[0][respoineIndex].flame = time * 21;
	responeData[0][respoineIndex].initPos = { -10.0f,20.0f,z };
	++respoineIndex;

	responeData[0][respoineIndex].enemyType = ENEMY_TYPE_NORMAL;
	responeData[0][respoineIndex].layerLevel = layerLevel;
	responeData[0][respoineIndex].flame = time * 21;
	responeData[0][respoineIndex].initPos = { -10.0f,0.0f,z };
	++respoineIndex;
	//2-----------------------------------------------------------------


	//2-----------------------------------------------------------------
	responeData[0][respoineIndex].enemyType = ENEMY_TYPE_NORMAL;
	responeData[0][respoineIndex].layerLevel = layerLevel;
	responeData[0][respoineIndex].flame = time * 23;
	responeData[0][respoineIndex].initPos = { 10.0f,20.0f,z };
	++respoineIndex;

	responeData[0][respoineIndex].enemyType = ENEMY_TYPE_NORMAL;
	responeData[0][respoineIndex].layerLevel = layerLevel;
	responeData[0][respoineIndex].flame = time * 23;
	responeData[0][respoineIndex].initPos = { 10.0f,0.0f,z };
	++respoineIndex;
	//2-----------------------------------------------------------------



	//4-----------------------------------------------------------------
	responeData[0][respoineIndex].enemyType = ENEMY_TYPE_NORMAL;
	responeData[0][respoineIndex].layerLevel = layerLevel;
	responeData[0][respoineIndex].flame = time * 26;
	responeData[0][respoineIndex].initPos = { -20.0f,20.0f,z };
	++respoineIndex;

	responeData[0][respoineIndex].enemyType = ENEMY_TYPE_NORMAL;
	responeData[0][respoineIndex].layerLevel = layerLevel;
	responeData[0][respoineIndex].flame = time * 26;
	responeData[0][respoineIndex].initPos = { -20.0f,0.0f,z };
	++respoineIndex;

	responeData[0][respoineIndex].enemyType = ENEMY_TYPE_NORMAL;
	responeData[0][respoineIndex].layerLevel = layerLevel;
	responeData[0][respoineIndex].flame = time * 26;
	responeData[0][respoineIndex].initPos = { -40.0f,20.0f,z };
	++respoineIndex;

	responeData[0][respoineIndex].enemyType = ENEMY_TYPE_NORMAL;
	responeData[0][respoineIndex].layerLevel = layerLevel;
	responeData[0][respoineIndex].flame = time * 26;
	responeData[0][respoineIndex].initPos = { -40.0f,0.0f,z };
	++respoineIndex;
	//4-----------------------------------------------------------------



	//4-----------------------------------------------------------------
	responeData[0][respoineIndex].enemyType = ENEMY_TYPE_NORMAL;
	responeData[0][respoineIndex].layerLevel = layerLevel;
	responeData[0][respoineIndex].flame = time * 29;
	responeData[0][respoineIndex].initPos = { 20.0f,20.0f,z };
	++respoineIndex;

	responeData[0][respoineIndex].enemyType = ENEMY_TYPE_NORMAL;
	responeData[0][respoineIndex].layerLevel = layerLevel;
	responeData[0][respoineIndex].flame = time * 29;
	responeData[0][respoineIndex].initPos = { 20.0f,0.0f,z };
	++respoineIndex;

	responeData[0][respoineIndex].enemyType = ENEMY_TYPE_NORMAL;
	responeData[0][respoineIndex].layerLevel = layerLevel;
	responeData[0][respoineIndex].flame = time * 29;
	responeData[0][respoineIndex].initPos = { 40.0f,20.0f,z };
	++respoineIndex;

	responeData[0][respoineIndex].enemyType = ENEMY_TYPE_NORMAL;
	responeData[0][respoineIndex].layerLevel = layerLevel;
	responeData[0][respoineIndex].flame = time * 29;
	responeData[0][respoineIndex].initPos = { 40.0f,0.0f,z };
	++respoineIndex;
	//4-----------------------------------------------------------------


	//4-----------------------------------------------------------------
	responeData[0][respoineIndex].enemyType = ENEMY_TYPE_NORMAL;
	responeData[0][respoineIndex].layerLevel = layerLevel;
	responeData[0][respoineIndex].flame = time * 32;
	responeData[0][respoineIndex].initPos = { -20.0f,20.0f,z };
	++respoineIndex;

	responeData[0][respoineIndex].enemyType = ENEMY_TYPE_NORMAL;
	responeData[0][respoineIndex].layerLevel = layerLevel;
	responeData[0][respoineIndex].flame = time * 32;
	responeData[0][respoineIndex].initPos = { -20.0f,0.0f,z };
	++respoineIndex;

	responeData[0][respoineIndex].enemyType = ENEMY_TYPE_NORMAL;
	responeData[0][respoineIndex].layerLevel = layerLevel;
	responeData[0][respoineIndex].flame = time * 32;
	responeData[0][respoineIndex].initPos = { 20.0f,20.0f,z };
	++respoineIndex;

	responeData[0][respoineIndex].enemyType = ENEMY_TYPE_NORMAL;
	responeData[0][respoineIndex].layerLevel = layerLevel;
	responeData[0][respoineIndex].flame = time * 32;
	responeData[0][respoineIndex].initPos = { 20.0f,0.0f,z };
	++respoineIndex;
	//4-----------------------------------------------------------------
#pragma endregion


	//操作可能OBJを纏めて生成する処理----------------------------------------------------------------
	for (int enemyType = 0; enemyType < responeData.size(); ++enemyType)
	{
		for (int enemyCount = 0; enemyCount < responeData[enemyType].size(); ++enemyCount)
		{
			if (responeData[enemyType][enemyCount].enemyType != -1)
			{
				switch (responeData[enemyType][enemyCount].enemyType)
				{
				case ENEMY_TYPE_NORMAL:
					enemies[enemyType][enemyCount] = std::make_unique<NormalEnemy>();
					break;

				case ENEMY_TYPE_MOTHER:
					enemies[enemyType][enemyCount] = std::make_unique<NormalEnemy>();
					//子敵の生成(テスト用)
					for (int i = 0; i < 8; ++i)
					{
						int index = enemiesHandle[ENEMY_TYPE_KID];
						enemies[ENEMY_TYPE_KID][index] = std::make_unique<KidEnemy>();
						++enemiesHandle[ENEMY_TYPE_KID];
					}
					break;

				default:
					break;
				}
				++enemiesHandle[enemyType];
			}
		}
	}
	//敵を纏めて生成する処理----------------------------------------------------------------



	//ゲームループの初期化----------------------------------------------------------------
	gameFlame = 0;
	//ゴールに触れ無かった場合に次のステージに移動する際の最大フレーム数
	for (int i = 0; i < changeLayerLevelMaxTime.size(); ++i)
	{
		changeLayerLevelMaxTime[i] = 1000;
	}
	gameStageLevel = 1;
	//ゲームループの初期化----------------------------------------------------------------


	eyePos = { 0.0f,5.0f,-10.0f };
	trackingTargetPos = { 0.0f,0.0f,0.0f };
	nowTargerPos = { 0.0f,0.0f,0.0f };
	leftRightAngleVel = { -91.0f,-91.0f,0.0f };
	upDownAngleVel = { -103.0f,-103.0f,0.0f };

	baseEyePos = { 0.0f,5.0f,-10.0f };
	baseTargetPos = { 0.0f,3.0f,0.0f };

	centralPos = { 0.0f,3.0f,0.0f };
	centralPos2 = centralPos;


	besidePoly->data.color = { 255.0f,255.0f,255.0f,255.0f };
	verticlaPoly->data.color = { 255.0f,255.0f,0.0f,255.0f };
	cameraPoly->data.color = { 255.0f,0.0f,0.0f,255.0f };
	r = 8.0f;
	r2 = 8.0f;


	testEnemyPos = { 0.0f,0.0f,100.0f };
	mulValue = { 10.0f,30.0f };
	mulValue2 = { 60.0f,60.0f };
	cameraChangeFlag = true;

	forceCameraDirVel.m128_f32[0] = -90.0f;
}

void Game::Finalize()
{
}

void Game::Input()
{
	KeyBoradInputManager *input = KeyBoradInputManager::Instance();
	ControllerInputManager *inputController = ControllerInputManager::Instance();

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


	if (inputController->InputTrigger(XINPUT_GAMEPAD_A))
	{
		bool debug = false;
		debug = true;
	}

	if (input->InputTrigger(DIK_O))
	{
		sceneNum = 0;
	}

	bool upFlag = false;
	bool downFlag = false;
	bool leftFlag = false;
	bool rightFlag = false;
	bool doneFlag = false;
	bool releaseFlag = false;

	const int DEAD_ZONE = 3000;
	if (inputController->InputStickState(LEFT_STICK, UP_SIDE, DEAD_ZONE))
	{
		upFlag = true;
	}
	if (inputController->InputStickState(LEFT_STICK, DOWN_SIDE, DEAD_ZONE))
	{
		downFlag = true;
	}
	if (inputController->InputStickState(LEFT_STICK, LEFT_SIDE, DEAD_ZONE))
	{
		leftFlag = true;
	}
	if (inputController->InputStickState(LEFT_STICK, RIGHT_SIDE, DEAD_ZONE))
	{
		rightFlag = true;
	}
	if (inputController->InputState(XINPUT_GAMEPAD_A))
	{
		doneFlag = true;
	}
	if (inputController->InputRelease(XINPUT_GAMEPAD_A))
	{
		releaseFlag = true;
	}



	if (!upFlag && !downFlag && !leftFlag && !rightFlag)
	{
		bool debug = false;
	}

	XMVECTOR joyStick;
	joyStick.m128_f32[0] = inputController->GetJoyStickLXNum(0) / 32767.0f;
	joyStick.m128_f32[1] = inputController->GetJoyStickLYNum(0) / 32767.0f;

	cursor.Input
	(
		upFlag,
		downFlag,
		leftFlag,
		rightFlag,
		doneFlag,
		releaseFlag,
		joyStick
	);


	//カメラの前後左右強制に向かせる処理
	if (input->InputState(DIK_Z))
	{
		forceCameraDirVel.m128_f32[0] = 0.0f;
	}
	if (input->InputState(DIK_X))
	{
		forceCameraDirVel.m128_f32[0] = -90.0f;
	}
	if (input->InputState(DIK_C))
	{
		forceCameraDirVel.m128_f32[0] = -180.0f;
	}
	if (input->InputState(DIK_V))
	{
		forceCameraDirVel.m128_f32[0] = -270.0f;
	}



	XMVECTOR vel = {};
	XMVECTOR verticalVel = {};
	XMVECTOR besideVel = {};

	float speed = 1.0f;
	if (upFlag)
	{
		vel.m128_f32[1] = -speed;
		verticalVel = { -speed,-speed,0.0f };
	}
	if (downFlag)
	{
		vel.m128_f32[1] = speed;
		verticalVel = { speed,speed,0.0f };
	}
	if (leftFlag)
	{
		vel.m128_f32[0] = -speed;
		besideVel = { speed,speed,0.0f };
	}
	if (rightFlag)
	{
		vel.m128_f32[0] = speed;
		besideVel = { -speed,-speed,0.0f };
	}


	XMVECTOR cursorValue = cursor.GetValue();


	nowTargerPos += vel;

	upDownAngleVel =
	{
		mulValue.x * -cursorValue.m128_f32[1],
		mulValue.y * -cursorValue.m128_f32[1]
	};
	leftRightAngleVel =
	{
		forceCameraDirVel.m128_f32[0] + mulValue2.x * -cursorValue.m128_f32[0],
		forceCameraDirVel.m128_f32[0] + mulValue2.y * -cursorValue.m128_f32[0]
	};

	if (KeyBoradInputManager::Instance()->InputTrigger(DIK_R))
	{
		gameFlame = 0;
		for (int i = 0; i < addEnemiesHandle.size(); ++i)
		{
			addEnemiesHandle[i] = 0;;
		}
	}
}

void Game::Update()
{
	/*ImGui::Begin("Camera");
	ImGui::Text("Target");
	ImGui::InputFloat("TargetX", &baseTargetPos.m128_f32[0]);
	ImGui::InputFloat("TargetY", &baseTargetPos.m128_f32[1]);
	ImGui::InputFloat("TargetZ", &baseTargetPos.m128_f32[2]);
	ImGui::Text("Central_Side");
	ImGui::InputFloat("CentralX", &centralPos.m128_f32[0]);
	ImGui::InputFloat("CentralY", &centralPos.m128_f32[1]);
	ImGui::InputFloat("CentralZ", &centralPos.m128_f32[2]);
	ImGui::Text("Central_UpDown");
	ImGui::InputFloat("Central2X", &centralPos2.m128_f32[0]);
	ImGui::InputFloat("Central2Y", &centralPos2.m128_f32[1]);
	ImGui::InputFloat("Central2Z", &centralPos2.m128_f32[2]);
	ImGui::Text("Force");
	ImGui::InputFloat("forceCameraDirVel0", &forceCameraDirVel.m128_f32[0]);
	ImGui::InputFloat("forceCameraDirVel1", &forceCameraDirVel.m128_f32[1]);
	ImGui::Text("Circle");
	ImGui::InputFloat("R", &r);
	ImGui::InputFloat("R2", &r2);
	ImGui::Text("MulValue");
	ImGui::InputFloat("mulValueX", &mulValue.x);
	ImGui::InputFloat("mulValueY", &mulValue.y);
	ImGui::InputFloat("mulValue2X", &mulValue2.x);
	ImGui::InputFloat("mulValue2Y", &mulValue2.y);
	ImGui::Text("leftRightAngleVel:X%f,Y:%f", leftRightAngleVel.m128_f32[0], leftRightAngleVel.m128_f32[1]);
	ImGui::Text("upDownAngleVel:X%f,Y:%f", upDownAngleVel.m128_f32[0], upDownAngleVel.m128_f32[1]);
	ImGui::Text("trackUpDownAngleVel:X%f,Y:%f", trackUpDownAngleVel.m128_f32[0], trackUpDownAngleVel.m128_f32[1]);
	ImGui::Text("trackLeftRightAngleVel:X%f,Y:%f", trackLeftRightAngleVel.m128_f32[0], trackLeftRightAngleVel.m128_f32[1]);
	ImGui::End();*/


	//操作感に関わる設定
	/*ImGui::Begin("Move");
	ImGui::Text("dontMoveCameraStartPos:X%f,Y:%f", cursor.dontMoveCameraStartPos.m128_f32[0], cursor.dontMoveCameraStartPos.m128_f32[1]);
	ImGui::Text("dontMoveCameraEndPos:X%f,Y:%f", cursor.dontMoveCameraEndPos.m128_f32[0], cursor.dontMoveCameraEndPos.m128_f32[1]);
	ImGui::Text("CursorPos:X%f,Y:%f", cursor.cursorPos.m128_f32[0], cursor.cursorPos.m128_f32[1]);
	ImGui::Text("CameraMoveValue:X%f,Y:%f", cursor.cameraMoveValue.m128_f32[0], cursor.cameraMoveValue.m128_f32[1]);
	ImGui::InputFloat("limitValue:X", &cursor.limitValue.m128_f32[0]);
	ImGui::InputFloat("limitValue:Y", &cursor.limitValue.m128_f32[1]);
	ImGui::InputFloat("NO_MOVE_DISTANCE:X", &cursor.NO_MOVE_DISTANCE.m128_f32[0]);
	ImGui::InputFloat("NO_MOVE_DISTANCE:Y", &cursor.NO_MOVE_DISTANCE.m128_f32[1]);
	ImGui::End();*/

	ImGui::Begin("Debug");
	ImGui::Checkbox("DebugCamera", &cameraChangeFlag);
	ImGui::Checkbox("DebugLine", &lineDebugFlag);
	ImGui::End();



#pragma region カメラ挙動

	//左右の角度変更のイージング
	{
		XMVECTOR distance = leftRightAngleVel - trackLeftRightAngleVel;
		trackLeftRightAngleVel += distance * 0.1f;
	}
	//上下の角度変更のイージング
	{
		XMVECTOR distance = upDownAngleVel - trackUpDownAngleVel;
		trackUpDownAngleVel += distance * 0.1f;
	}
	//左右の回転
	besidePoly->data.transform.pos =
	{
		cosf(KazMath::AngleToRadian(trackLeftRightAngleVel.m128_f32[0])) * r,
		0.0f,
		sinf(KazMath::AngleToRadian(trackLeftRightAngleVel.m128_f32[1])) * r
	};
	//上下の回転
	verticlaPoly->data.transform.pos =
	{
		0.0f,
		trackUpDownAngleVel.m128_f32[0],
		0.0f,
	};


	//注視点の自動
	//baseTargetPos.m128_f32[2] = 0.0f + 1.0f * -cursor.GetValue().m128_f32[1];
	//if (baseTargetPos.m128_f32[2] <= 0.0f)
	//{
	//	baseTargetPos.m128_f32[2] = 0.0f;
	//}

	//centralPos.m128_f32[2] = 0.0f + 3.0f * -cursor.GetValue().m128_f32[1];




	{
		float mul = 1.0f;
		float limitValue = 3.0f;
		//上下でプレイヤーのいる位置を変える
		if (0.1f <= cursor.GetValue().m128_f32[1])
		{
			mul *= -1.0f;
			limitValue = 2.5f;
		}

		honraiPlayerCameraPos.m128_f32[0] = 0.0f + (2.0f * -cursor.GetValue().m128_f32[0]);
		honraiPlayerCameraPos.m128_f32[2] = 0.0f + (limitValue * cursor.GetValue().m128_f32[1]) * mul;


		if (0.0f <= honraiPlayerCameraPos.m128_f32[2])
		{
			honraiPlayerCameraPos.m128_f32[2] = 0.0f;
		}
		XMVECTOR distance = honraiPlayerCameraPos - player.pos;
		player.pos += distance * 0.1f;
	}


	//上下左右の回転
	cameraPoly->data.transform.pos = centralPos + (besidePoly->data.transform.pos + verticlaPoly->data.transform.pos);



	if (cameraChangeFlag)
	{
		eyePos = KazMath::LoadVecotrToXMFLOAT3(cameraPoly->data.transform.pos);
		targetPos = KazMath::LoadVecotrToXMFLOAT3(baseTargetPos);
	}
	else
	{
		//デバック用
		eyePos = KazMath::CaluEyePosForDebug(eyePos, debugCameraMove, angle);
		targetPos = KazMath::CaluTargetPosForDebug(eyePos, angle.x);
	}
	CameraMgr::Instance()->Camera(eyePos, targetPos, { 0.0f,1.0f,0.0f });

#pragma endregion


	if (KeyBoradInputManager::Instance()->InputTrigger(DIK_H) && !initPFlag)
	{
		XMVECTOR screenPos = { 0.0f,0.0f,1.0f };
		XMVECTOR leftUpPos = KazMath::ConvertScreenPosToWorldPos(screenPos, CameraMgr::Instance()->GetViewMatrix(), CameraMgr::Instance()->GetPerspectiveMatProjection());

		screenPos = { WIN_X,0.0f,1.0f };
		XMVECTOR rightUpPos = KazMath::ConvertScreenPosToWorldPos(screenPos, CameraMgr::Instance()->GetViewMatrix(), CameraMgr::Instance()->GetPerspectiveMatProjection());

		screenPos = { 0.0f,WIN_Y,1.0f };
		XMVECTOR leftDownPos = KazMath::ConvertScreenPosToWorldPos(screenPos, CameraMgr::Instance()->GetViewMatrix(), CameraMgr::Instance()->GetPerspectiveMatProjection());

		screenPos = { WIN_X,WIN_Y,1.0f };
		XMVECTOR rightDownPos = KazMath::ConvertScreenPosToWorldPos(screenPos, CameraMgr::Instance()->GetViewMatrix(), CameraMgr::Instance()->GetPerspectiveMatProjection());

		std::array<SpriteVertex, 4>vert;
		XMVECTOR adjPos = { 50.0f,0.0f,0.0f };
		vert[0].pos = KazMath::LoadVecotrToXMFLOAT3(leftUpPos - adjPos);
		vert[0].uv = { 0.0f,0.0f };
		vert[1].pos = KazMath::LoadVecotrToXMFLOAT3(rightUpPos + adjPos);
		vert[1].uv = { 1.0f,0.0f };
		vert[2].pos = KazMath::LoadVecotrToXMFLOAT3(leftDownPos - adjPos);
		vert[2].uv = { 0.0f,1.0f };
		vert[3].pos = KazMath::LoadVecotrToXMFLOAT3(rightDownPos + adjPos);
		vert[3].uv = { 1.0f,1.0f };

		for (int i = 0; i < vert.size(); ++i)
		{
			vert[i].pos.z = 0.0f;
		}

		GradationData data;
		data.firstColor = XMFLOAT4(0.93f, 0.65f, 0.53f, 1.0f);
		data.endColor = XMFLOAT4(0.24f, 0.09f, 0.62f, 1.0f);
		for (int i = 0; i < polygon.size(); ++i)
		{
			polygon[i] = std::make_unique<PolygonRender>(vert);
			int handle = polygon[i]->CreateConstBuffer(sizeof(GradationData), typeid(GradationData).name(), GRAPHICS_RANGE_TYPE_CBV, GRAPHICS_PRAMTYPE_DATA);
			polygon[i]->TransData(&data, handle, typeid(data).name());
			polygon[i]->data.pipelineName = PIPELINE_NAME_SPRITE_GRADATION;
		}
		CameraMgr::Instance()->CameraSetting(60.0f, 1000.0f);

		initPFlag = true;
	}

	if (initPFlag)
	{
		polygon[0]->data.transform.pos.m128_f32[2] = 650.0f;
		polygon[1]->data.transform.pos.m128_f32[0] = 550.0f;
		polygon[1]->data.transform.rotation.m128_f32[1] = 100.0f;
		polygon[2]->data.transform.pos.m128_f32[0] = -550.0f;
		polygon[2]->data.transform.rotation.m128_f32[1] = 100.0f;
		polygon[3]->data.transform.pos.m128_f32[2] = -650.0f;
	}



	//ゴールに触れ無かった場合に次のステージに移動する処理----------------------------------------------------------------
	if (changeLayerLevelMaxTime[gameStageLevel] <= gameFlame)
	{
		//++gameStageLevel;
		//gameFlame = 0;
	}
	//ゴールに触れ無かった場合に次のステージに移動する処理----------------------------------------------------------------


#pragma region 生成処理
	//敵を追加で初期化する処理----------------------------------------------------------------
	for (int enemyType = 0; enemyType < enemies.size(); ++enemyType)
	{
		for (int enemyCount = 0; enemyCount < enemies[enemyType].size(); ++enemyCount)
		{
			//生成されている、初期化している敵のみ更新処理を通す
			bool enableToUseDataFlag = enemies[enemyType][enemyCount] != nullptr && enemies[enemyType][enemyCount]->GetData()->oprationObjData->initFlag;
			if (enableToUseDataFlag)
			{
				EnemyData *enemyData = enemies[enemyType][enemyCount]->GetData().get();
				//追加で生成するデータを検知したら生成する
				if (enemyData->genarateData.enemyType != -1)
				{
					//生成する敵の種類
					int enemyTypeData = enemyData->genarateData.enemyType;
					//最後に生成して次のハンドル
					int nowHandle = addEnemiesHandle[enemyTypeData];
					responeData[enemyTypeData][nowHandle].enemyType = enemyData->genarateData.enemyType;
					//現在のレイヤーレベルに合わせる
					responeData[enemyTypeData][nowHandle].layerLevel = gameStageLevel;
					//現在のフレーム数+インターバルフレーム*個数で設定する
					responeData[enemyTypeData][nowHandle].flame = gameFlame;
					responeData[enemyTypeData][nowHandle].initPos = enemyData->genarateData.initPos;
					//ハンドルを増やす
					++addEnemiesHandle[enemyTypeData];

					//追加したら終了処理を入れる
					enemies[enemyType][enemyCount]->GetData()->genarateData.Finalize();
				}
			}
		}
	}
	//敵を追加で初期化する処理----------------------------------------------------------------


	//敵をどのタイミングで初期化する処理----------------------------------------------------------------
	for (int enemyType = 0; enemyType < responeData.size(); ++enemyType)
	{
		for (int enemyCount = 0; enemyCount < responeData[enemyType].size(); ++enemyCount)
		{
			bool enableToUseThisDataFlag = responeData[enemyType][enemyCount].enemyType != -1;
			bool readyToInitDataFlag = responeData[enemyType][enemyCount].flame == gameFlame &&
				responeData[enemyType][enemyCount].layerLevel == gameStageLevel;

			if (enableToUseThisDataFlag && readyToInitDataFlag)
			{
				switch (enemyType)
				{
				case ENEMY_TYPE_NORMAL:
					enemies[enemyType][enemyCount]->Init(responeData[enemyType][enemyCount].initPos);
					break;

				case ENEMY_TYPE_KID:
					enemies[enemyType][enemyCount]->Init(responeData[enemyType][enemyCount].initPos);
					break;
				default:
					break;
				}
			}
		}
	}
	//敵をどのタイミングで初期化する処理----------------------------------------------------------------
#pragma endregion


#pragma region ロックオン
	for (int enemyType = 0; enemyType < enemies.size(); ++enemyType)
	{
		for (int enemyCount = 0; enemyCount < enemies[enemyType].size(); ++enemyCount)
		{
			//生成されている、初期化している敵のみ更新処理を通す
			bool enableToUseDataFlag = enemies[enemyType][enemyCount] != nullptr && enemies[enemyType][enemyCount]->GetData()->oprationObjData->initFlag;
			if (enableToUseDataFlag)
			{
				//敵のデータのポインタを代入
				EnemyData *enemyData = enemies[enemyType][enemyCount]->GetData().get();

				//ロックオン判定
				bool enableToLockOnNumFlag = cursor.LockOn();
				bool enableToLockOnEnemyFlag = enemies[enemyType][enemyCount]->AliveOrNot() && !enemies[enemyType][enemyCount]->LockedOrNot();
				bool hitFlag = CollisionManager::Instance()->CheckRayAndSphere(cursor.hitBox, enemyData->hitBox);
				if (hitFlag &&
					enableToLockOnNumFlag &&
					enableToLockOnEnemyFlag)
				{
					//カーソルのカウント数を増やす
					cursor.Count();
					//敵が当たった情報を書く
					enemies[enemyType][enemyCount]->Hit();

					//線演出をかける際にどの配列を使用するか決める
					for (int i = 0; i < lineEffectArrayData.size(); ++i)
					{
						if (!lineEffectArrayData[i].usedFlag)
						{
							lineLevel[i].Attack2(player.pos, *enemyData->hitBox.center, {});
							lineEffectArrayData[i].usedFlag = true;
							lineEffectArrayData[i].lineIndex = i;
							lineEffectArrayData[i].enemyTypeIndex = enemyType;
							lineEffectArrayData[i].enemyIndex = enemyCount;
							break;
						}
					}
				}

				bool releaseFlag = cursor.Release();
				bool aliveFlag = enemies[enemyType][enemyCount]->AliveOrNot();
				bool lockedFlag = enemies[enemyType][enemyCount]->LockedOrNot();
				if (releaseFlag)
				{
					bool debug = false;
				}
				if (aliveFlag)
				{
					bool debug = false;
				}
				if (lockedFlag)
				{
					bool debug = false;
				}
			}
		}
	}

	//線がたどり着いたら敵を死亡させる
	for (int i = 0; i < lineEffectArrayData.size(); ++i)
	{
		if (lineEffectArrayData[i].usedFlag)
		{
			int lineIndex = lineEffectArrayData[i].lineIndex;
			int enemyTypeIndex = lineEffectArrayData[i].enemyTypeIndex;
			int enemyIndex = lineEffectArrayData[i].enemyIndex;
			//演出を合わせて死亡
			if (lineLevel[lineIndex].lineReachObjFlag)
			{
				enemies[enemyTypeIndex][enemyIndex]->Dead();
				lineEffectArrayData[i].Reset();
			}
		}
	}

	//線演出が終わった際にリンクのデータをリセットする
	for (int i = 0; i < lineEffectArrayData.size(); ++i)
	{
		if (lineEffectArrayData[i].usedFlag)
		{
			int lineIndex = lineEffectArrayData[i].lineIndex;
			if (lineLevel[lineIndex].allFinishFlag)
			{
				lineEffectArrayData[i].Reset();
			}
		}
	}


#pragma endregion


#pragma region 更新処理
	//更新処理----------------------------------------------------------------
	player.Update();
	cursor.Update();
	hitBox.Update();
	stage.Update();

	//ロックオンのリリース処理
	if (cursor.releaseFlag)
	{
		for (int i = 0; i < lineEffectArrayData.size(); ++i)
		{
			if (lineEffectArrayData[i].usedFlag)
			{
				int lineIndex = lineEffectArrayData[i].lineIndex;
				lineLevel[lineIndex].ReleaseShot();
			}
		}
	}

	//ロックオン中に必要なデータ
	for (int i = 0; i < lineEffectArrayData.size(); ++i)
	{
		if (lineEffectArrayData[i].usedFlag)
		{
			int lineIndex = lineEffectArrayData[i].lineIndex;
			int enemyTypeIndex = lineEffectArrayData[i].enemyTypeIndex;
			int enemyIndex = lineEffectArrayData[i].enemyIndex;
			XMVECTOR pos = *enemies[enemyTypeIndex][enemyIndex]->GetData()->hitBox.center;
			lineLevel[lineIndex].CalucurateDistance(player.pos, pos);
		}
	}


	for (int i = 0; i < lineLevel.size(); ++i)
	{
		lineLevel[i].playerPos = player.pos;
		lineLevel[i].Update();
	}

	//敵の更新処理
	for (int enemyType = 0; enemyType < enemies.size(); ++enemyType)
	{
		for (int enemyCount = 0; enemyCount < enemies[enemyType].size(); ++enemyCount)
		{
			//生成されている、初期化している敵のみ更新処理を通す
			bool enableToUseDataFlag = enemies[enemyType][enemyCount] != nullptr && enemies[enemyType][enemyCount]->GetData()->oprationObjData->initFlag;
			if (enableToUseDataFlag)
			{
				enemies[enemyType][enemyCount]->Update();
			}
		}
	}
	//更新処理----------------------------------------------------------------
#pragma endregion


	goalBox.Update();

	//ゲームループの経過時間----------------------------------------------------------------
	++gameFlame;
	//ゲームループの経過時間----------------------------------------------------------------

}

void Game::Draw()
{
	short mainHandle = mainRenderTarget.data.handle;


	RenderTargetStatus::Instance()->PrepareToChangeBarrier(handles[0]);
	RenderTargetStatus::Instance()->ClearRenderTarget(handles[0]);
	if (initPFlag)
	{
		for (int i = 0; i < polygon.size(); ++i)
		{
			polygon[i]->Draw();
		}
	}
	topPolygon->data.transform.pos.m128_f32[1] = 400.0f;
	topPolygon->Draw();
	stage.Draw();
	if (lineDebugFlag)
	{
		bg.Draw();
	}
	player.Draw();
	for (int i = 0; i < lineLevel.size(); ++i)
	{
		lineLevel[i].Draw();
	}

	//敵の描画処理----------------------------------------------------------------
	for (int enemyType = 0; enemyType < enemies.size(); ++enemyType)
	{
		for (int enemyCount = 0; enemyCount < enemies[enemyType].size(); ++enemyCount)
		{
			//生成されている敵のみ描画処理を通す
			bool enableToUseDataFlag = enemies[enemyType][enemyCount] != nullptr && enemies[enemyType][enemyCount]->GetData()->oprationObjData->initFlag;
			if (enableToUseDataFlag)
			{
				enemies[enemyType][enemyCount]->Draw();
			}
		}
	}
	goalBox.Draw();
	goalBox.effect.Draw();

	//輝度抽出
	RenderTargetStatus::Instance()->PrepareToChangeBarrier(addHandle, handles[0]);
	RenderTargetStatus::Instance()->ClearRenderTarget(addHandle);
	luminaceTex.Draw();
	RenderTargetStatus::Instance()->PrepareToCloseBarrier(addHandle);
	RenderTargetStatus::Instance()->SetDoubleBufferFlame(BG_COLOR);
	mainRenderTarget.Draw();
	addRenderTarget.data.handle = buler->BlurImage(addHandle);
	addRenderTarget.Draw();

	cursor.Draw();
}

int Game::SceneChange()
{
	if (sceneNum != -1) {
		int tmp = sceneNum;
		sceneNum = -1;
		return tmp;
	}

	return SCENE_NONE;
}
