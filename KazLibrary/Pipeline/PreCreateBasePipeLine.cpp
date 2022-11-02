#include "PreCreateBasePipeLine.h"
#include"../Pipeline/GraphicsRootSignature.h"
#include"../Helper/ResourceFilePass.h"

PreCreateBasePipeLine::PreCreateBasePipeLine()
{
	GraphicsPipeLineMgr *pipelineMgr = GraphicsPipeLineMgr::Instance();

#pragma region InputLayOutの登録
	{
		D3D12_INPUT_ELEMENT_DESC *input3DLayOut = new D3D12_INPUT_ELEMENT_DESC[3];

		input3DLayOut[0] =
		{
			"POSITION",
			0,
			DXGI_FORMAT_R32G32B32_FLOAT,
			0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
			0
		};

		input3DLayOut[1] =
		{
			"NORMAL",
			0,
			DXGI_FORMAT_R32G32B32_FLOAT,
			0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
			0
		};

		input3DLayOut[2] =
		{
			"TEXCOORD",
			0,
			DXGI_FORMAT_R32G32_FLOAT,
			0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
			0
		};


		InputLayOutData *input = new InputLayOutData();
		input->inputLayOut = input3DLayOut;
		input->size = 3;
		GraphicsPipeLineMgr::Instance()->RegisterInputLayOutWithData(*input, LAYOUT_POS_NORMAL_TEX);
	};

	{
		D3D12_INPUT_ELEMENT_DESC *input3DLayOut = new D3D12_INPUT_ELEMENT_DESC[2];

		input3DLayOut[0] =
		{
			"POSITION",
			0,
			DXGI_FORMAT_R32G32B32_FLOAT,
			0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
			0
		};

		input3DLayOut[1] =
		{
			"NORMAL",
			0,
			DXGI_FORMAT_R32G32B32_FLOAT,
			0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
			0
		};


		InputLayOutData *input = new InputLayOutData();
		input->inputLayOut = input3DLayOut;
		input->size = 2;
		GraphicsPipeLineMgr::Instance()->RegisterInputLayOutWithData(*input, LAYOUT_POS_NORMAL);
	};

	{
		D3D12_INPUT_ELEMENT_DESC *input2DLayOut = new D3D12_INPUT_ELEMENT_DESC[2];

		input2DLayOut[0] =
		{
		"POSITION",
		0,
		DXGI_FORMAT_R32G32B32_FLOAT,
		0,
		D3D12_APPEND_ALIGNED_ELEMENT,
		D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
		0 };


		input2DLayOut[1] =
		{
		"TEXCOORD",
		0,
		DXGI_FORMAT_R32G32_FLOAT,
		0,
		D3D12_APPEND_ALIGNED_ELEMENT,
		D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
		0
		};


		InputLayOutData *input = new InputLayOutData;
		input->inputLayOut = input2DLayOut;
		input->size = 2;
		GraphicsPipeLineMgr::Instance()->RegisterInputLayOutWithData(*input, LAYOUT_POS_TEX);
	}




	{
		D3D12_INPUT_ELEMENT_DESC *inputColorLayOut = new D3D12_INPUT_ELEMENT_DESC;

		inputColorLayOut[0] =
		{
		"POSITION",
		0,
		DXGI_FORMAT_R32G32B32_FLOAT,
		0,
		D3D12_APPEND_ALIGNED_ELEMENT,
		D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
		0
		};

		InputLayOutData *input = new InputLayOutData;

		input->inputLayOut = inputColorLayOut;
		input->size = 1;
		GraphicsPipeLineMgr::Instance()->RegisterInputLayOutWithData(*input, LAYOUT_POS);
	}




	{
		D3D12_INPUT_ELEMENT_DESC *inputFbxLayOut = new D3D12_INPUT_ELEMENT_DESC[5];

		inputFbxLayOut[0] =
		{
		"POSITION",
		0,
		DXGI_FORMAT_R32G32B32_FLOAT,
		0,
		D3D12_APPEND_ALIGNED_ELEMENT,
		D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
		0 };


		inputFbxLayOut[1] =
		{
		"NORMAL",
		0,
		DXGI_FORMAT_R32G32B32_FLOAT,
		0,
		D3D12_APPEND_ALIGNED_ELEMENT,
		D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
		0
		};

		inputFbxLayOut[2] =
		{
		"TEXCOORD",
		0,
		DXGI_FORMAT_R32G32_FLOAT,
		0,
		D3D12_APPEND_ALIGNED_ELEMENT,
		D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
		0
		};

		inputFbxLayOut[3] =
		{
	"BONEINDICES",
	0,
	DXGI_FORMAT_R32G32B32A32_UINT,
	0,
	D3D12_APPEND_ALIGNED_ELEMENT,
	D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
	0
		};

		inputFbxLayOut[4] =
		{
		"BONEWEIGHTS",
		0,
		DXGI_FORMAT_R32G32B32A32_FLOAT,
		0,
		D3D12_APPEND_ALIGNED_ELEMENT,
		D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
		0
		};


		InputLayOutData *input = new InputLayOutData;

		input->inputLayOut = inputFbxLayOut;
		input->size = 5;
		GraphicsPipeLineMgr::Instance()->RegisterInputLayOutWithData(*input, LAYOUT_POS_NORMAL_TEX_BONE_WEIGHT);
	}

	{
		D3D12_INPUT_ELEMENT_DESC *inputLineLayOut = new D3D12_INPUT_ELEMENT_DESC[2];


		inputLineLayOut[0] =
		{
		"POSITION",
		0,
		DXGI_FORMAT_R32G32B32_FLOAT,
		0,
		D3D12_APPEND_ALIGNED_ELEMENT,
		D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
		0
		};

		inputLineLayOut[1] =
		{
		"TEXCOORD",
		0,
		DXGI_FORMAT_R32G32_FLOAT,
		0,
		D3D12_APPEND_ALIGNED_ELEMENT,
		D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
		0
		};

		InputLayOutData *input = new InputLayOutData;
		input->inputLayOut = inputLineLayOut;
		input->size = 2;
		GraphicsPipeLineMgr::Instance()->RegisterInputLayOutWithData(*input, LAYOUT_POS_TICK);
	}
#pragma endregion


#pragma region シェーダーのコンパイルと登録
	OutputDebugStringA("シェーダーのコンパイルを開始します\n");
	//色シェーダー
	pipelineMgr->RegisterVertexShaderWithData(KazFilePathName::VertexShaderPath + "BasicVertexShader.hlsl", "VSmain", "vs_6_4", SHADER_VERTEX_COLOR);
	pipelineMgr->RegisterPixcelShaderWithData(KazFilePathName::PixelShaderPath + "BasicPixelShader.hlsl", "PSmain", "ps_6_4", SHADER_PIXCEL_COLOR);

	//テクスチャシェーダー...テクスチャ影あり
	pipelineMgr->RegisterVertexShaderWithData(KazFilePathName::VertexShaderPath + "TextureVertexShader.hlsl", "VSmain", "vs_6_4", SHADER_VERTEX_TEXTURE);
	pipelineMgr->RegisterPixcelShaderWithData(KazFilePathName::PixelShaderPath + "TexturePixelShader.hlsl", "PSmain", "ps_6_4", SHADER_PIXCEL_TEXTURE);


	//スプライトシェーダー...テクスチャ影無し
	pipelineMgr->RegisterVertexShaderWithData(KazFilePathName::VertexShaderPath + "SpriteVertexShader.hlsl", "VSmain", "vs_6_4", SHADER_VERTEX_SPRITE);
	pipelineMgr->RegisterPixcelShaderWithData(KazFilePathName::PixelShaderPath + "SpritePixelShader.hlsl", "PSmain", "ps_6_4", SHADER_PIXCEL_SPRITE);


	//objシェーダー
	pipelineMgr->RegisterVertexShaderWithData(KazFilePathName::VertexShaderPath + "OBJVertexShader.hlsl", "VSmain", "vs_6_4", SHADER_VERTEX_OBJ);
	pipelineMgr->RegisterVertexShaderWithData(KazFilePathName::VertexShaderPath + "OBJVertexExpantionShader.hlsl", "VSmain", "vs_6_4", SHADER_VERTEX_EXPANTION_OBJ);	// 頂点拡張用
	pipelineMgr->RegisterVertexShaderWithData(KazFilePathName::VertexShaderPath + "OBJGetShadowMapVertexShader.hlsl", "VSmain", "vs_6_4", SHADER_VERTEX_GET_SHADOWMAP);	// シャドウマップ取得用
	pipelineMgr->RegisterVertexShaderWithData(KazFilePathName::VertexShaderPath + "OBJDrawShadowMapVertexShader.hlsl", "VSmain", "vs_6_4", SHADER_VERTEX_DRAW_SHADOWMAP);	// シャドウマップを元に影描画用
	pipelineMgr->RegisterPixcelShaderWithData(KazFilePathName::PixelShaderPath + "OBJPixelShader.hlsl", "PSmain", "ps_6_4", SHADER_PIXCEL_OBJ);
	pipelineMgr->RegisterPixcelShaderWithData(KazFilePathName::PixelShaderPath + "OBJPixelExpantionShader.hlsl", "PSmain", "ps_6_4", SHADER_PIXCEL_EXPANTION_OBJ);	// 頂点拡張用
	pipelineMgr->RegisterPixcelShaderWithData(KazFilePathName::PixelShaderPath + "OBJGetShadowPixcelShader.hlsl", "PSmain", "ps_6_4", SHADER_PIXCEL_GET_SHADOWMAP);	// シャドウマップ取得用
	pipelineMgr->RegisterPixcelShaderWithData(KazFilePathName::PixelShaderPath + "OBJDrawShadowMapPixcelShader.hlsl", "PSmain", "ps_6_4", SHADER_PIXCEL_DRAW_SHADOWMAP);	// シャドウマップを元に影描画用

	pipelineMgr->RegisterVertexShaderWithData(KazFilePathName::VertexShaderPath + "InstanceObjGetShadowMapVertexShader.hlsl", "VSmain", "vs_6_4", SHADER_VERTEX_INSTANCE_GET_SHADOWMAP);	// シャドウマップ取得用
	pipelineMgr->RegisterPixcelShaderWithData(KazFilePathName::PixelShaderPath + "InstanceObjGetShadowPixelShader.hlsl", "PSmain", "ps_6_4", SHADER_PIXCEL_INSTANCE_GET_SHADOWMAP);	// シャドウマップを元に影描画用

	pipelineMgr->RegisterPixcelShaderWithData(KazFilePathName::PixelShaderPath + "InstanceObjExpantionPixelShader.hlsl", "PSmain", "ps_6_4", SHADER_PIXCEL_INSTANCE_OBJ_EXPANTION);


	//ライト
	pipelineMgr->RegisterVertexShaderWithData(KazFilePathName::VertexShaderPath + "LightVertexShader.hlsl", "VSmain", "vs_6_4", SHADER_VERTEX_LIGHT);
	pipelineMgr->RegisterPixcelShaderWithData(KazFilePathName::PixelShaderPath + "LightPixelShader.hlsl", "PSmain", "ps_6_4", SHADER_PIXCEL_LIGHT);


	//輝度抽出
	pipelineMgr->RegisterVertexShaderWithData(KazFilePathName::VertexShaderPath + "LumiVertexShader.hlsl", "VSmain", "vs_6_4", SHADER_VERTEX_LUMINAS);
	pipelineMgr->RegisterPixcelShaderWithData(KazFilePathName::PixelShaderPath + "LumiPixelShader.hlsl", "PSmain", "ps_6_4", SHADER_PIXCEL_LUMINAS);


	//ガウシアンブラー
	pipelineMgr->RegisterPixcelShaderWithData(KazFilePathName::PixelShaderPath + "GaussianPixelShader.hlsl", "GaussianUpBlur", "ps_6_4", SHADER_PIXCEL_GAUSSIAN_UPBLUR);
	pipelineMgr->RegisterPixcelShaderWithData(KazFilePathName::PixelShaderPath + "GaussianPixelShader.hlsl", "GaussianSideBlur", "ps_6_4", SHADER_PIXCEL_GAUSSIAN_SIDEBLUR);



	//fbx
	pipelineMgr->RegisterVertexShaderWithData(KazFilePathName::VertexShaderPath + "FbxModelVertexShader.hlsl", "VSmain", "vs_6_4", SHADER_VERTEX_FBX);
	pipelineMgr->RegisterVertexShaderWithData(KazFilePathName::VertexShaderPath + "FbxOutPutPosVertexShader.hlsl", "VSmain", "vs_6_4", SHADER_VERTEX_FBX_FOG);
	pipelineMgr->RegisterPixcelShaderWithData(KazFilePathName::PixelShaderPath + "FbxModelPixelShader.hlsl", "PSmain", "ps_6_4", SHADER_PIXCEL_FBX);



	//bloom
	pipelineMgr->RegisterPixcelShaderWithData(KazFilePathName::PixelShaderPath + "BloomPixelShader.hlsl", "PSmain", "ps_6_4", SHADER_PIXCEL_BLOOM);


	//削除予定
	pipelineMgr->RegisterVertexShaderWithData(KazFilePathName::VertexShaderPath + "multipassVertexShader.hlsl", "VSmain", "vs_6_4", SHADER_VERTEX_MULTIPASS);


	//削除予定
	pipelineMgr->RegisterPixcelShaderWithData(KazFilePathName::PixelShaderPath + "GoalLightPixelShader.hlsl", "PSmain", "ps_6_4", SHADER_PIXCEL_GOALLIGHT);



	//Fog
	pipelineMgr->RegisterVertexShaderWithData(KazFilePathName::VertexShaderPath + "FogVertexShader.hlsl", "VSmain", "vs_6_4", SHADER_VERTEX_FOG_COLOR);
	pipelineMgr->RegisterPixcelShaderWithData(KazFilePathName::PixelShaderPath + "FogPixelShader.hlsl", "PSmain", "ps_6_4", SHADER_PIXCEL_FOG_COLOR);


	pipelineMgr->RegisterPixcelShaderWithData(KazFilePathName::PixelShaderPath + "FogObjPixelShader.hlsl", "PSmain", "ps_6_4", SHADER_PIXCEL_FOG_OBJ);


	pipelineMgr->RegisterPixcelShaderWithData(KazFilePathName::PixelShaderPath + "FogFbxPixelShader.hlsl", "PSmain", "ps_6_4", SHADER_PIXCEL_FOG_FBX);


	//太さ変えられる線用ジオメトリーシェーダー
	//pipelineMgr->RegisterGeometoryShaderWithData(KazFilePathName::RelativeShaderPath + "LineGeometryShader.hlsl", "GSmain", "gs_5_0", SHADER_GEOMETORY_LINE);


	//線
	pipelineMgr->RegisterVertexShaderWithData(KazFilePathName::VertexShaderPath + "LineVertexShader.hlsl", "VSmain", "vs_6_4", SHADER_VERTEX_LINE);
	pipelineMgr->RegisterPixcelShaderWithData(KazFilePathName::PixelShaderPath + "LinePixelShader.hlsl", "PSmain", "ps_6_4", SHADER_PIXCEL_LINE);



	pipelineMgr->RegisterPixcelShaderWithData(KazFilePathName::PixelShaderPath + "LineTickFogPixelShader.hlsl", "PSmain", "ps_6_4", SHADER_PIXCEL_TICK_LINE_FOG);



	pipelineMgr->RegisterPixcelShaderWithData(KazFilePathName::PixelShaderPath + "FogTexturePixelShader.hlsl", "PSmain", "ps_6_4", SHADER_PIXCEL_FOG_SPRITE);


	pipelineMgr->RegisterPixcelShaderWithData(KazFilePathName::PixelShaderPath + "FogLinePixelShader.hlsl", "PSmain", "ps_6_4", SHADER_PIXCEL_LINE_FOG);

	//削除予定
	pipelineMgr->RegisterPixcelShaderWithData(KazFilePathName::PixelShaderPath + "FogLineForEffectBGPixelShader.hlsl", "PSmain", "ps_6_4", SHADER_PIXCEL_LINE_FOG_FORBG);

	pipelineMgr->RegisterPixcelShaderWithData(KazFilePathName::PixelShaderPath + "LuminancePixelShader.hlsl", "PSmain", "ps_6_4", SHADER_PIXEL_LUMINANCE);


	pipelineMgr->RegisterVertexShaderWithData(KazFilePathName::VertexShaderPath + "ObjLightVertexShader.hlsl", "VSmain", "vs_6_4", SHADER_VERTEX_OBJ_LIGHT);
	pipelineMgr->RegisterPixcelShaderWithData(KazFilePathName::PixelShaderPath + "ObjLightPixelShader.hlsl", "PSmain", "ps_6_4", SHADER_PIXCEL_OBJ_LIGHT);


	pipelineMgr->RegisterVertexShaderWithData(KazFilePathName::VertexShaderPath + "InstanceVertexShader.hlsl", "VSmain", "vs_6_4", SHADER_VERTEX_INSTANCE_OBJ);
	pipelineMgr->RegisterPixcelShaderWithData(KazFilePathName::PixelShaderPath + "InstancePixelShader.hlsl", "PSmain", "ps_6_4", SHADER_PIXCEL_INSTANCE_OBJ);

	pipelineMgr->RegisterVertexShaderWithData(KazFilePathName::VertexShaderPath + "InstancePosVertexShader.hlsl", "VSmain", "vs_6_4", SHADER_VERTEX_INSTANCE_COLOR);
	pipelineMgr->RegisterPixcelShaderWithData(KazFilePathName::PixelShaderPath + "InstanceColorPixelShader.hlsl", "PSmain", "ps_6_4", SHADER_PIXCEL_INSTANCE_COLOR);


	pipelineMgr->RegisterVertexShaderWithData(KazFilePathName::VertexShaderPath + "InstancePosNormalColorVertexShader.hlsl", "VSmain", "vs_6_4", SHADER_VERTEX_INSTANCE_COLOR_SHADING);
	pipelineMgr->RegisterPixcelShaderWithData(KazFilePathName::PixelShaderPath + "InstanceColorShadingMultiPassPixelShader.hlsl", "PSmain", "ps_6_4", SHADER_PIXCEL_INSTANCE_COLOR_SHADING);


	pipelineMgr->RegisterVertexShaderWithData(KazFilePathName::VertexShaderPath + "InstanceColorGetShadowVertexShader.hlsl", "VSmain", "vs_6_4", SHADER_VERTEX_INSTANCE_COLOR_GET_SHADOWMAP);
	pipelineMgr->RegisterPixcelShaderWithData(KazFilePathName::PixelShaderPath + "InstanceColorGetShadowPixelShader.hlsl", "PSmain", "ps_6_4", SHADER_PIXCEL_INSTANCE_COLOR_GET_SHADOW);

	pipelineMgr->RegisterPixcelShaderWithData(KazFilePathName::PixelShaderPath + "LineUvPixelShader.hlsl", "PSmain", "ps_6_4", SHADER_PIXCEL_LINE_UV);


	pipelineMgr->RegisterPixcelShaderWithData(KazFilePathName::PixelShaderPath + "SpriteWindowPixelShader.hlsl", "PSmain", "ps_6_4", SHADER_PIXCEL_SPRITE_WINDOW);

	//コンピュートシェーダーのコンパイル
	//pipelineMgr->RegisterComputeShaderWithData(KazFilePathName::ComputeShaderPath + "TestComputeShader.hlsl", "CSmain", "cs_6_4", SHADER_COMPUTE_TEST);
	pipelineMgr->RegisterComputeShaderWithData(KazFilePathName::ComputeShaderPath + "FloorParticleComputeShader.hlsl", "CSmain", "cs_6_4", SHADER_COMPUTE_FLOORPARTICLE);
	pipelineMgr->RegisterComputeShaderWithData(KazFilePathName::ComputeShaderPath + "FloorParticleMoveComputeShader.hlsl", "CSmain", "cs_6_4", SHADER_COMPUTE_FLOORPARTICLE_MOVE);
	pipelineMgr->RegisterComputeShaderWithData(KazFilePathName::ComputeShaderPath + "BlockParticleInitComputeShader.hlsl", "CSmain", "cs_6_4", SHADER_COMPUTE_BLOCKPARTICLE);
	pipelineMgr->RegisterComputeShaderWithData(KazFilePathName::ComputeShaderPath + "PortalLineComputeShader.hlsl", "CSmain", "cs_6_4", SHADER_COMPUTE_PORTALLINE);
	pipelineMgr->RegisterComputeShaderWithData(KazFilePathName::ComputeShaderPath + "PortalLineMoveComputeShader.hlsl", "CSmain", "cs_6_4", SHADER_COMPUTE_PORTALLINE_MOVE);
	pipelineMgr->RegisterComputeShaderWithData(KazFilePathName::ComputeShaderPath + "MeshParticleComputeShader.hlsl", "CSmain", "cs_6_4", SHADER_COMPUTE_MESHPARTICLE);
	pipelineMgr->RegisterComputeShaderWithData(KazFilePathName::ComputeShaderPath + "BlockParticleMoveComputeShader.hlsl", "CSmain", "cs_6_4", SHADER_COMPUTE_BLOCKPARTICLE_MOVE);

	pipelineMgr->RegisterPixcelShaderWithData(KazFilePathName::PixelShaderPath + "VHSPixelShader.hlsl", "PSmain", "ps_6_4", SHADER_PIXCEL_WIHITENOISE);

	pipelineMgr->RegisterPixcelShaderWithData(KazFilePathName::PixelShaderPath + "FbxTwoRenderPixelShader.hlsl", "PSmain", "ps_6_4", SHADER_PIXCEL_FBX_RENDER_TWO_LIGHT);
	pipelineMgr->RegisterPixcelShaderWithData(KazFilePathName::PixelShaderPath + "FbxMultiPassPixelShader.hlsl", "PSmain", "ps_6_4", SHADER_PIXCEL_FBX_RENDER_TWO);
	pipelineMgr->RegisterPixcelShaderWithData(KazFilePathName::PixelShaderPath + "GoalEffectPixelShader.hlsl", "PSmain", "ps_6_4", SHADER_PIXCEL_SPRITE_GOAL_EFFECT);


	pipelineMgr->RegisterPixcelShaderWithData(KazFilePathName::PixelShaderPath + "ColorTwoRenderPixelShader.hlsl", "PSmain", "ps_6_4", SHADER_PIXCEL_COLOR_MULTITEX);
	pipelineMgr->RegisterPixcelShaderWithData(KazFilePathName::PixelShaderPath + "InstanceColorMultiPassPixelShader.hlsl", "PSmain", "ps_6_4", SHADER_PIXCEL_INSTANCE_COLOR_MULTITEX);
	pipelineMgr->RegisterPixcelShaderWithData(KazFilePathName::PixelShaderPath + "SpriteTwoRenderPixelShader.hlsl", "PSmain", "ps_6_4", SHADER_PIXCEL_SPRITE_MULTITEX);
	pipelineMgr->RegisterPixcelShaderWithData(KazFilePathName::PixelShaderPath + "ObjTwoRenderPixelShader.hlsl", "PSmain", "ps_6_4", SHADER_PIXCEL_OBJ_MULTITEX);
	pipelineMgr->RegisterPixcelShaderWithData(KazFilePathName::PixelShaderPath + "GoalLightTwoRenderPixelShader.hlsl", "PSmain", "ps_6_4", SHADER_PIXCEL_GOALLIGHT_MULTITEX);
	pipelineMgr->RegisterPixcelShaderWithData(KazFilePathName::PixelShaderPath + "FogColorTwoRenderPixelShader.hlsl", "PSmain", "ps_6_4", SHADER_PIXCEL_FOG_COLOR_MULTITEX);
	pipelineMgr->RegisterPixcelShaderWithData(KazFilePathName::PixelShaderPath + "FogMultiPassPixelShader.hlsl", "PSmain", "ps_6_4", SHADER_PIXCEL_FOG_MULTITEX);
	pipelineMgr->RegisterPixcelShaderWithData(KazFilePathName::PixelShaderPath + "MultiPassLuminancePixelShader.hlsl", "PSmain", "ps_6_4", SHADER_PIXCEL_LUMINANCE_MULTI);
	pipelineMgr->RegisterPixcelShaderWithData(KazFilePathName::PixelShaderPath + "LineUvMultiTexPixelShader.hlsl", "PSmain", "ps_6_4", SHADER_PIXCEL_LINE_UV_MULTITEX);



	
	pipelineMgr->RegisterPixcelShaderWithData(KazFilePathName::PixelShaderPath + "LineFlashEffectPixelShader.hlsl", "PSmain", "ps_6_4", SHADER_PIXCEL_LINE_FLASHEFFECT);

	pipelineMgr->RegisterPixcelShaderWithData(KazFilePathName::PixelShaderPath + "GradationPixelShader.hlsl", "PSmain", "ps_6_4", SHADER_PIXCEL_SPRITE_GRADATION);

	pipelineMgr->RegisterVertexShaderWithData(KazFilePathName::VertexShaderPath + "ObjFogVertexShader.hlsl", "VSmain", "vs_6_4", SHADER_VERTEX_FOG_OBJ);
	pipelineMgr->RegisterPixcelShaderWithData(KazFilePathName::PixelShaderPath + "GradationObjPixelShader.hlsl", "PSmain", "ps_6_4", SHADER_PIXCEL_OBJ_GRADATION);

	pipelineMgr->RegisterVertexShaderWithData(KazFilePathName::VertexShaderPath + "GPUParticleVertexShader.hlsl", "VSmain", "vs_6_4", SHADER_VERTEX_GPUPARTICLE);
	pipelineMgr->RegisterPixcelShaderWithData(KazFilePathName::PixelShaderPath + "GPUParticlePixelShader.hlsl", "PSmain", "ps_6_4", SHADER_PIXEL_GPUPARTICLE);


	pipelineMgr->RegisterVertexShaderWithData(KazFilePathName::VertexShaderPath + "GPUParticlePosUvVertexShader.hlsl", "VSmain", "vs_6_4", SHADER_VERTEX_GPUPARTICLE_POSUV);
	pipelineMgr->RegisterPixcelShaderWithData(KazFilePathName::PixelShaderPath + "GPUParticleTexturePixelShader.hlsl", "PSmain", "ps_6_4", SHADER_PIXEL_GPUPARTICLE_TEX);



	pipelineMgr->RegisterPixcelShaderWithData(KazFilePathName::PixelShaderPath + "NoiseSignalPixelShader.hlsl", "PSmain", "ps_6_4", SHADER_PIXCEL_SPRITE_NOISE);

	pipelineMgr->RegisterVertexShaderWithData(KazFilePathName::VertexShaderPath + "PortalVertexShader.hlsl", "VSmain", "vs_6_4", SHADER_VERTEX_PORTAL);
	pipelineMgr->RegisterPixcelShaderWithData(KazFilePathName::PixelShaderPath + "PortalPixelShader.hlsl", "PSmain", "ps_6_4", SHADER_PIXEL_PORTAL);


	pipelineMgr->RegisterPixcelShaderWithData(KazFilePathName::PixelShaderPath + "SpriteColorPixelShader.hlsl", "PSmain", "ps_6_4", SHADER_PIXEL_SPRITE_COLOR);

	OutputDebugStringA("シェーダーのコンパイルを終了します\n");
#pragma endregion


#pragma region パイプラインの設定の登録

#pragma region ブレンドの設定
	//加算合成
	D3D12_RENDER_TARGET_BLEND_DESC addBlendDesc{};
	addBlendDesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	addBlendDesc.BlendEnable = true;
	addBlendDesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	addBlendDesc.SrcBlendAlpha = D3D12_BLEND_ONE;
	addBlendDesc.DestBlendAlpha = D3D12_BLEND_ZERO;

	addBlendDesc.BlendOp = D3D12_BLEND_OP_ADD;
	addBlendDesc.SrcBlend = D3D12_BLEND_ONE;
	addBlendDesc.DestBlend = D3D12_BLEND_ONE;

	//減算合成
	D3D12_RENDER_TARGET_BLEND_DESC subBlendDesc{};
	subBlendDesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	subBlendDesc.BlendEnable = true;
	subBlendDesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	subBlendDesc.SrcBlendAlpha = D3D12_BLEND_ONE;
	subBlendDesc.DestBlendAlpha = D3D12_BLEND_ZERO;

	subBlendDesc.BlendOp = D3D12_BLEND_OP_REV_SUBTRACT;
	subBlendDesc.SrcBlend = D3D12_BLEND_ONE;
	subBlendDesc.DestBlend = D3D12_BLEND_ONE;

	//反転
	D3D12_RENDER_TARGET_BLEND_DESC revBlendDesc{};
	revBlendDesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;;
	revBlendDesc.BlendEnable = true;
	revBlendDesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	revBlendDesc.SrcBlendAlpha = D3D12_BLEND_ONE;
	revBlendDesc.DestBlendAlpha = D3D12_BLEND_ZERO;

	revBlendDesc.BlendOp = D3D12_BLEND_OP_ADD;
	revBlendDesc.SrcBlend = D3D12_BLEND_INV_DEST_COLOR;
	revBlendDesc.DestBlend = D3D12_BLEND_ZERO;

	//α
	D3D12_RENDER_TARGET_BLEND_DESC alphaBlendDesc{};
	alphaBlendDesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	alphaBlendDesc.BlendEnable = true;
	alphaBlendDesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	alphaBlendDesc.SrcBlendAlpha = D3D12_BLEND_ONE;
	alphaBlendDesc.DestBlendAlpha = D3D12_BLEND_ZERO;

	alphaBlendDesc.BlendOp = D3D12_BLEND_OP_ADD;
	alphaBlendDesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
	alphaBlendDesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;

#pragma endregion


	//テクスチャ用
#pragma region PIPELINE_DATA_BACKCARING_ALPHABLEND
	{
		//パイプラインの設定
		D3D12_GRAPHICS_PIPELINE_STATE_DESC gPipeline{};
		//サンプルマスク
		gPipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
		//ラスタライザ
		//背面カリング、塗りつぶし、深度クリッピング有効
		CD3DX12_RASTERIZER_DESC rasterrize(D3D12_DEFAULT);
		//rasterrize.CullMode = D3D12_CULL_MODE_NONE;
		gPipeline.RasterizerState = rasterrize;
		//ブレンドモード
		gPipeline.BlendState.RenderTarget[0] = alphaBlendDesc;

		//図形の形状
		gPipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

		//その他設定
		gPipeline.NumRenderTargets = 1;
		gPipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
		gPipeline.SampleDesc.Count = 1;

		//デプスステンシルステートの設定
		gPipeline.DepthStencilState.DepthEnable = true;							//深度テストを行う
		gPipeline.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;//書き込み許可
		gPipeline.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;		//小さければOK
		gPipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;							//深度値フォーマット
		GraphicsPipeLineMgr::Instance()->RegisterPipeLineDataWithData(gPipeline, PIPELINE_DATA_BACKCARING_ALPHABLEND);
	}
#pragma endregion


	{
		//パイプラインの設定
		D3D12_GRAPHICS_PIPELINE_STATE_DESC gPipeline{};
		//サンプルマスク
		gPipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
		//ラスタライザ
		//背面カリング、塗りつぶし、深度クリッピング有効
		CD3DX12_RASTERIZER_DESC rasterrize(D3D12_DEFAULT);
		gPipeline.RasterizerState = rasterrize;
		//ブレンドモード
		gPipeline.BlendState.RenderTarget[0] = alphaBlendDesc;
		gPipeline.BlendState.RenderTarget[1] = alphaBlendDesc;
		//gPipeline.BlendState.IndependentBlendEnable = true;

		//図形の形状
		gPipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

		//その他設定
		gPipeline.NumRenderTargets = 2;
		gPipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
		gPipeline.RTVFormats[1] = DXGI_FORMAT_R8G8B8A8_UNORM;
		gPipeline.SampleDesc.Count = 1;

		//デプスステンシルステートの設定
		gPipeline.DepthStencilState.DepthEnable = true;							//深度テストを行う
		gPipeline.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;//書き込み許可
		gPipeline.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;		//小さければOK
		gPipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;							//深度値フォーマット
		GraphicsPipeLineMgr::Instance()->RegisterPipeLineDataWithData(gPipeline, PIPELINE_DATA_BACKCARING_ALPHABLEND_RNEDERTARGET_SECOND);
	}


	{
		//パイプラインの設定
		D3D12_GRAPHICS_PIPELINE_STATE_DESC gPipeline{};
		//サンプルマスク
		gPipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
		//ラスタライザ
		//背面カリング、塗りつぶし、深度クリッピング有効
		CD3DX12_RASTERIZER_DESC rasterrize(D3D12_DEFAULT);
		rasterrize.CullMode = D3D12_CULL_MODE_NONE;
		gPipeline.RasterizerState = rasterrize;
		//ブレンドモード
		gPipeline.BlendState.RenderTarget[0] = alphaBlendDesc;
		gPipeline.BlendState.RenderTarget[1] = alphaBlendDesc;
		//gPipeline.BlendState.IndependentBlendEnable = true;

		//図形の形状
		gPipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

		//その他設定
		gPipeline.NumRenderTargets = 2;
		gPipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
		gPipeline.RTVFormats[1] = DXGI_FORMAT_R8G8B8A8_UNORM;
		gPipeline.SampleDesc.Count = 1;

		//デプスステンシルステートの設定
		gPipeline.DepthStencilState.DepthEnable = true;							//深度テストを行う
		gPipeline.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;//書き込み許可
		gPipeline.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;		//小さければOK
		gPipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;							//深度値フォーマット
		GraphicsPipeLineMgr::Instance()->RegisterPipeLineDataWithData(gPipeline, PIPELINE_DATA_NOCARING_ALPHABLEND_RNEDERTARGET_SECOND);
	}

#pragma region PIPELINE_DATA_BACKCARING_ALPHABLEND_WIREFLAME
	{
		//パイプラインの設定
		D3D12_GRAPHICS_PIPELINE_STATE_DESC gPipeline{};
		//サンプルマスク
		gPipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
		//ラスタライザ
		//背面カリング、塗りつぶし、深度クリッピング有効
		CD3DX12_RASTERIZER_DESC rasterrize(D3D12_DEFAULT);
		rasterrize.FillMode = D3D12_FILL_MODE_WIREFRAME;
		gPipeline.RasterizerState = rasterrize;

		//ブレンドモード
		gPipeline.BlendState.RenderTarget[0] = alphaBlendDesc;

		//図形の形状
		gPipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

		//その他設定
		gPipeline.NumRenderTargets = 1;
		gPipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
		gPipeline.SampleDesc.Count = 1;

		//デプスステンシルステートの設定
		gPipeline.DepthStencilState.DepthEnable = true;							//深度テストを行う
		gPipeline.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;//書き込み許可
		gPipeline.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;		//小さければOK
		gPipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;							//深度値フォーマット
		GraphicsPipeLineMgr::Instance()->RegisterPipeLineDataWithData(gPipeline, PIPELINE_DATA_BACKCARING_ALPHABLEND_WIREFLAME);
	}
#pragma endregion

#pragma region PIPELINE_DATA_NOCARING_ALPHABLEND_WIREFLAME
	{
		//パイプラインの設定
		D3D12_GRAPHICS_PIPELINE_STATE_DESC gPipeline{};
		//サンプルマスク
		gPipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
		//ラスタライザ
		//背面カリング、塗りつぶし、深度クリッピング有効
		CD3DX12_RASTERIZER_DESC rasterrize(D3D12_DEFAULT);
		rasterrize.FillMode = D3D12_FILL_MODE_WIREFRAME;
		rasterrize.CullMode = D3D12_CULL_MODE_NONE;
		gPipeline.RasterizerState = rasterrize;

		//ブレンドモード
		gPipeline.BlendState.RenderTarget[0] = alphaBlendDesc;

		//図形の形状
		gPipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

		//その他設定
		gPipeline.NumRenderTargets = 1;
		gPipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
		gPipeline.SampleDesc.Count = 1;

		//デプスステンシルステートの設定
		gPipeline.DepthStencilState.DepthEnable = true;							//深度テストを行う
		gPipeline.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;//書き込み許可
		gPipeline.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;		//小さければOK
		gPipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;							//深度値フォーマット
		GraphicsPipeLineMgr::Instance()->RegisterPipeLineDataWithData(gPipeline, PIPELINE_DATA_NOCARING_ALPHABLEND_WIREFLAME);
	}
#pragma endregion

#pragma region PIPELINE_DATA_BACKCARING_NOBLEND_WIREFLAME
	{
		//パイプラインの設定
		D3D12_GRAPHICS_PIPELINE_STATE_DESC gPipeline{};
		//サンプルマスク
		gPipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
		//ラスタライザ
		//背面カリング、塗りつぶし、深度クリッピング有効
		CD3DX12_RASTERIZER_DESC rasterrize(D3D12_DEFAULT);
		rasterrize.FillMode = D3D12_FILL_MODE_WIREFRAME;
		gPipeline.RasterizerState = rasterrize;

		//ブレンドモード
		gPipeline.BlendState.RenderTarget[0].BlendEnable = false;

		//図形の形状
		gPipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

		//その他設定
		gPipeline.NumRenderTargets = 1;
		gPipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
		gPipeline.SampleDesc.Count = 1;

		//デプスステンシルステートの設定
		gPipeline.DepthStencilState.DepthEnable = true;							//深度テストを行う
		gPipeline.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;//書き込み許可
		gPipeline.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;		//小さければOK
		gPipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;							//深度値フォーマット
		GraphicsPipeLineMgr::Instance()->RegisterPipeLineDataWithData(gPipeline, PIPELINE_DATA_BACKCARING_NOBLEND_WIREFLAME);
	}
#pragma endregion

#pragma region PIPELINE_DATA_NOCARING_NOBLEND_WIREFLAME
	{
		//パイプラインの設定
		D3D12_GRAPHICS_PIPELINE_STATE_DESC gPipeline{};
		//サンプルマスク
		gPipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
		//ラスタライザ
		//背面カリング、塗りつぶし、深度クリッピング有効
		CD3DX12_RASTERIZER_DESC rasterrize(D3D12_DEFAULT);
		rasterrize.FillMode = D3D12_FILL_MODE_WIREFRAME;
		rasterrize.CullMode = D3D12_CULL_MODE_NONE;
		gPipeline.RasterizerState = rasterrize;

		//ブレンドモード
		gPipeline.BlendState.RenderTarget[0].BlendEnable = false;

		//図形の形状
		gPipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

		//その他設定
		gPipeline.NumRenderTargets = 1;
		gPipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
		gPipeline.SampleDesc.Count = 1;

		//デプスステンシルステートの設定
		gPipeline.DepthStencilState.DepthEnable = true;							//深度テストを行う
		gPipeline.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;//書き込み許可
		gPipeline.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;		//小さければOK
		gPipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;							//深度値フォーマット
		GraphicsPipeLineMgr::Instance()->RegisterPipeLineDataWithData(gPipeline, PIPELINE_DATA_NOCARING_NOBLEND_WIREFLAME);
	}
#pragma endregion

#pragma region PIPELINE_DATA_BACKCARING_ALPHABLEND_WIREFLAME_MULTITEX
	{
		//パイプラインの設定
		D3D12_GRAPHICS_PIPELINE_STATE_DESC gPipeline{};
		//サンプルマスク
		gPipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
		//ラスタライザ
		//背面カリング、塗りつぶし、深度クリッピング有効
		CD3DX12_RASTERIZER_DESC rasterrize(D3D12_DEFAULT);
		rasterrize.FillMode = D3D12_FILL_MODE_WIREFRAME;
		gPipeline.RasterizerState = rasterrize;

		//ブレンドモード
		gPipeline.BlendState.RenderTarget[0] = alphaBlendDesc;
		gPipeline.BlendState.RenderTarget[1] = alphaBlendDesc;

		//図形の形状
		gPipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

		//その他設定
		gPipeline.NumRenderTargets = 2;
		gPipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
		gPipeline.RTVFormats[1] = DXGI_FORMAT_R8G8B8A8_UNORM;
		gPipeline.SampleDesc.Count = 1;

		//デプスステンシルステートの設定
		gPipeline.DepthStencilState.DepthEnable = true;							//深度テストを行う
		gPipeline.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;//書き込み許可
		gPipeline.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;		//小さければOK
		gPipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;							//深度値フォーマット
		GraphicsPipeLineMgr::Instance()->RegisterPipeLineDataWithData(gPipeline, PIPELINE_DATA_BACKCARING_ALPHABLEND_WIREFLAME_MUTITEX);
	}
#pragma endregion


#pragma region PIPELINE_DATA_NOCARING_NOBLEND_WIREFLAME_MULTIPASS_TWO
	{
		//パイプラインの設定
		D3D12_GRAPHICS_PIPELINE_STATE_DESC gPipeline{};
		//サンプルマスク
		gPipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
		//ラスタライザ
		//背面カリング、塗りつぶし、深度クリッピング有効
		CD3DX12_RASTERIZER_DESC rasterrize(D3D12_DEFAULT);
		rasterrize.FillMode = D3D12_FILL_MODE_WIREFRAME;
		rasterrize.CullMode = D3D12_CULL_MODE_NONE;
		gPipeline.RasterizerState = rasterrize;

		//ブレンドモード
		gPipeline.BlendState.RenderTarget[0].BlendEnable = false;
		gPipeline.BlendState.RenderTarget[1].BlendEnable = false;

		//図形の形状
		gPipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

		//その他設定
		gPipeline.NumRenderTargets = 2;
		gPipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
		gPipeline.RTVFormats[1] = DXGI_FORMAT_R8G8B8A8_UNORM;
		gPipeline.SampleDesc.Count = 1;

		//デプスステンシルステートの設定
		gPipeline.DepthStencilState.DepthEnable = true;							//深度テストを行う
		gPipeline.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;//書き込み許可
		gPipeline.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;		//小さければOK
		gPipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;							//深度値フォーマット
		GraphicsPipeLineMgr::Instance()->RegisterPipeLineDataWithData(gPipeline, PIPELINE_DATA_NOCARING_NOBLEND_WIREFLAME_MULTIPASS_TWO);
	}
#pragma endregion

	//スプライト用
#pragma region PIPELINE_DATA_NOCARING_BLENDALPHA
	{
		D3D12_GRAPHICS_PIPELINE_STATE_DESC gPipeline{};
		//スプライト用
		//サンプルマスク
		gPipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;

		//ラスタライザ
		//背面カリング、塗りつぶし、深度クリッピング有効
		CD3DX12_RASTERIZER_DESC rasterrize(D3D12_DEFAULT);
		rasterrize.CullMode = D3D12_CULL_MODE_NONE;
		gPipeline.RasterizerState = rasterrize;


		gPipeline.BlendState.RenderTarget[0] = alphaBlendDesc;
		gPipeline.BlendState.AlphaToCoverageEnable = false;

		//図形の形状
		gPipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;


		//その他設定
		gPipeline.NumRenderTargets = 1;
		gPipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
		gPipeline.SampleDesc.Count = 1;


		//デプスステンシルステートの設定
		gPipeline.DepthStencilState.DepthEnable = true;							//深度テストを行う
		gPipeline.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;//書き込み許可
		gPipeline.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
		gPipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;							//深度値フォーマット
		GraphicsPipeLineMgr::Instance()->RegisterPipeLineDataWithData(gPipeline, PIPELINE_DATA_NOCARING_BLENDALPHA);
	}
#pragma endregion

	//Obj用
#pragma region PIPELINE_DATA_NOCARING_NOBLEND
	//Obj用のパイプラインの設定
	{
		D3D12_GRAPHICS_PIPELINE_STATE_DESC gPipeline{};
		D3D12_RENDER_TARGET_BLEND_DESC blendDesc{};
		//サンプルマスク
		gPipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;

		//ラスタライザ
		//背面カリング、塗りつぶし、深度クリッピング有効
		CD3DX12_RASTERIZER_DESC rasterrize(D3D12_DEFAULT);
		rasterrize.CullMode = D3D12_CULL_MODE_NONE;
		gPipeline.RasterizerState = rasterrize;

		//ブレンドモード
		blendDesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
		gPipeline.BlendState.RenderTarget[0] = blendDesc;

		//図形の形状
		gPipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

		//その他設定
		gPipeline.NumRenderTargets = 1;
		gPipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
		gPipeline.SampleDesc.Count = 1;

		//デプスステンシルステートの設定
		gPipeline.DepthStencilState.DepthEnable = true;							//深度テストを行う
		gPipeline.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;//書き込み許可
		gPipeline.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;		//小さければOK
		gPipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;							//深度値フォーマット
		GraphicsPipeLineMgr::Instance()->RegisterPipeLineDataWithData(gPipeline, PIPELINE_DATA_NOCARING_NOBLEND);
	}

#pragma endregion

	//Obj用
#pragma region PIPELINE_DATA_NOCARING_NOBLEND_R32
	//Obj用のパイプラインの設定
	{
		D3D12_GRAPHICS_PIPELINE_STATE_DESC gPipeline{};
		D3D12_RENDER_TARGET_BLEND_DESC blendDesc{};
		//サンプルマスク
		gPipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;

		//ラスタライザ
		//背面カリング、塗りつぶし、深度クリッピング有効
		CD3DX12_RASTERIZER_DESC rasterrize(D3D12_DEFAULT);
		gPipeline.RasterizerState = rasterrize;
		gPipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;

		//ブレンドモード
		blendDesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
		gPipeline.BlendState.RenderTarget[0] = blendDesc;

		//図形の形状
		gPipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

		//その他設定
		gPipeline.NumRenderTargets = 1;
		gPipeline.RTVFormats[0] = DXGI_FORMAT_R32_FLOAT;
		gPipeline.SampleDesc.Count = 1;

		//デプスステンシルステートの設定
		gPipeline.DepthStencilState.DepthEnable = true;							//深度テストを行う
		gPipeline.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;//書き込み許可
		gPipeline.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;		//小さければOK
		gPipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;							//深度値フォーマット
		GraphicsPipeLineMgr::Instance()->RegisterPipeLineDataWithData(gPipeline, PIPELINE_DATA_NOCARING_NOBLEND_R32);
	}

#pragma endregion

	//Line用
#pragma region PIPELINE_DATA_NOCARING_ALPHABLEND_LINELIST
	{
		D3D12_GRAPHICS_PIPELINE_STATE_DESC gPipeline{};
		D3D12_RENDER_TARGET_BLEND_DESC blendDesc{};
		//サンプルマスク
		gPipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;

		//ラスタライザ
		//背面カリング、塗りつぶし、深度クリッピング有効
		CD3DX12_RASTERIZER_DESC rasterrize(D3D12_DEFAULT);
		gPipeline.RasterizerState = rasterrize;
		gPipeline.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
		gPipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;

		//ブレンドモード
		blendDesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
		gPipeline.BlendState.RenderTarget[0] = alphaBlendDesc;

		//図形の形状
		gPipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE;

		//その他設定
		gPipeline.NumRenderTargets = 1;
		gPipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
		gPipeline.SampleDesc.Count = 1;

		//デプスステンシルステートの設定
		gPipeline.DepthStencilState.DepthEnable = true;							//深度テストを行う
		gPipeline.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;//書き込み許可
		gPipeline.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;		//小さければOK
		gPipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;							//深度値フォーマット
		GraphicsPipeLineMgr::Instance()->RegisterPipeLineDataWithData(gPipeline, PIPELINE_DATA_NOCARING_ALPHABLEND_LINE);
	}
#pragma endregion

	//Line用
#pragma region PIPELINE_DATA_NOCARING_NOBLEND_LINELIST
	{
		D3D12_GRAPHICS_PIPELINE_STATE_DESC gPipeline{};
		D3D12_RENDER_TARGET_BLEND_DESC blendDesc{};
		//サンプルマスク
		gPipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;

		//ラスタライザ
		//背面カリング、塗りつぶし、深度クリッピング有効
		CD3DX12_RASTERIZER_DESC rasterrize(D3D12_DEFAULT);
		gPipeline.RasterizerState = rasterrize;
		gPipeline.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
		gPipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;

		//ブレンドモード
		blendDesc.BlendEnable = false;

		//図形の形状
		gPipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE;

		//その他設定
		gPipeline.NumRenderTargets = 1;
		gPipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
		gPipeline.SampleDesc.Count = 1;

		//デプスステンシルステートの設定
		gPipeline.DepthStencilState.DepthEnable = true;							//深度テストを行う
		gPipeline.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;//書き込み許可
		gPipeline.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;		//小さければOK
		gPipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;							//深度値フォーマット
		GraphicsPipeLineMgr::Instance()->RegisterPipeLineDataWithData(gPipeline, PIPELINE_DATA_NOCARING_NOBLEND_LINE);
	}
#pragma endregion

	//LineDepth用
#pragma region PIPELINE_DATA_NOCARING_NOBLEND_LINE_Z_ALWAYS
	{
		D3D12_GRAPHICS_PIPELINE_STATE_DESC gPipeline{};
		D3D12_RENDER_TARGET_BLEND_DESC blendDesc{};
		//サンプルマスク
		gPipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;

		//ラスタライザ
		//背面カリング、塗りつぶし、深度クリッピング有効
		CD3DX12_RASTERIZER_DESC rasterrize(D3D12_DEFAULT);
		gPipeline.RasterizerState = rasterrize;
		gPipeline.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
		gPipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;

		//ブレンドモード
		blendDesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
		gPipeline.BlendState.RenderTarget[0] = alphaBlendDesc;


		//図形の形状
		gPipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE;

		//その他設定
		gPipeline.NumRenderTargets = 1;
		gPipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
		gPipeline.SampleDesc.Count = 1;

		//デプスステンシルステートの設定
		gPipeline.DepthStencilState.DepthEnable = true;							//深度テストを行う
		gPipeline.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;//書き込み許可
		gPipeline.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_ALWAYS;		//小さければOK
		gPipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;							//深度値フォーマット
		GraphicsPipeLineMgr::Instance()->RegisterPipeLineDataWithData(gPipeline, PIPELINE_DATA_NOCARING_ALPHABLEND_LINE_Z_ALWAYS);
	}
#pragma endregion

#pragma region PIPELINE_DATA_NOCARING_ALPHABLEND_LINELIST_MULTITEX
	{
		D3D12_GRAPHICS_PIPELINE_STATE_DESC gPipeline{};
		D3D12_RENDER_TARGET_BLEND_DESC blendDesc{};
		//サンプルマスク
		gPipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;

		//ラスタライザ
		//背面カリング、塗りつぶし、深度クリッピング有効
		CD3DX12_RASTERIZER_DESC rasterrize(D3D12_DEFAULT);
		gPipeline.RasterizerState = rasterrize;
		gPipeline.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
		gPipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;

		//ブレンドモード
		blendDesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
		gPipeline.BlendState.RenderTarget[0] = alphaBlendDesc;
		gPipeline.BlendState.RenderTarget[1] = alphaBlendDesc;

		//図形の形状
		gPipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE;

		//その他設定
		gPipeline.NumRenderTargets = 2;
		gPipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
		gPipeline.RTVFormats[1] = DXGI_FORMAT_R8G8B8A8_UNORM;
		gPipeline.SampleDesc.Count = 1;

		//デプスステンシルステートの設定
		gPipeline.DepthStencilState.DepthEnable = true;							//深度テストを行う
		gPipeline.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;//書き込み許可
		gPipeline.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;		//小さければOK
		gPipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;							//深度値フォーマット
		GraphicsPipeLineMgr::Instance()->RegisterPipeLineDataWithData(gPipeline, PIPELINE_DATA_NOCARING_ALPHABLEND_LINE_MULTITEX);
	}
#pragma endregion


	//スプライトの加算合成用
#pragma region PIPELINE_DATA_NOCARING_ADDBLEND
	{
		D3D12_GRAPHICS_PIPELINE_STATE_DESC gPipeline{};
		//サンプルマスク
		gPipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;

		//ラスタライザ
		//塗りつぶし、深度クリッピング有効
		CD3DX12_RASTERIZER_DESC rasterrize(D3D12_DEFAULT);
		gPipeline.RasterizerState = rasterrize;
		gPipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;

		//ブレンドモード
		gPipeline.BlendState.RenderTarget[0] = addBlendDesc;
		gPipeline.BlendState.AlphaToCoverageEnable = false;


		//図形の形状
		gPipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

		//その他設定
		gPipeline.NumRenderTargets = 1;
		gPipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
		gPipeline.SampleDesc.Count = 1;

		//デプスステンシルステートの設定
		gPipeline.DepthStencilState.DepthEnable = false;							//深度テストを行う
		gPipeline.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;//書き込み許可
		gPipeline.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_ALWAYS;		//小さければOK
		gPipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;							//深度値フォーマット
		GraphicsPipeLineMgr::Instance()->RegisterPipeLineDataWithData(gPipeline, PIPELINE_DATA_NOCARING_ADDBLEND_CUTALPHA);
	}
#pragma endregion


	//スプライト用αカット
#pragma region PIPELINE_DATA_NOCARING_BLENDALPHA_CUT
	{
		D3D12_GRAPHICS_PIPELINE_STATE_DESC gPipeline{};
		//スプライト用
		//サンプルマスク
		gPipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;

		//ラスタライザ
		//背面カリング、塗りつぶし、深度クリッピング有効
		CD3DX12_RASTERIZER_DESC rasterrize(D3D12_DEFAULT);
		gPipeline.RasterizerState = rasterrize;
		gPipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;

		gPipeline.BlendState.RenderTarget[0] = alphaBlendDesc;
		gPipeline.BlendState.AlphaToCoverageEnable = true;

		//図形の形状
		gPipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;


		//その他設定
		gPipeline.NumRenderTargets = 1;
		gPipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
		gPipeline.SampleDesc.Count = 1;


		//デプスステンシルステートの設定
		gPipeline.DepthStencilState.DepthEnable = true;							//深度テストを行う
		gPipeline.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;//書き込み許可
		gPipeline.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
		gPipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;							//深度値フォーマット
		GraphicsPipeLineMgr::Instance()->RegisterPipeLineDataWithData(gPipeline, PIPELINE_DATA_NOCARING_BLENDALPHA_CUT);
	}
#pragma endregion

#pragma region PIPELINE_DATA_NOCARING_BLENDALPHA_CUT_MULTITEX
	{
		D3D12_GRAPHICS_PIPELINE_STATE_DESC gPipeline{};
		//スプライト用
		//サンプルマスク
		gPipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;

		//ラスタライザ
		//背面カリング、塗りつぶし、深度クリッピング有効
		CD3DX12_RASTERIZER_DESC rasterrize(D3D12_DEFAULT);
		gPipeline.RasterizerState = rasterrize;
		gPipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;

		gPipeline.BlendState.RenderTarget[0] = alphaBlendDesc;
		gPipeline.BlendState.RenderTarget[1] = alphaBlendDesc;
		gPipeline.BlendState.AlphaToCoverageEnable = true;

		//図形の形状
		gPipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;


		//その他設定
		gPipeline.NumRenderTargets = 2;
		gPipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
		gPipeline.RTVFormats[1] = DXGI_FORMAT_R8G8B8A8_UNORM;
		gPipeline.SampleDesc.Count = 1;


		//デプスステンシルステートの設定
		gPipeline.DepthStencilState.DepthEnable = true;							//深度テストを行う
		gPipeline.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;//書き込み許可
		gPipeline.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
		gPipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;							//深度値フォーマット
		GraphicsPipeLineMgr::Instance()->RegisterPipeLineDataWithData(gPipeline, PIPELINE_DATA_NOCARING_BLENDALPHA_CUT_MULTITEX);
	}
#pragma endregion


	//スプライト深度無効用
#pragma region PIPELINE_DATA_NOCARING_BLENDALPHA_DEPTHOFF
	{
		D3D12_GRAPHICS_PIPELINE_STATE_DESC gPipeline{};
		//スプライト用
		//サンプルマスク
		gPipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;

		//ラスタライザ
		//背面カリング、塗りつぶし、深度クリッピング有効
		CD3DX12_RASTERIZER_DESC rasterrize(D3D12_DEFAULT);
		gPipeline.RasterizerState = rasterrize;
		gPipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;

		gPipeline.BlendState.RenderTarget[0] = alphaBlendDesc;
		gPipeline.BlendState.AlphaToCoverageEnable = false;

		//図形の形状
		gPipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;


		//その他設定
		gPipeline.NumRenderTargets = 1;
		gPipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
		gPipeline.SampleDesc.Count = 1;


		//デプスステンシルステートの設定
		gPipeline.DepthStencilState.DepthEnable = false;							//深度テストを行う
		gPipeline.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;//書き込み許可
		gPipeline.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
		gPipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;							//深度値フォーマット
		GraphicsPipeLineMgr::Instance()->RegisterPipeLineDataWithData(gPipeline, PIPELINE_DATA_NOCARING_BLENDALPHA_DEPTHOFF);
	}
#pragma endregion


	//頂点拡張用の前面カリング
#pragma region PIPELINE_DATA_EXPANTION_VERTEX
	{
		{
			D3D12_GRAPHICS_PIPELINE_STATE_DESC gPipeline{};
			D3D12_RENDER_TARGET_BLEND_DESC blendDesc{};
			//サンプルマスク
			gPipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;

			//ラスタライザ
			//前面カリング、塗りつぶし、深度クリッピング有効
			CD3DX12_RASTERIZER_DESC rasterrize(D3D12_DEFAULT);
			gPipeline.RasterizerState = rasterrize;
			gPipeline.RasterizerState.CullMode = D3D12_CULL_MODE_FRONT;

			//ブレンドモード
			blendDesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
			gPipeline.BlendState.RenderTarget[0] = blendDesc;

			//図形の形状
			gPipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

			//その他設定
			gPipeline.NumRenderTargets = 1;
			gPipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
			gPipeline.SampleDesc.Count = 1;

			//デプスステンシルステートの設定
			gPipeline.DepthStencilState.DepthEnable = true;							//深度テストを行う
			gPipeline.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;//書き込み許可
			gPipeline.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;		//小さければOK
			gPipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;							//深度値フォーマット
			GraphicsPipeLineMgr::Instance()->RegisterPipeLineDataWithData(gPipeline, PIPELINE_DATA_EXPANTION_VERTEX);
		}
	}
#pragma endregion


#pragma region PIPELINE_DATA_NOCARING_BLENDALPHA_DEPTH_ALWAYS
	{
		D3D12_GRAPHICS_PIPELINE_STATE_DESC gPipeline{};
		//スプライト用
		//サンプルマスク
		gPipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;

		//ラスタライザ
		//背面カリング、塗りつぶし、深度クリッピング有効
		CD3DX12_RASTERIZER_DESC rasterrize(D3D12_DEFAULT);
		gPipeline.RasterizerState = rasterrize;
		gPipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;

		gPipeline.BlendState.RenderTarget[0] = alphaBlendDesc;

		//図形の形状
		gPipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;


		//その他設定
		gPipeline.NumRenderTargets = 1;
		gPipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
		gPipeline.SampleDesc.Count = 1;


		//デプスステンシルステートの設定
		gPipeline.DepthStencilState.DepthEnable = true;							//深度テストを行う
		gPipeline.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;//書き込み許可
		gPipeline.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_ALWAYS;
		gPipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;							//深度値フォーマット
		GraphicsPipeLineMgr::Instance()->RegisterPipeLineDataWithData(gPipeline, PIPELINE_DATA_NOCARING_BLENDALPHA_DEPTH_ALWAYS);
	}
#pragma endregion

	//スプライト用
#pragma region PIPELINE_DATA_NOCARING_CUTALPHA_DEPTH_ALWAYS
	{
		D3D12_GRAPHICS_PIPELINE_STATE_DESC gPipeline{};
		//スプライト用
		//サンプルマスク
		gPipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;

		//ラスタライザ
		//背面カリング、塗りつぶし、深度クリッピング有効
		CD3DX12_RASTERIZER_DESC rasterrize(D3D12_DEFAULT);
		gPipeline.RasterizerState = rasterrize;
		gPipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;

		gPipeline.BlendState.RenderTarget[0] = alphaBlendDesc;
		gPipeline.BlendState.AlphaToCoverageEnable = true;

		//図形の形状
		gPipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;


		//その他設定
		gPipeline.NumRenderTargets = 1;
		gPipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
		gPipeline.SampleDesc.Count = 1;


		//デプスステンシルステートの設定
		gPipeline.DepthStencilState.DepthEnable = true;							//深度テストを行う
		gPipeline.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;//書き込み許可
		gPipeline.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_ALWAYS;
		gPipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;							//深度値フォーマット
		GraphicsPipeLineMgr::Instance()->RegisterPipeLineDataWithData(gPipeline, PIPELINE_DATA_NOCARING_CUTALPHA_DEPTH_ALWAYS);
	}
#pragma endregion


#pragma region PIPELINE_DATA_NOCARING_BLENDALPHA
	{
		D3D12_GRAPHICS_PIPELINE_STATE_DESC gPipeline{};
		//スプライト用
		//サンプルマスク
		gPipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;

		//ラスタライザ
		//背面カリング、塗りつぶし、深度クリッピング有効
		CD3DX12_RASTERIZER_DESC rasterrize(D3D12_DEFAULT);
		gPipeline.RasterizerState = rasterrize;
		gPipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;

		gPipeline.BlendState.RenderTarget[0] = alphaBlendDesc;
		gPipeline.BlendState.AlphaToCoverageEnable = false;

		//図形の形状
		gPipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;


		//その他設定
		gPipeline.NumRenderTargets = 1;
		gPipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
		gPipeline.SampleDesc.Count = 1;


		//デプスステンシルステートの設定
		gPipeline.DepthStencilState.DepthEnable = true;							//深度テストを行う
		gPipeline.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;//書き込み許可
		gPipeline.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
		gPipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;							//深度値フォーマット
		GraphicsPipeLineMgr::Instance()->RegisterPipeLineDataWithData(gPipeline, PIPELINE_DATA_NOCARING_BLENDALPHA_DEPTH_LESSEQUAL);
	}
#pragma endregion




	//スプライト用
#pragma region PIPELINE_DATA_NOCARING_BLENDALPHA_ZOFF
	{
		D3D12_GRAPHICS_PIPELINE_STATE_DESC gPipeline{};
		//スプライト用
		//サンプルマスク
		gPipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;

		//ラスタライザ
		//背面カリング、塗りつぶし、深度クリッピング有効
		CD3DX12_RASTERIZER_DESC rasterrize(D3D12_DEFAULT);
		gPipeline.RasterizerState = rasterrize;
		gPipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;


		//加算とα合成
		D3D12_RENDER_TARGET_BLEND_DESC addAndAlphaBlendDesc{};
		addAndAlphaBlendDesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
		addAndAlphaBlendDesc.BlendEnable = true;
		addAndAlphaBlendDesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
		addAndAlphaBlendDesc.SrcBlendAlpha = D3D12_BLEND_ONE;
		addAndAlphaBlendDesc.DestBlendAlpha = D3D12_BLEND_ZERO;

		addAndAlphaBlendDesc.BlendOp = D3D12_BLEND_OP_ADD;
		addAndAlphaBlendDesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
		addAndAlphaBlendDesc.DestBlend = D3D12_BLEND_ONE;

		//D3D12_BLEND_ONE;
		//alphaBlendDesc.BlendOp = D3D12_BLEND_OP_ADD;
		//alphaBlendDesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
		//alphaBlendDesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;


		gPipeline.BlendState.RenderTarget[0] = addAndAlphaBlendDesc;
		gPipeline.BlendState.AlphaToCoverageEnable = false;

		//図形の形状
		gPipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;


		//その他設定
		gPipeline.NumRenderTargets = 1;
		gPipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
		gPipeline.SampleDesc.Count = 1;


		//デプスステンシルステートの設定
		gPipeline.DepthStencilState.DepthEnable = true;							//深度テストを行う
		gPipeline.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;//書き込み許可
		gPipeline.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_ALWAYS;
		gPipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;							//深度値フォーマット
		GraphicsPipeLineMgr::Instance()->RegisterPipeLineDataWithData(gPipeline, PIPELINE_DATA_NOCARING_BLENDALPHA_DEPTH_OFF);
	}
#pragma endregion


	//スプライトの加算合成用 αあり
#pragma region PIPELINE_DATA_NOCARING_ADDBLEND
	{
		D3D12_GRAPHICS_PIPELINE_STATE_DESC gPipeline{};
		//サンプルマスク
		gPipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;

		//ラスタライザ
		//塗りつぶし、深度クリッピング有効
		CD3DX12_RASTERIZER_DESC rasterrize(D3D12_DEFAULT);
		gPipeline.RasterizerState = rasterrize;
		gPipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;

		//ブレンドモード
		gPipeline.BlendState.RenderTarget[0] = addBlendDesc;
		gPipeline.BlendState.AlphaToCoverageEnable = false;


		//図形の形状
		gPipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

		//その他設定
		gPipeline.NumRenderTargets = 1;
		gPipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
		gPipeline.SampleDesc.Count = 1;

		//デプスステンシルステートの設定
		gPipeline.DepthStencilState.DepthEnable = true;							//深度テストを行う
		gPipeline.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;//書き込み許可
		gPipeline.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;		//小さければOK
		gPipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;							//深度値フォーマット
		GraphicsPipeLineMgr::Instance()->RegisterPipeLineDataWithData(gPipeline, PIPELINE_DATA_NOCARING_ADDBLEND);
	}
#pragma endregion



	//マルチパスのスプライト用
#pragma region PIPELINE_DATA_NOCARING_BLENDALPHA_RENDERTARGET_TWO
	{
		D3D12_GRAPHICS_PIPELINE_STATE_DESC gPipeline{};
		//スプライト用
		//サンプルマスク
		gPipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;

		//ラスタライザ
		//背面カリング、塗りつぶし、深度クリッピング有効
		CD3DX12_RASTERIZER_DESC rasterrize(D3D12_DEFAULT);
		gPipeline.RasterizerState = rasterrize;
		gPipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;

		//ブレンドモード
		gPipeline.BlendState.RenderTarget[0] = alphaBlendDesc;
		gPipeline.BlendState.RenderTarget[1] = alphaBlendDesc;
		//gPipeline.BlendState.IndependentBlendEnable = true;

		//図形の形状
		gPipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

		//その他設定
		gPipeline.NumRenderTargets = 2;
		gPipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
		gPipeline.RTVFormats[1] = DXGI_FORMAT_R8G8B8A8_UNORM;
		gPipeline.SampleDesc.Count = 1;



		//デプスステンシルステートの設定
		gPipeline.DepthStencilState.DepthEnable = true;							//深度テストを行う
		gPipeline.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;//書き込み許可
		gPipeline.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
		gPipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;							//深度値フォーマット
		GraphicsPipeLineMgr::Instance()->RegisterPipeLineDataWithData(gPipeline, PIPELINE_DATA_NOCARING_BLENDALPHA_MULTIPASS_TWO);
	}
#pragma endregion







	//コンピュートパイプライン
	D3D12_COMPUTE_PIPELINE_STATE_DESC desc = {};
	desc.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;
	desc.NodeMask = 0;
	GraphicsPipeLineMgr::Instance()->RegisterComputePipeLineDataWithData(desc, PIPELINE_COMPUTE_DATA_TEST);

#pragma endregion


#pragma region パイプラインの生成と登録

	//コンピュートパイプラインの作成
	/*GraphicsPipeLineMgr::Instance()->CreateComputePipeLine(
		SHADER_COMPUTE_TEST,
		PIPELINE_COMPUTE_DATA_TEST,
		ROOTSIGNATURE_DATA_SRV_UAV,
		PIPELINE_COMPUTE_NAME_TEST
	);*/

	//床に散らばらっているパーティクル
	GraphicsPipeLineMgr::Instance()->CreateComputePipeLine(
		SHADER_COMPUTE_FLOORPARTICLE,
		PIPELINE_COMPUTE_DATA_TEST,
		ROOTSIGNATURE_DATA_UAV,
		PIPELINE_COMPUTE_NAME_FLOORPARTICLE
	);

	GraphicsPipeLineMgr::Instance()->CreateComputePipeLine(
		SHADER_COMPUTE_FLOORPARTICLE_MOVE,
		PIPELINE_COMPUTE_DATA_TEST,
		ROOTSIGNATURE_DATA_DRAW_UAB_CB,
		PIPELINE_COMPUTE_NAME_FLOORPARTICLE_MOVE
	);

	GraphicsPipeLineMgr::Instance()->CreateComputePipeLine(
		SHADER_COMPUTE_PORTALLINE,
		PIPELINE_COMPUTE_DATA_TEST,
		ROOTSIGNATURE_DATA_DRAW_UAB_CB,
		PIPELINE_COMPUTE_NAME_PORTALLINE
	);

	GraphicsPipeLineMgr::Instance()->CreateComputePipeLine(
		SHADER_COMPUTE_PORTALLINE_MOVE,
		PIPELINE_COMPUTE_DATA_TEST,
		ROOTSIGNATURE_DATA_DRAW_UAB_CB,
		PIPELINE_COMPUTE_NAME_PORTALLINE_MOVE
	);

	

	//ブロックにちりばめられるパーティクル
	GraphicsPipeLineMgr::Instance()->CreateComputePipeLine(
		SHADER_COMPUTE_BLOCKPARTICLE,
		PIPELINE_COMPUTE_DATA_TEST,
		ROOTSIGNATURE_DATA_DRAW_UAB_CB,
		PIPELINE_COMPUTE_NAME_BLOCKPARTICLE
	);

	//エミッターの場所を決める
	GraphicsPipeLineMgr::Instance()->CreateComputePipeLine(
		SHADER_COMPUTE_MESHPARTICLE,
		PIPELINE_COMPUTE_DATA_TEST,
		ROOTSIGNATURE_DATA_UAB_UAB_UAB_CB,
		PIPELINE_COMPUTE_NAME_MESHPARTICLE
	);

	GraphicsPipeLineMgr::Instance()->CreateComputePipeLine(
		SHADER_COMPUTE_BLOCKPARTICLE_MOVE,
		PIPELINE_COMPUTE_DATA_TEST,
		ROOTSIGNATURE_DATA_DRAW_UAB_CB,
		PIPELINE_COMPUTE_NAME_BLOCKPARTICLE_MOVE
	);


	//GPUパーティクル用のパイプライン
	GraphicsPipeLineMgr::Instance()->CreatePipeLine(
		LAYOUT_POS_TEX,
		SHADER_VERTEX_GPUPARTICLE,
		SHADER_PIXEL_GPUPARTICLE,
		PIPELINE_DATA_NOCARING_BLENDALPHA_MULTIPASS_TWO,
		ROOTSIGNATURE_DATA_DRAW_UAV,
		PIPELINE_NAME_GPUPARTICLE
	);

	//GPUパーティクル用のパイプライン
	GraphicsPipeLineMgr::Instance()->CreatePipeLine(
		LAYOUT_POS_TEX,
		SHADER_VERTEX_GPUPARTICLE_POSUV,
		SHADER_PIXEL_GPUPARTICLE_TEX,
		PIPELINE_DATA_NOCARING_BLENDALPHA_MULTIPASS_TWO,
		ROOTSIGNATURE_DATA_DRAW_UAB_TEX,
		PIPELINE_NAME_GPUPARTICLE_TEX
	);



	//ポータルのGPUパーティクル用のパイプライン
	GraphicsPipeLineMgr::Instance()->CreatePipeLine(
		LAYOUT_POS_TEX,
		SHADER_VERTEX_GPUPARTICLE,
		SHADER_PIXEL_GPUPARTICLE,
		PIPELINE_DATA_NOCARING_BLENDALPHA_MULTIPASS_TWO,
		ROOTSIGNATURE_DATA_DRAW_UAV,
		PIPELINE_NAME_GPUPARTICLE_PORTAL
	);




	//色パイプライン
	GraphicsPipeLineMgr::Instance()->CreatePipeLine(
		LAYOUT_POS,
		SHADER_VERTEX_COLOR,
		SHADER_PIXCEL_COLOR,
		PIPELINE_DATA_BACKCARING_ALPHABLEND,
		ROOTSIGNATURE_DATA_DRAW,
		PIPELINE_NAME_COLOR
	);

	GraphicsPipeLineMgr::Instance()->CreatePipeLine(
		LAYOUT_POS,
		SHADER_VERTEX_COLOR,
		SHADER_PIXCEL_COLOR,
		PIPELINE_DATA_NOCARING_ALPHABLEND_LINE_Z_ALWAYS,
		ROOTSIGNATURE_DATA_DRAW,
		PIPELINE_NAME_COLOR_LINE_Z_ALWAYS
	);

	GraphicsPipeLineMgr::Instance()->CreatePipeLine(
		LAYOUT_POS_TEX,
		SHADER_VERTEX_PORTAL,
		SHADER_PIXEL_PORTAL,
		PIPELINE_DATA_NOCARING_NOBLEND,
		ROOTSIGNATURE_DATA_DRAW_TEX,
		PIPELINE_NAME_PORTAL
	);

	GraphicsPipeLineMgr::Instance()->CreatePipeLine(
		LAYOUT_POS,
		SHADER_VERTEX_COLOR,
		SHADER_PIXCEL_COLOR,
		PIPELINE_DATA_NOCARING_ALPHABLEND_LINE,
		ROOTSIGNATURE_DATA_DRAW,
		PIPELINE_NAME_COLOR_LINE
	);



	GraphicsPipeLineMgr::Instance()->CreatePipeLine(
		LAYOUT_POS,
		SHADER_VERTEX_COLOR,
		SHADER_PIXCEL_COLOR,
		PIPELINE_DATA_NOCARING_BLENDALPHA,
		ROOTSIGNATURE_DATA_DRAW,
		PIPELINE_NAME_COLOR_NOCARING
	);

	//インスタンシング描画色パイプラインにシャドウを入れる
	//GraphicsPipeLineMgr::Instance()->CreatePipeLine(
	//	LAYOUT_POS_NORMAL_TEX,
	//	SHADER_VERTEX_INSTANCE_COLOR_GET_SHADOWMAP,
	//	SHADER_PIXCEL_INSTANCE_COLOR_GET_SHADOW,
	//	PIPELINE_DATA_NOCARING_NOBLEND_R32,
	//	ROOTSIGNATURE_DATA_DATA1_DATA2,
	//	PIPELINE_NAME_INSTANCE_COLOR_GET_SHADOWMAP
	//);


	//インスタンシング描画色パイプライン
	GraphicsPipeLineMgr::Instance()->CreatePipeLine(
		LAYOUT_POS,
		SHADER_VERTEX_INSTANCE_COLOR,
		SHADER_PIXCEL_INSTANCE_COLOR,
		PIPELINE_DATA_BACKCARING_ALPHABLEND,
		ROOTSIGNATURE_DATA_UAV,
		PIPELINE_NAME_INSTANCE_COLOR
	);

	GraphicsPipeLineMgr::Instance()->CreatePipeLine(
		LAYOUT_POS,
		SHADER_VERTEX_INSTANCE_COLOR,
		SHADER_PIXCEL_INSTANCE_COLOR,
		PIPELINE_DATA_BACKCARING_ALPHABLEND_WIREFLAME,
		ROOTSIGNATURE_DATA_UAV,
		PIPELINE_NAME_INSTANCE_COLOR_WIREFLAME
	);


	//色パイプライン(ワイヤーフレーム)
	GraphicsPipeLineMgr::Instance()->CreatePipeLine(
		LAYOUT_POS,
		SHADER_VERTEX_COLOR,
		SHADER_PIXCEL_COLOR,
		PIPELINE_DATA_BACKCARING_ALPHABLEND_WIREFLAME,
		ROOTSIGNATURE_DATA_DRAW,
		PIPELINE_NAME_COLOR_WIREFLAME
	);

	//スプライトパイプライン
	GraphicsPipeLineMgr::Instance()->CreatePipeLine(
		LAYOUT_POS_TEX,
		SHADER_VERTEX_SPRITE,
		SHADER_PIXCEL_SPRITE,
		PIPELINE_DATA_NOCARING_BLENDALPHA,
		ROOTSIGNATURE_DATA_DRAW_TEX,
		PIPELINE_NAME_SPRITE
	);

	//スプライトパイプライン
	GraphicsPipeLineMgr::Instance()->CreatePipeLine(
		LAYOUT_POS_TEX,
		SHADER_VERTEX_SPRITE,
		SHADER_PIXCEL_SPRITE_WINDOW,
		PIPELINE_DATA_NOCARING_BLENDALPHA,
		ROOTSIGNATURE_DATA_DRAW_DATA1_TEX_TEX2,
		PIPELINE_NAME_SPRITE_WINODW
	);


	//スプライトパイプライン
	GraphicsPipeLineMgr::Instance()->CreatePipeLine(
		LAYOUT_POS_TEX,
		SHADER_VERTEX_SPRITE,
		SHADER_PIXEL_SPRITE_COLOR,
		PIPELINE_DATA_NOCARING_BLENDALPHA_CUT,
		ROOTSIGNATURE_DATA_DRAW_TEX,
		PIPELINE_NAME_SPRITE_COLOR
	);

	//スプライトパイプライン+常に深度負ける
	GraphicsPipeLineMgr::Instance()->CreatePipeLine(
		LAYOUT_POS_TEX,
		SHADER_VERTEX_SPRITE,
		SHADER_PIXCEL_SPRITE,
		PIPELINE_DATA_NOCARING_BLENDALPHA_DEPTH_LESSEQUAL,
		ROOTSIGNATURE_DATA_DRAW_TEX,
		PIPELINE_NAME_SPRITE_DEPTH_NOTEQUAL
	);


	//スプライト+砂嵐パイプライン
	GraphicsPipeLineMgr::Instance()->CreatePipeLine(
		LAYOUT_POS_TEX,
		SHADER_VERTEX_SPRITE,
		SHADER_PIXCEL_SPRITE_NOISE,
		PIPELINE_DATA_NOCARING_BLENDALPHA,
		ROOTSIGNATURE_DATA_DRAW_TEX_DATA1,
		PIPELINE_NAME_SPRITE_NOISE
	);


	//スプライトパイプライン
	GraphicsPipeLineMgr::Instance()->CreatePipeLine(
		LAYOUT_POS_TEX,
		SHADER_VERTEX_SPRITE,
		SHADER_PIXCEL_SPRITE,
		PIPELINE_DATA_NOCARING_BLENDALPHA_DEPTH_ALWAYS,
		ROOTSIGNATURE_DATA_DRAW_TEX,
		PIPELINE_NAME_SPRITE_Z_ALWAYS
	);

	//スプライトパイプライン
	GraphicsPipeLineMgr::Instance()->CreatePipeLine(
		LAYOUT_POS_TEX,
		SHADER_VERTEX_SPRITE,
		SHADER_PIXCEL_SPRITE,
		PIPELINE_DATA_NOCARING_BLENDALPHA_DEPTH_ALWAYS,
		ROOTSIGNATURE_DATA_DRAW_TEX,
		PIPELINE_NAME_SPRITE_Z_ALWAYS
	);

	GraphicsPipeLineMgr::Instance()->CreatePipeLine(
		LAYOUT_POS_TEX,
		SHADER_VERTEX_SPRITE,
		SHADER_PIXCEL_SPRITE,
		PIPELINE_DATA_NOCARING_CUTALPHA_DEPTH_ALWAYS,
		ROOTSIGNATURE_DATA_DRAW_TEX,
		PIPELINE_NAME_SPRITE_Z_ALWAYS_CUTALPHA
	);

	//スプライトパイプライン
	GraphicsPipeLineMgr::Instance()->CreatePipeLine(
		LAYOUT_POS_TEX,
		SHADER_VERTEX_SPRITE,
		SHADER_PIXCEL_SPRITE,
		PIPELINE_DATA_NOCARING_BLENDALPHA_DEPTH_OFF,
		ROOTSIGNATURE_DATA_DRAW_TEX,
		PIPELINE_NAME_SPRITE_Z_OFF
	);


	//スプライトパイプライン
	GraphicsPipeLineMgr::Instance()->CreatePipeLine(
		LAYOUT_POS_TEX,
		SHADER_VERTEX_SPRITE,
		SHADER_PIXCEL_SPRITE,
		PIPELINE_DATA_NOCARING_NOBLEND,
		ROOTSIGNATURE_DATA_DRAW_TEX,
		PIPELINE_NAME_SPRITE_NOBLEND
	);

	//ゴールエフェクト
	GraphicsPipeLineMgr::Instance()->CreatePipeLine(
		LAYOUT_POS_TEX,
		SHADER_VERTEX_SPRITE,
		SHADER_PIXCEL_SPRITE_GOAL_EFFECT,
		//PIPELINE_DATA_NOCARING_ADDBLEND,
		PIPELINE_DATA_NOCARING_BLENDALPHA,
		ROOTSIGNATURE_DATA_DRAW_TEX_DATA1,
		PIPELINE_NAME_SPRITE_GOAL_EFFECT
	);

	//ゴールエフェクト2
	GraphicsPipeLineMgr::Instance()->CreatePipeLine(
		LAYOUT_POS_TEX,
		SHADER_VERTEX_SPRITE,
		SHADER_PIXCEL_SPRITE_GOAL_EFFECT,
		PIPELINE_DATA_NOCARING_BLENDALPHA_DEPTHOFF,
		ROOTSIGNATURE_DATA_DRAW_TEX_DATA1,
		PIPELINE_NAME_SPRITE_GOAL_EFFECT_DEPTHOFF
	);

	//Objパイプライン
	GraphicsPipeLineMgr::Instance()->CreatePipeLine(
		LAYOUT_POS_NORMAL_TEX,
		SHADER_VERTEX_OBJ,
		SHADER_PIXCEL_OBJ,
		PIPELINE_DATA_NOCARING_BLENDALPHA,
		ROOTSIGNATURE_DATA_DRAW_TEX_DATA1,
		PIPELINE_NAME_OBJ
	);

	//OBJパイプラインのワイヤーフレーム
	GraphicsPipeLineMgr::Instance()->CreatePipeLine(
		LAYOUT_POS,
		SHADER_VERTEX_COLOR,
		SHADER_PIXCEL_COLOR,
		PIPELINE_DATA_BACKCARING_ALPHABLEND_WIREFLAME,
		ROOTSIGNATURE_DATA_DRAW_DATA1,
		PIPELINE_NAME_OBJ_WIREFLAME
	);


	GraphicsPipeLineMgr::Instance()->CreatePipeLine(
		LAYOUT_POS,
		SHADER_VERTEX_FOG_COLOR,
		SHADER_PIXCEL_FOG_COLOR,
		PIPELINE_DATA_NOCARING_ALPHABLEND_WIREFLAME,
		ROOTSIGNATURE_DATA_DRAW_DATA1,
		PIPELINE_NAME_OBJ_WIREFLAME_FOG
	);

	GraphicsPipeLineMgr::Instance()->CreatePipeLine(
		LAYOUT_POS_NORMAL_TEX,
		SHADER_VERTEX_FOG_OBJ,
		SHADER_PIXCEL_OBJ_GRADATION,
		PIPELINE_DATA_NOCARING_BLENDALPHA,
		ROOTSIGNATURE_DATA_DRAW_TEX_DATA1_DATA2,
		PIPELINE_NAME_OBJ_FOG_GRADATION
	);


	//インスタンシング描画Objパイプライン
	GraphicsPipeLineMgr::Instance()->CreatePipeLine(
		LAYOUT_POS_NORMAL_TEX,
		SHADER_VERTEX_INSTANCE_OBJ,
		SHADER_PIXCEL_INSTANCE_OBJ,
		PIPELINE_DATA_NOCARING_BLENDALPHA,
		ROOTSIGNATURE_DATA_DATA1_DATA2_TEX,
		PIPELINE_NAME_INSTANCE_OBJ
	);

	//頂点拡大のパイプライン
	GraphicsPipeLineMgr::Instance()->CreatePipeLine(
		LAYOUT_POS_NORMAL_TEX,
		SHADER_VERTEX_EXPANTION_OBJ,
		SHADER_PIXCEL_EXPANTION_OBJ,
		PIPELINE_DATA_EXPANTION_VERTEX,
		ROOTSIGNATURE_DATA_DRAW_TEX_DATA1_DATA2,
		PIPELINE_NAME_OBJ_EXPANSION_VERTEX
	);

	////インスタンシング描画頂点拡大のパイプライン
	//GraphicsPipeLineMgr::Instance()->CreatePipeLine(
	//	LAYOUT_POS_NORMAL_TEX,
	//	SHADER_VERTEX_INSTANCE_OBJ,
	//	SHADER_PIXCEL_INSTANCE_OBJ_EXPANTION,
	//	PIPELINE_DATA_EXPANTION_VERTEX,
	//	ROOTSIGNATURE_DATA_DATA1_DATA2_DATA3_TEX,
	//	PIPELINE_NAME_INSTANCE_OBJ_EXPANTION_VERTEX
	//);

	//シャドウマップ取得用パイプライン
	GraphicsPipeLineMgr::Instance()->CreatePipeLine(
		LAYOUT_POS_NORMAL_TEX,
		SHADER_VERTEX_GET_SHADOWMAP,
		SHADER_PIXCEL_GET_SHADOWMAP,
		PIPELINE_DATA_NOCARING_NOBLEND_R32,
		ROOTSIGNATURE_DATA_DRAW_TEX_DATA1_DATA2,
		PIPELINE_NAME_OBJ_GET_SHADOWMAP
	);

	//インスタンシング描画シャドウマップ取得用パイプライン
	//GraphicsPipeLineMgr::Instance()->CreatePipeLine(
	//	LAYOUT_POS_NORMAL_TEX,
	//	SHADER_VERTEX_INSTANCE_GET_SHADOWMAP,
	//	SHADER_PIXCEL_INSTANCE_GET_SHADOWMAP,
	//	PIPELINE_DATA_NOCARING_NOBLEND_R32,
	//	ROOTSIGNATURE_DATA_DATA1_DATA2_DATA3_TEX,
	//	PIPELINE_NAME_INSTANCE_OBJ_GET_SHADOWMAP
	//);

	////シャドウマップを元に影描画用パイプライン
	//GraphicsPipeLineMgr::Instance()->CreatePipeLine(
	//	LAYOUT_POS_NORMAL_TEX,
	//	SHADER_VERTEX_DRAW_SHADOWMAP,
	//	SHADER_PIXCEL_DRAW_SHADOWMAP,
	//	PIPELINE_DATA_NOCARING_NOBLEND,
	//	ROOTSIGNATURE_DATA_DRAW_TEX_TEX2_DATA1_DATA2,
	//	PIPELINE_NAME_OBJ_DRAW_SHADOWMAP
	//);

	//LightObjパイプライン
	GraphicsPipeLineMgr::Instance()->CreatePipeLine(
		LAYOUT_POS_NORMAL_TEX,
		SHADER_VERTEX_OBJ_LIGHT,
		SHADER_PIXCEL_OBJ_LIGHT,
		PIPELINE_DATA_NOCARING_NOBLEND,
		ROOTSIGNATURE_DATA_DRAW_TEX_DATA1_DATA2,
		PIPELINE_NAME_OBJ_LIGHT
	);


	//Lineパイプライン
	GraphicsPipeLineMgr::Instance()->CreatePipeLine(
		LAYOUT_POS,
		SHADER_VERTEX_COLOR,
		SHADER_PIXCEL_COLOR,
		PIPELINE_DATA_NOCARING_ALPHABLEND_LINE,
		ROOTSIGNATURE_DATA_DRAW,
		PIPELINE_NAME_LINE
	);

	//FogLineパイプライン
	GraphicsPipeLineMgr::Instance()->CreatePipeLine(
		LAYOUT_POS,
		SHADER_VERTEX_FOG_COLOR,
		SHADER_PIXCEL_FOG_COLOR,
		PIPELINE_DATA_NOCARING_ALPHABLEND_LINE,
		ROOTSIGNATURE_DATA_DRAW_DATA1,
		PIPELINE_NAME_FOG_LINE
	);


	//FogLineパイプライン
	GraphicsPipeLineMgr::Instance()->CreatePipeLine(
		LAYOUT_POS,
		SHADER_VERTEX_FOG_COLOR,
		SHADER_PIXCEL_FOG_MULTITEX,
		PIPELINE_DATA_NOCARING_ALPHABLEND_LINE_MULTITEX,
		ROOTSIGNATURE_DATA_DRAW_DATA1_DATA2,
		PIPELINE_NAME_FOG_LINE_MULTIPASS
	);


	//インスタンシング描画色パイプライン
	GraphicsPipeLineMgr::Instance()->CreatePipeLine(
		LAYOUT_POS,
		SHADER_VERTEX_INSTANCE_COLOR,
		SHADER_PIXCEL_INSTANCE_COLOR,
		PIPELINE_DATA_NOCARING_ALPHABLEND_LINE,
		ROOTSIGNATURE_DATA_UAV,
		PIPELINE_NAME_INSTANCE_COLOR_LINE
	);


	//Lineパイプライン(uv付き)
	GraphicsPipeLineMgr::Instance()->CreatePipeLine(
		LAYOUT_POS_TEX,
		SHADER_VERTEX_SPRITE,
		SHADER_PIXCEL_LINE_UV,
		PIPELINE_DATA_NOCARING_ALPHABLEND_LINE,
		ROOTSIGNATURE_DATA_DRAW_DATA1,
		PIPELINE_NAME_LINE_UV
	);

	//LineFlash
	GraphicsPipeLineMgr::Instance()->CreatePipeLine(
		LAYOUT_POS_TEX,
		SHADER_VERTEX_SPRITE,
		SHADER_PIXCEL_LINE_FLASHEFFECT,
		PIPELINE_DATA_NOCARING_ALPHABLEND_LINE_MULTITEX,
		ROOTSIGNATURE_DATA_DRAW_DATA1,
		PIPELINE_NAME_LINE_FLASHEFFECT
	);

	//ライト
	GraphicsPipeLineMgr::Instance()->CreatePipeLine(
		LAYOUT_POS_NORMAL_TEX,
		SHADER_VERTEX_LIGHT,
		SHADER_PIXCEL_LIGHT,
		PIPELINE_DATA_NOCARING_BLENDALPHA,
		ROOTSIGNATURE_MODE_LIGHT,
		PIPELINE_NAME_LIGHT
	);

	//テクスチャ
	GraphicsPipeLineMgr::Instance()->CreatePipeLine(
		LAYOUT_POS_NORMAL_TEX,
		SHADER_VERTEX_TEXTURE,
		SHADER_PIXCEL_TEXTURE,
		PIPELINE_DATA_BACKCARING_ALPHABLEND,
		ROOTSIGNATURE_DATA_DRAW_TEX,
		PIPELINE_NAME_TEXTURE
	);

	//輝度抽出
	GraphicsPipeLineMgr::Instance()->CreatePipeLine(
		LAYOUT_POS_TEX,
		SHADER_VERTEX_LUMINAS,
		SHADER_PIXCEL_LUMINAS,
		PIPELINE_DATA_BACKCARING_ALPHABLEND,
		ROOTSIGNATURE_MODE_LIGHT_BORDER,
		PIPELINE_NAME_LUMINANCE
	);

	//ガウシアン上ぼかし用のデータ
	GraphicsPipeLineMgr::Instance()->CreatePipeLine(
		LAYOUT_POS_TEX,
		SHADER_VERTEX_SPRITE,
		SHADER_PIXCEL_GAUSSIAN_UPBLUR,
		PIPELINE_DATA_NOCARING_NOBLEND,
		ROOTSIGNATURE_MODE_LIGHT_BORDER,
		PIPELINE_NAME_GAUSSIAN_UPBLUR
	);

	//ガウシアン上ぼかし用のデータ
	GraphicsPipeLineMgr::Instance()->CreatePipeLine(
		LAYOUT_POS_TEX,
		SHADER_VERTEX_SPRITE,
		SHADER_PIXCEL_GAUSSIAN_SIDEBLUR,
		PIPELINE_DATA_NOCARING_NOBLEND,
		ROOTSIGNATURE_MODE_LIGHT_BORDER,
		PIPELINE_NAME_GAUSSIAN_SIDEBLUR
	);

	//Fbx描画用パイプライン
	GraphicsPipeLineMgr::Instance()->CreatePipeLine(
		LAYOUT_POS_NORMAL_TEX_BONE_WEIGHT,
		SHADER_VERTEX_FBX,
		SHADER_PIXCEL_FBX,
		PIPELINE_DATA_BACKCARING_ALPHABLEND,
		ROOTSIGNATURE_DATA_DRAW_TEX_SKINING,
		PIPELINE_NAME_FBX
	);

	//Fbx描画用パイプライン
	GraphicsPipeLineMgr::Instance()->CreatePipeLine(
		LAYOUT_POS_NORMAL_TEX_BONE_WEIGHT,
		SHADER_VERTEX_FBX,
		SHADER_PIXCEL_FBX_RENDER_TWO,
		PIPELINE_DATA_BACKCARING_ALPHABLEND_RNEDERTARGET_SECOND,
		ROOTSIGNATURE_DATA_DRAW_TEX_SKINING,
		PIPELINE_NAME_FBX_RENDERTARGET_TWO
	);

	GraphicsPipeLineMgr::Instance()->CreatePipeLine(
		LAYOUT_POS_NORMAL_TEX_BONE_WEIGHT,
		SHADER_VERTEX_FBX,
		SHADER_PIXCEL_FBX_RENDER_TWO_LIGHT,
		PIPELINE_DATA_BACKCARING_ALPHABLEND_RNEDERTARGET_SECOND,
		ROOTSIGNATURE_DATA_DRAW_TEX_SKINING_DATA3,
		PIPELINE_NAME_FBX_RENDERTARGET_TWO_LIGHT
	);

	//加算合成
	GraphicsPipeLineMgr::Instance()->CreatePipeLine(
		LAYOUT_POS_TEX,
		SHADER_VERTEX_SPRITE,
		SHADER_PIXCEL_SPRITE,
		PIPELINE_DATA_NOCARING_ADDBLEND_CUTALPHA,
		ROOTSIGNATURE_DATA_DRAW_TEX,
		PIPELINE_NAME_ADDBLEND
	);

	//川瀬式ブルーム
	GraphicsPipeLineMgr::Instance()->CreatePipeLine(
		LAYOUT_POS_TEX,
		SHADER_VERTEX_SPRITE,
		SHADER_PIXCEL_BLOOM,
		PIPELINE_DATA_NOCARING_ADDBLEND_CUTALPHA,
		ROOTSIGNATURE_DATA_DRAW_TEX4,
		PIPELINE_NAME_BLOOM
	);

	//正規化デバイス上の加算合成画像描画
	GraphicsPipeLineMgr::Instance()->CreatePipeLine(
		LAYOUT_POS_TEX,
		SHADER_VERTEX_MULTIPASS,
		SHADER_PIXCEL_SPRITE,
		PIPELINE_DATA_NOCARING_ADDBLEND_CUTALPHA,
		ROOTSIGNATURE_DATA_DRAW_TEX,
		PIPELINE_NAME_MULTIPASS
	);

	//ゴールライト用
	GraphicsPipeLineMgr::Instance()->CreatePipeLine(
		LAYOUT_POS_TEX,
		SHADER_VERTEX_SPRITE,
		SHADER_PIXCEL_GOALLIGHT,
		PIPELINE_DATA_NOCARING_BLENDALPHA,
		ROOTSIGNATURE_MODE_LIGHT,
		PIPELINE_NAME_GOALLIGHT
	);

	//Fog
	GraphicsPipeLineMgr::Instance()->CreatePipeLine(
		LAYOUT_POS,
		SHADER_VERTEX_FOG_COLOR,
		SHADER_PIXCEL_FOG_COLOR,
		PIPELINE_DATA_NOCARING_BLENDALPHA,
		ROOTSIGNATURE_DATA_DRAW_DATA1,
		PIPELINE_NAME_FOG_COLOR
	);


	//FogFbx描画用パイプライン
	GraphicsPipeLineMgr::Instance()->CreatePipeLine(
		LAYOUT_POS_NORMAL_TEX_BONE_WEIGHT,
		SHADER_VERTEX_FBX,
		SHADER_PIXCEL_FOG_FBX,
		PIPELINE_DATA_BACKCARING_ALPHABLEND,
		ROOTSIGNATURE_DATA_DRAW_TEX_SKINING_DATA1,
		PIPELINE_NAME_FOG_FBX
	);


	GraphicsPipeLineMgr::Instance()->CreatePipeLine(
		LAYOUT_POS_NORMAL_TEX_BONE_WEIGHT,
		SHADER_VERTEX_FBX_FOG,
		SHADER_PIXCEL_FOG_COLOR,
		PIPELINE_DATA_NOCARING_NOBLEND_WIREFLAME,
		ROOTSIGNATURE_DATA_DRAW_SKINING_DATA1,
		PIPELINE_NAME_FOG_FBX_WIREFLAME
	);

	GraphicsPipeLineMgr::Instance()->CreatePipeLine(
		LAYOUT_POS_NORMAL_TEX_BONE_WEIGHT,
		SHADER_VERTEX_FBX_FOG,
		SHADER_PIXCEL_FOG_MULTITEX,
		PIPELINE_DATA_NOCARING_NOBLEND_WIREFLAME_MULTIPASS_TWO,
		ROOTSIGNATURE_DATA_DRAW_SKINING_DATA1,
		PIPELINE_NAME_FOG_FBX_WIREFLAME_MULTI_TWO
	);
	

	//Objパイプライン
	GraphicsPipeLineMgr::Instance()->CreatePipeLine(
		LAYOUT_POS_NORMAL_TEX,
		SHADER_VERTEX_OBJ,
		SHADER_PIXCEL_FOG_OBJ,
		PIPELINE_DATA_NOCARING_NOBLEND,
		ROOTSIGNATURE_DATA_DRAW_DATA1_TEX_DATA2,
		PIPELINE_NAME_FOG_OBJ
	);

	//線の太さ変える
	//GraphicsPipeLineMgr::Instance()->CreatePipeLine(
	//	LAYOUT_POS_TICK,
	//	SHADER_VERTEX_LINE,
	//	SHADER_PIXCEL_LINE,
	//	PIPELINE_DATA_NOCARING_ALPHABLEND_LINE,
	//	ROOTSIGNATURE_MODE_LINE,
	//	PIPELINE_NAME_LINE_TICK,
	//	SHADER_GEOMETORY_LINE
	//);

	//スプライト...フォグ付き
	GraphicsPipeLineMgr::Instance()->CreatePipeLine(
		LAYOUT_POS_TEX,
		SHADER_VERTEX_SPRITE,
		SHADER_PIXCEL_FOG_SPRITE,
		PIPELINE_DATA_NOCARING_BLENDALPHA,
		ROOTSIGNATURE_MODE_LIGHT,
		PIPELINE_NAME_FOG_SPRITE
	);

	//線の太さ変える...フォグ付き
	//GraphicsPipeLineMgr::Instance()->CreatePipeLine(
	//	LAYOUT_POS_TICK,
	//	SHADER_VERTEX_LINE,
	//	SHADER_PIXCEL_TICK_LINE_FOG,
	//	PIPELINE_DATA_NOCARING_ALPHABLEND_LINE,
	//	ROOTSIGNATURE_MODE_LINE,
	//	PIPELINE_NAME_FOG_LINE_TICK,
	//	SHADER_GEOMETORY_LINE
	//);

	//スプライトパイプライン
	GraphicsPipeLineMgr::Instance()->CreatePipeLine(
		LAYOUT_POS_TEX,
		SHADER_VERTEX_SPRITE,
		SHADER_PIXCEL_SPRITE,
		PIPELINE_DATA_NOCARING_BLENDALPHA_CUT,
		ROOTSIGNATURE_DATA_DRAW_TEX,
		PIPELINE_NAME_SPRITE_CUTALPHA
	);



	//FogLineForBgパイプライン
	GraphicsPipeLineMgr::Instance()->CreatePipeLine(
		LAYOUT_POS,
		SHADER_VERTEX_COLOR,
		SHADER_PIXCEL_LINE_FOG_FORBG,
		PIPELINE_DATA_NOCARING_ALPHABLEND_LINE,
		ROOTSIGNATURE_MODE_LINE,
		PIPELINE_NAME_FOG_LINE_FORBG
	);


	GraphicsPipeLineMgr::Instance()->CreatePipeLine(
		LAYOUT_POS_TEX,
		SHADER_VERTEX_SPRITE,
		SHADER_PIXCEL_WIHITENOISE,
		PIPELINE_DATA_NOCARING_BLENDALPHA,
		ROOTSIGNATURE_MODE_LIGHT_BORDER,
		PIPELINE_NAME_SPRITE_VHS
	);


	//板ポリの色付き
	GraphicsPipeLineMgr::Instance()->CreatePipeLine(
		LAYOUT_POS_TEX,
		SHADER_VERTEX_SPRITE,
		SHADER_PIXCEL_SPRITE_GRADATION,
		PIPELINE_DATA_NOCARING_BLENDALPHA,
		ROOTSIGNATURE_DATA_DRAW_DATA1,
		PIPELINE_NAME_SPRITE_GRADATION
	);



	//マルチパスは「テクスチャ用」、
	//Create2DPipeLineはスプライト
	//CreateGetLightPipelineはスプライト用
	//basicはスプライト用
	//ブレンド用のは一杯用意する必要がある


#pragma endregion


	//マルチテクスチャ用
	//色パイプライン
	GraphicsPipeLineMgr::Instance()->CreatePipeLine(
		LAYOUT_POS,
		SHADER_VERTEX_COLOR,
		SHADER_PIXCEL_COLOR_MULTITEX,
		PIPELINE_DATA_NOCARING_ALPHABLEND_RNEDERTARGET_SECOND,
		ROOTSIGNATURE_DATA_DRAW_DATA1,
		PIPELINE_NAME_COLOR_MULTITEX
	);

	GraphicsPipeLineMgr::Instance()->CreatePipeLine(
		LAYOUT_POS,
		SHADER_VERTEX_INSTANCE_COLOR,
		SHADER_PIXCEL_INSTANCE_COLOR_MULTITEX,
		PIPELINE_DATA_NOCARING_ALPHABLEND_RNEDERTARGET_SECOND,
		ROOTSIGNATURE_DATA_UAV_CB,
		PIPELINE_NAME_INSTANCE_COLOR_MULTITEX
	);


	GraphicsPipeLineMgr::Instance()->CreatePipeLine(
		LAYOUT_POS_NORMAL_TEX,
		SHADER_VERTEX_INSTANCE_COLOR_SHADING,
		SHADER_PIXCEL_INSTANCE_COLOR_SHADING,
		PIPELINE_DATA_NOCARING_ALPHABLEND_RNEDERTARGET_SECOND,
		ROOTSIGNATURE_DATA_UAV_CB,
		PIPELINE_NAME_INSTANCE_COLOR_MULTITEX_SHADING
	);


	//フォグ
	GraphicsPipeLineMgr::Instance()->CreatePipeLine(
		LAYOUT_POS,
		SHADER_VERTEX_FOG_COLOR,
		SHADER_PIXCEL_FOG_COLOR_MULTITEX,
		PIPELINE_DATA_NOCARING_BLENDALPHA_MULTIPASS_TWO,
		ROOTSIGNATURE_DATA_DRAW_DATA1,
		PIPELINE_NAME_FOG_COLOR_MULTITEX
	);

	//スプライトパイプライン
	GraphicsPipeLineMgr::Instance()->CreatePipeLine(
		LAYOUT_POS_TEX,
		SHADER_VERTEX_SPRITE,
		SHADER_PIXCEL_SPRITE_MULTITEX,
		PIPELINE_DATA_NOCARING_BLENDALPHA_MULTIPASS_TWO,
		ROOTSIGNATURE_DATA_DRAW_TEX,
		PIPELINE_NAME_SPRITE_MULTITEX
	);


	//Objパイプライン
	GraphicsPipeLineMgr::Instance()->CreatePipeLine(
		LAYOUT_POS_NORMAL_TEX,
		SHADER_VERTEX_OBJ,
		SHADER_PIXCEL_OBJ_MULTITEX,
		PIPELINE_DATA_BACKCARING_ALPHABLEND_RNEDERTARGET_SECOND,
		ROOTSIGNATURE_DATA_DRAW_TEX_DATA1,
		PIPELINE_NAME_OBJ_MULTITEX
	);

	//ゴールエフェクト
	GraphicsPipeLineMgr::Instance()->CreatePipeLine(
		LAYOUT_POS_TEX,
		SHADER_VERTEX_SPRITE,
		SHADER_PIXCEL_GOALLIGHT_MULTITEX,
		PIPELINE_DATA_NOCARING_BLENDALPHA_MULTIPASS_TWO,
		ROOTSIGNATURE_DATA_DRAW_TEX_DATA1,
		PIPELINE_NAME_SPRITE_GOAL_EFFECT_MULTITEX
	);

	//輝度抽出
	//スプライトパイプライン
	GraphicsPipeLineMgr::Instance()->CreatePipeLine(
		LAYOUT_POS_TEX,
		SHADER_VERTEX_SPRITE,
		SHADER_PIXCEL_LUMINANCE_MULTI,
		PIPELINE_DATA_NOCARING_BLENDALPHA,
		ROOTSIGNATURE_DATA_DRAW_TEX_TEX2,
		PIPELINE_NAME_SPRITE_LUMI
	);

	GraphicsPipeLineMgr::Instance()->CreatePipeLine(
		LAYOUT_POS_TEX,
		SHADER_VERTEX_SPRITE,
		SHADER_PIXCEL_SPRITE,
		PIPELINE_DATA_NOCARING_BLENDALPHA_CUT_MULTITEX,
		ROOTSIGNATURE_DATA_DRAW_TEX,
		PIPELINE_NAME_SPRITE_CUTALPHA_MULTITEX
	);


	//色パイプライン(ワイヤーフレーム) MULTI
	GraphicsPipeLineMgr::Instance()->CreatePipeLine(
		LAYOUT_POS,
		SHADER_VERTEX_COLOR,
		SHADER_PIXCEL_COLOR_MULTITEX,
		PIPELINE_DATA_BACKCARING_ALPHABLEND_WIREFLAME_MUTITEX,
		ROOTSIGNATURE_DATA_DRAW_DATA1,
		PIPELINE_NAME_COLOR_WIREFLAME_MULTITEX
	);


	//Lineパイプライン
	GraphicsPipeLineMgr::Instance()->CreatePipeLine(
		LAYOUT_POS,
		SHADER_VERTEX_COLOR,
		SHADER_PIXCEL_COLOR_MULTITEX,
		PIPELINE_DATA_NOCARING_ALPHABLEND_LINE_MULTITEX,
		ROOTSIGNATURE_DATA_DRAW_DATA1,
		PIPELINE_NAME_LINE_MULTITEX
	);

	//Lineパイプライン(uv付き)
	GraphicsPipeLineMgr::Instance()->CreatePipeLine(
		LAYOUT_POS_TEX,
		SHADER_VERTEX_SPRITE,
		SHADER_PIXCEL_LINE_UV_MULTITEX,
		PIPELINE_DATA_NOCARING_ALPHABLEND_LINE_MULTITEX,
		ROOTSIGNATURE_DATA_DRAW_DATA1,
		PIPELINE_NAME_LINE_UV_MULTITEX
	);



#pragma region パイプラインの設定の登録HDR用
	DXGI_FORMAT_R32G32B32A32_FLOAT;

	//加算スプライト
#pragma region PIPELINE_DATA_NOCARING_ADDBLEND_HDR
	{
		D3D12_GRAPHICS_PIPELINE_STATE_DESC gPipeline{};
		//サンプルマスク
		gPipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;

		//ラスタライザ
		//塗りつぶし、深度クリッピング有効
		CD3DX12_RASTERIZER_DESC rasterrize(D3D12_DEFAULT);
		gPipeline.RasterizerState = rasterrize;
		gPipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;

		//ブレンドモード
		gPipeline.BlendState.RenderTarget[0] = alphaBlendDesc;
		gPipeline.BlendState.AlphaToCoverageEnable = true;


		//図形の形状
		gPipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

		//その他設定
		gPipeline.NumRenderTargets = 1;
		gPipeline.RTVFormats[0] = DXGI_FORMAT_R32G32B32A32_FLOAT;
		gPipeline.SampleDesc.Count = 1;

		//デプスステンシルステートの設定
		gPipeline.DepthStencilState.DepthEnable = true;							//深度テストを行う
		gPipeline.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;//書き込み許可
		gPipeline.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;		//小さければOK
		gPipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;							//深度値フォーマット
		GraphicsPipeLineMgr::Instance()->RegisterPipeLineDataWithData(gPipeline, PIPELINE_DATA_NOCARING_ADDBLEND_HDR);
	}
#pragma endregion


	//OBJ
#pragma region PIPELINE_DATA_NOCARING_NOBLEND_HDR
//Obj用のパイプラインの設定
	{
		D3D12_GRAPHICS_PIPELINE_STATE_DESC gPipeline{};
		D3D12_RENDER_TARGET_BLEND_DESC blendDesc{};
		//サンプルマスク
		gPipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;

		//ラスタライザ
		//背面カリング、塗りつぶし、深度クリッピング有効
		CD3DX12_RASTERIZER_DESC rasterrize(D3D12_DEFAULT);
		gPipeline.RasterizerState = rasterrize;
		gPipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;

		//ブレンドモード
		blendDesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
		gPipeline.BlendState.RenderTarget[0] = blendDesc;

		//図形の形状
		gPipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

		//その他設定
		gPipeline.NumRenderTargets = 1;
		gPipeline.RTVFormats[0] = DXGI_FORMAT_R32G32B32A32_FLOAT;
		gPipeline.SampleDesc.Count = 1;

		//デプスステンシルステートの設定
		gPipeline.DepthStencilState.DepthEnable = true;							//深度テストを行う
		gPipeline.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;//書き込み許可
		gPipeline.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;		//小さければOK
		gPipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;							//深度値フォーマット
		GraphicsPipeLineMgr::Instance()->RegisterPipeLineDataWithData(gPipeline, PIPELINE_DATA_NOCARING_NOBLEND_HDR);
	}

#pragma endregion


	//LINE
#pragma region PIPELINE_DATA_NOCARING_ALPHABLEND_LINE_HDR
	{
		D3D12_GRAPHICS_PIPELINE_STATE_DESC gPipeline{};
		D3D12_RENDER_TARGET_BLEND_DESC blendDesc{};
		//サンプルマスク
		gPipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;

		//ラスタライザ
		//背面カリング、塗りつぶし、深度クリッピング有効
		CD3DX12_RASTERIZER_DESC rasterrize(D3D12_DEFAULT);
		gPipeline.RasterizerState = rasterrize;
		gPipeline.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
		gPipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;

		//ブレンドモード
		blendDesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
		gPipeline.BlendState.RenderTarget[0] = alphaBlendDesc;

		//図形の形状
		gPipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE;

		//その他設定
		gPipeline.NumRenderTargets = 1;
		gPipeline.RTVFormats[0] = DXGI_FORMAT_R32G32B32A32_FLOAT;
		gPipeline.SampleDesc.Count = 1;

		//デプスステンシルステートの設定
		gPipeline.DepthStencilState.DepthEnable = true;							//深度テストを行う
		gPipeline.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;//書き込み許可
		gPipeline.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;		//小さければOK
		gPipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;							//深度値フォーマット
		GraphicsPipeLineMgr::Instance()->RegisterPipeLineDataWithData(gPipeline, PIPELINE_DATA_NOCARING_ALPHABLEND_LINE_HDR);
	}
#pragma endregion


	//POINT
#pragma region PIPELINE_DATA_BACKCARING_ALPHABLEND_POINT_HDR
	{
		D3D12_GRAPHICS_PIPELINE_STATE_DESC gPipeline{};
		//スプライト用
		//サンプルマスク
		gPipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;

		//ラスタライザ
		//背面カリング、塗りつぶし、深度クリッピング有効
		CD3DX12_RASTERIZER_DESC rasterrize(D3D12_DEFAULT);
		gPipeline.RasterizerState = rasterrize;
		gPipeline.RasterizerState.FillMode = D3D12_FILL_MODE_WIREFRAME;
		//gPipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;

		gPipeline.BlendState.RenderTarget[0] = alphaBlendDesc;
		gPipeline.BlendState.AlphaToCoverageEnable = false;

		//図形の形状
		gPipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT;


		//その他設定
		gPipeline.NumRenderTargets = 1;
		gPipeline.RTVFormats[0] = DXGI_FORMAT_R32G32B32A32_FLOAT;
		gPipeline.SampleDesc.Count = 1;


		//デプスステンシルステートの設定
		gPipeline.DepthStencilState.DepthEnable = true;							//深度テストを行う
		gPipeline.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;//書き込み許可
		gPipeline.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
		gPipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;							//深度値フォーマット
		GraphicsPipeLineMgr::Instance()->RegisterPipeLineDataWithData(gPipeline, PIPELINE_DATA_BACKCARING_ALPHABLEND_POINT_HDR);
	}
#pragma endregion


	//aスプライト
#pragma region PIPELINE_DATA_NOCARING_BLENDALPHA_HDR
	{
		D3D12_GRAPHICS_PIPELINE_STATE_DESC gPipeline{};
		//スプライト用
		//サンプルマスク
		gPipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;

		//ラスタライザ
		//背面カリング、塗りつぶし、深度クリッピング有効
		CD3DX12_RASTERIZER_DESC rasterrize(D3D12_DEFAULT);
		gPipeline.RasterizerState = rasterrize;
		gPipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;

		gPipeline.BlendState.RenderTarget[0] = alphaBlendDesc;
		gPipeline.BlendState.AlphaToCoverageEnable = true;

		//図形の形状
		gPipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;


		//その他設定
		gPipeline.NumRenderTargets = 1;
		gPipeline.RTVFormats[0] = DXGI_FORMAT_R32G32B32A32_FLOAT;
		gPipeline.SampleDesc.Count = 1;


		//デプスステンシルステートの設定
		gPipeline.DepthStencilState.DepthEnable = true;							//深度テストを行う
		gPipeline.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;//書き込み許可
		gPipeline.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
		gPipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;							//深度値フォーマット
		GraphicsPipeLineMgr::Instance()->RegisterPipeLineDataWithData(gPipeline, PIPELINE_DATA_NOCARING_BLENDALPHA_HDR);
	}
#pragma endregion


#pragma region PIPELINE_DATA_BACKCARING_ALPHABLEND_HDR
	{
		//パイプラインの設定
		D3D12_GRAPHICS_PIPELINE_STATE_DESC gPipeline{};
		//サンプルマスク
		gPipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
		//ラスタライザ
		//背面カリング、塗りつぶし、深度クリッピング有効
		CD3DX12_RASTERIZER_DESC rasterrize(D3D12_DEFAULT);
		gPipeline.RasterizerState = rasterrize;
		//ブレンドモード
		gPipeline.BlendState.RenderTarget[0] = alphaBlendDesc;

		//図形の形状
		gPipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

		//その他設定
		gPipeline.NumRenderTargets = 1;
		gPipeline.RTVFormats[0] = DXGI_FORMAT_R32G32B32A32_FLOAT;
		gPipeline.SampleDesc.Count = 1;

		//デプスステンシルステートの設定
		gPipeline.DepthStencilState.DepthEnable = true;							//深度テストを行う
		gPipeline.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;//書き込み許可
		gPipeline.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;		//小さければOK
		gPipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;							//深度値フォーマット
		GraphicsPipeLineMgr::Instance()->RegisterPipeLineDataWithData(gPipeline, PIPELINE_DATA_BACKCARING_ALPHABLEND_HDR);
	}
#pragma endregion


	//テクスチャ用
#pragma region PIPELINE_DATA_BACKCARING_NOBLEND
	{
		//パイプラインの設定
		D3D12_GRAPHICS_PIPELINE_STATE_DESC gPipeline{};
		//サンプルマスク
		gPipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
		//ラスタライザ
		//背面カリング、塗りつぶし、深度クリッピング有効
		CD3DX12_RASTERIZER_DESC rasterrize(D3D12_DEFAULT);
		rasterrize.FillMode = D3D12_FILL_MODE_WIREFRAME;
		gPipeline.RasterizerState = rasterrize;
		//ブレンドモード
		gPipeline.BlendState.RenderTarget[0] = addBlendDesc;

		//図形の形状
		gPipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

		//その他設定
		gPipeline.NumRenderTargets = 1;
		gPipeline.RTVFormats[0] = DXGI_FORMAT_R32G32B32A32_FLOAT;
		gPipeline.SampleDesc.Count = 1;

		//デプスステンシルステートの設定
		gPipeline.DepthStencilState.DepthEnable = true;							//深度テストを行う
		gPipeline.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;//書き込み許可
		gPipeline.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;		//小さければOK
		gPipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;							//深度値フォーマット
		GraphicsPipeLineMgr::Instance()->RegisterPipeLineDataWithData(gPipeline, PIPELINE_DATA_BACKCARING_ALPHABLEND_WIREFLAME_HDR);
	}
#pragma endregion
#pragma endregion



#pragma region パイプラインの生成と登録HDR用

	//OBJ
	GraphicsPipeLineMgr::Instance()->CreatePipeLine(
		LAYOUT_POS_NORMAL_TEX,
		SHADER_VERTEX_OBJ,
		SHADER_PIXCEL_OBJ,
		PIPELINE_DATA_NOCARING_NOBLEND_HDR,
		ROOTSIGNATURE_DATA_DRAW_TEX_DATA1,
		PIPELINE_NAME_OBJ_HDR
	);

	//LINE
	GraphicsPipeLineMgr::Instance()->CreatePipeLine(
		LAYOUT_POS,
		SHADER_VERTEX_COLOR,
		SHADER_PIXCEL_COLOR,
		PIPELINE_DATA_NOCARING_ALPHABLEND_LINE_HDR,
		ROOTSIGNATURE_DATA_DRAW,
		PIPELINE_NAME_LINE_HDR
	);

	//AddSPRITE
	GraphicsPipeLineMgr::Instance()->CreatePipeLine(
		LAYOUT_POS_TEX,
		SHADER_VERTEX_SPRITE,
		SHADER_PIXCEL_SPRITE,
		PIPELINE_DATA_NOCARING_ADDBLEND_HDR,
		ROOTSIGNATURE_DATA_DRAW_TEX,
		PIPELINE_NAME_SPRITE_ADDBLEND_HDR
	);

	//αSPRITE
	GraphicsPipeLineMgr::Instance()->CreatePipeLine(
		LAYOUT_POS_TEX,
		SHADER_VERTEX_SPRITE,
		SHADER_PIXCEL_SPRITE,
		PIPELINE_DATA_NOCARING_BLENDALPHA_HDR,
		ROOTSIGNATURE_DATA_DRAW_TEX,
		PIPELINE_NAME_SPRITE_ALPHABLEND_HDR
	);

	//高輝度抽象用パイプライン
	GraphicsPipeLineMgr::Instance()->CreatePipeLine(
		LAYOUT_POS_TEX,
		SHADER_VERTEX_SPRITE,
		SHADER_PIXEL_LUMINANCE,
		PIPELINE_DATA_NOCARING_BLENDALPHA_HDR,
		ROOTSIGNATURE_DATA_DRAW_TEX,
		PIPELINE_NAME_LUMINANCE_HDR
	);

	//FogLineパイプライン
	GraphicsPipeLineMgr::Instance()->CreatePipeLine(
		LAYOUT_POS,
		SHADER_VERTEX_COLOR,
		SHADER_PIXCEL_LINE_FOG,
		PIPELINE_DATA_NOCARING_ALPHABLEND_LINE_HDR,
		ROOTSIGNATURE_MODE_LINE,
		PIPELINE_NAME_FOG_LINE_HDR
	);

	//色
	GraphicsPipeLineMgr::Instance()->CreatePipeLine(
		LAYOUT_POS,
		SHADER_VERTEX_COLOR,
		SHADER_PIXCEL_COLOR,
		PIPELINE_DATA_BACKCARING_ALPHABLEND_HDR,
		ROOTSIGNATURE_DATA_DRAW,
		PIPELINE_NAME_COLOR_HDR
	);

	//Sprite
	GraphicsPipeLineMgr::Instance()->CreatePipeLine(
		LAYOUT_POS_TEX,
		SHADER_VERTEX_SPRITE,
		SHADER_PIXCEL_SPRITE,
		PIPELINE_DATA_BACKCARING_ALPHABLEND_HDR,
		ROOTSIGNATURE_DATA_DRAW_TEX,
		PIPELINE_NAME_SPRITE_HDR
	);

	//線の太さ変える
	//GraphicsPipeLineMgr::Instance()->CreatePipeLine(
	//	LAYOUT_POS_TICK,
	//	SHADER_VERTEX_LINE,
	//	SHADER_PIXCEL_LINE,
	//	PIPELINE_DATA_NOCARING_ALPHABLEND_LINE_HDR,
	//	ROOTSIGNATURE_MODE_LINE,
	//	PIPELINE_NAME_LINE_TICK_HDR,
	//	SHADER_GEOMETORY_LINE
	//);

	//色パイプライン(ワイヤーフレーム)
	GraphicsPipeLineMgr::Instance()->CreatePipeLine(
		LAYOUT_POS,
		SHADER_VERTEX_COLOR,
		SHADER_PIXCEL_COLOR,
		PIPELINE_DATA_BACKCARING_ALPHABLEND_WIREFLAME_HDR,
		ROOTSIGNATURE_DATA_DRAW,
		PIPELINE_NAME_COLOR_WIREFLAME_HDR
	);


	//FogLineForBgパイプライン
	GraphicsPipeLineMgr::Instance()->CreatePipeLine(
		LAYOUT_POS,
		SHADER_VERTEX_COLOR,
		SHADER_PIXCEL_LINE_FOG_FORBG,
		PIPELINE_DATA_NOCARING_ALPHABLEND_LINE_HDR,
		ROOTSIGNATURE_MODE_LINE,
		PIPELINE_NAME_FOG_LINE_FORBG_HDR
	);
#pragma endregion

}
