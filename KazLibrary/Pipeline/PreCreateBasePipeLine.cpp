#include "PreCreateBasePipeLine.h"
#include"../Pipeline/GraphicsRootSignature.h"
#include"../Helper/ResourceFilePass.h"

PreCreateBasePipeLine::PreCreateBasePipeLine()
{
	GraphicsPipeLineMgr* pipelineMgr = GraphicsPipeLineMgr::Instance();


#pragma region InputLayOutの登録
	{
		D3D12_INPUT_ELEMENT_DESC* input3DLayOut = new D3D12_INPUT_ELEMENT_DESC[3];

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


		InputLayOutData* input = new InputLayOutData();
		input->inputLayOut = input3DLayOut;
		input->size = 3;
		GraphicsPipeLineMgr::Instance()->RegisterInputLayOutWithData(*input, LAYOUT_POS_NORMAL_TEX);
	};

	{
		D3D12_INPUT_ELEMENT_DESC* input2DLayOut = new D3D12_INPUT_ELEMENT_DESC[2];

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


		InputLayOutData* input = new InputLayOutData;
		input->inputLayOut = input2DLayOut;
		input->size = 2;
		GraphicsPipeLineMgr::Instance()->RegisterInputLayOutWithData(*input, LAYOUT_POS_TEX);
	}




	{
		D3D12_INPUT_ELEMENT_DESC* inputColorLayOut = new D3D12_INPUT_ELEMENT_DESC;

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

		InputLayOutData* input = new InputLayOutData;

		input->inputLayOut = inputColorLayOut;
		input->size = 1;
		GraphicsPipeLineMgr::Instance()->RegisterInputLayOutWithData(*input, LAYOUT_POS);
	}




	{
		D3D12_INPUT_ELEMENT_DESC* inputFbxLayOut = new D3D12_INPUT_ELEMENT_DESC[5];

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


		InputLayOutData* input = new InputLayOutData;

		input->inputLayOut = inputFbxLayOut;
		input->size = 5;
		GraphicsPipeLineMgr::Instance()->RegisterInputLayOutWithData(*input, LAYOUT_POS_NORMAL_TEX_BONE_WEIGHT);
	}

	{
		D3D12_INPUT_ELEMENT_DESC* inputLineLayOut = new D3D12_INPUT_ELEMENT_DESC[2];


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

		InputLayOutData* input = new InputLayOutData;
		input->inputLayOut = inputLineLayOut;
		input->size = 2;
		GraphicsPipeLineMgr::Instance()->RegisterInputLayOutWithData(*input, LAYOUT_POS_TICK);
	}
#pragma endregion

#pragma region シェーダーのコンパイルと登録
	OutputDebugStringA("シェーダーのコンパイルを開始します\n");
	//色シェーダー
	pipelineMgr->RegisterVertexShaderWithData(KazFilePathName::VertexShaderPath + "BasicVertexShader.hlsl", "VSmain", "vs_5_0", SHADER_VERTEX_COLOR);
	pipelineMgr->RegisterPixcelShaderWithData(KazFilePathName::PixelShaderPath + "BasicPixelShader.hlsl", "PSmain", "ps_5_0", SHADER_PIXCEL_COLOR);

	//テクスチャシェーダー...テクスチャ影あり
	pipelineMgr->RegisterVertexShaderWithData(KazFilePathName::VertexShaderPath + "TextureVertexShader.hlsl", "VSmain", "vs_5_0", SHADER_VERTEX_TEXTURE);
	pipelineMgr->RegisterPixcelShaderWithData(KazFilePathName::PixelShaderPath + "TexturePixelShader.hlsl", "PSmain", "ps_5_0", SHADER_PIXCEL_TEXTURE);


	//スプライトシェーダー...テクスチャ影無し
	pipelineMgr->RegisterVertexShaderWithData(KazFilePathName::VertexShaderPath + "SpriteVertexShader.hlsl", "VSmain", "vs_5_0", SHADER_VERTEX_SPRITE);
	pipelineMgr->RegisterPixcelShaderWithData(KazFilePathName::PixelShaderPath + "SpritePixelShader.hlsl", "PSmain", "ps_5_0", SHADER_PIXCEL_SPRITE);


	//objシェーダー
	pipelineMgr->RegisterVertexShaderWithData(KazFilePathName::VertexShaderPath + "OBJVertexShader.hlsl", "VSmain", "vs_5_0", SHADER_VERTEX_OBJ);
	pipelineMgr->RegisterVertexShaderWithData(KazFilePathName::VertexShaderPath + "OBJVertexExpantionShader.hlsl", "VSmain", "vs_5_0", SHADER_VERTEX_EXPANTION_OBJ);	// 頂点拡張用
	pipelineMgr->RegisterVertexShaderWithData(KazFilePathName::VertexShaderPath + "OBJGetShadowMapVertexShader.hlsl", "VSmain", "vs_5_0", SHADER_VERTEX_GET_SHADOWMAP);	// シャドウマップ取得用
	pipelineMgr->RegisterVertexShaderWithData(KazFilePathName::VertexShaderPath + "OBJDrawShadowMapVertexShader.hlsl", "VSmain", "vs_5_0", SHADER_VERTEX_DRAW_SHADOWMAP);	// シャドウマップを元に影描画用
	pipelineMgr->RegisterPixcelShaderWithData(KazFilePathName::PixelShaderPath + "OBJPixelShader.hlsl", "PSmain", "ps_5_0", SHADER_PIXCEL_OBJ);
	pipelineMgr->RegisterPixcelShaderWithData(KazFilePathName::PixelShaderPath + "OBJPixelExpantionShader.hlsl", "PSmain", "ps_5_0", SHADER_PIXCEL_EXPANTION_OBJ);	// 頂点拡張用
	pipelineMgr->RegisterPixcelShaderWithData(KazFilePathName::PixelShaderPath + "OBJGetShadowPixcelShader.hlsl", "PSmain", "ps_5_0", SHADER_PIXCEL_GET_SHADOWMAP);	// シャドウマップ取得用
	pipelineMgr->RegisterPixcelShaderWithData(KazFilePathName::PixelShaderPath + "OBJDrawShadowMapPixcelShader.hlsl", "PSmain", "ps_5_0", SHADER_PIXCEL_DRAW_SHADOWMAP);	// シャドウマップを元に影描画用

	pipelineMgr->RegisterVertexShaderWithData(KazFilePathName::VertexShaderPath + "InstanceObjGetShadowMapVertexShader.hlsl", "VSmain", "vs_5_0", SHADER_VERTEX_INSTANCE_GET_SHADOWMAP);	// シャドウマップ取得用
	pipelineMgr->RegisterPixcelShaderWithData(KazFilePathName::PixelShaderPath + "InstanceObjGetShadowPixelShader.hlsl", "PSmain", "ps_5_0", SHADER_PIXCEL_INSTANCE_GET_SHADOWMAP);	// シャドウマップを元に影描画用

	pipelineMgr->RegisterPixcelShaderWithData(KazFilePathName::PixelShaderPath + "InstanceObjExpantionPixelShader.hlsl", "PSmain", "ps_5_0", SHADER_PIXCEL_INSTANCE_OBJ_EXPANTION);


	//ライト
	pipelineMgr->RegisterVertexShaderWithData(KazFilePathName::VertexShaderPath + "LightVertexShader.hlsl", "VSmain", "vs_5_0", SHADER_VERTEX_LIGHT);
	pipelineMgr->RegisterPixcelShaderWithData(KazFilePathName::PixelShaderPath + "LightPixelShader.hlsl", "PSmain", "ps_5_0", SHADER_PIXCEL_LIGHT);


	//輝度抽出
	pipelineMgr->RegisterVertexShaderWithData(KazFilePathName::VertexShaderPath + "LumiVertexShader.hlsl", "VSmain", "vs_5_0", SHADER_VERTEX_LUMINAS);
	pipelineMgr->RegisterPixcelShaderWithData(KazFilePathName::PixelShaderPath + "LumiPixelShader.hlsl", "PSmain", "ps_5_0", SHADER_PIXCEL_LUMINAS);


	//ガウシアンブラー
	pipelineMgr->RegisterPixcelShaderWithData(KazFilePathName::PixelShaderPath + "GaussianPixelShader.hlsl", "GaussianUpBlur", "ps_5_0", SHADER_PIXCEL_GAUSSIAN_UPBLUR);
	pipelineMgr->RegisterPixcelShaderWithData(KazFilePathName::PixelShaderPath + "GaussianPixelShader.hlsl", "GaussianSideBlur", "ps_5_0", SHADER_PIXCEL_GAUSSIAN_SIDEBLUR);



	//fbx
	pipelineMgr->RegisterVertexShaderWithData(KazFilePathName::VertexShaderPath + "FbxModelVertexShader.hlsl", "VSmain", "vs_5_0", SHADER_VERTEX_FBX);
	pipelineMgr->RegisterPixcelShaderWithData(KazFilePathName::PixelShaderPath + "FbxModelPixelShader.hlsl", "PSmain", "ps_5_0", SHADER_PIXCEL_FBX);



	//bloom
	pipelineMgr->RegisterPixcelShaderWithData(KazFilePathName::PixelShaderPath + "BloomPixelShader.hlsl", "PSmain", "ps_5_0", SHADER_PIXCEL_BLOOM);


	//削除予定
	pipelineMgr->RegisterVertexShaderWithData(KazFilePathName::VertexShaderPath + "multipassVertexShader.hlsl", "VSmain", "vs_5_0", SHADER_VERTEX_MULTIPASS);


	//削除予定
	pipelineMgr->RegisterPixcelShaderWithData(KazFilePathName::PixelShaderPath + "GoalLightPixelShader.hlsl", "PSmain", "ps_5_0", SHADER_PIXCEL_GOALLIGHT);



	//Fog
	pipelineMgr->RegisterVertexShaderWithData(KazFilePathName::VertexShaderPath + "FogVertexShader.hlsl", "VSmain", "vs_5_0", SHADER_VERTEX_FOG_COLOR);
	pipelineMgr->RegisterPixcelShaderWithData(KazFilePathName::PixelShaderPath + "FogPixelShader.hlsl", "PSmain", "ps_5_0", SHADER_PIXCEL_FOG_COLOR);




	pipelineMgr->RegisterPixcelShaderWithData(KazFilePathName::PixelShaderPath + "FogObjPixelShader.hlsl", "PSmain", "ps_5_0", SHADER_PIXCEL_FOG_OBJ);


	pipelineMgr->RegisterPixcelShaderWithData(KazFilePathName::PixelShaderPath + "FogFbxPixelShader.hlsl", "PSmain", "ps_5_0", SHADER_PIXCEL_FOG_FBX);


	//太さ変えられる線用ジオメトリーシェーダー
	//pipelineMgr->RegisterGeometoryShaderWithData(KazFilePathName::RelativeShaderPath + "LineGeometryShader.hlsl", "GSmain", "gs_5_0", SHADER_GEOMETORY_LINE);


	//線
	pipelineMgr->RegisterVertexShaderWithData(KazFilePathName::VertexShaderPath + "LineVertexShader.hlsl", "VSmain", "vs_5_0", SHADER_VERTEX_LINE);
	pipelineMgr->RegisterPixcelShaderWithData(KazFilePathName::PixelShaderPath + "LinePixelShader.hlsl", "PSmain", "ps_5_0", SHADER_PIXCEL_LINE);



	pipelineMgr->RegisterPixcelShaderWithData(KazFilePathName::PixelShaderPath + "LineTickFogPixelShader.hlsl", "PSmain", "ps_5_0", SHADER_PIXCEL_TICK_LINE_FOG);



	pipelineMgr->RegisterPixcelShaderWithData(KazFilePathName::PixelShaderPath + "FogTexturePixelShader.hlsl", "PSmain", "ps_5_0", SHADER_PIXCEL_FOG_SPRITE);


	pipelineMgr->RegisterPixcelShaderWithData(KazFilePathName::PixelShaderPath + "FogLinePixelShader.hlsl", "PSmain", "ps_5_0", SHADER_PIXCEL_LINE_FOG);

	//削除予定
	pipelineMgr->RegisterPixcelShaderWithData(KazFilePathName::PixelShaderPath + "FogLineForEffectBGPixelShader.hlsl", "PSmain", "ps_5_0", SHADER_PIXCEL_LINE_FOG_FORBG);

	pipelineMgr->RegisterPixcelShaderWithData(KazFilePathName::PixelShaderPath + "LuminancePixelShader.hlsl", "PSmain", "ps_5_0", SHADER_PIXEL_LUMINANCE);


	pipelineMgr->RegisterVertexShaderWithData(KazFilePathName::VertexShaderPath + "ObjLightVertexShader.hlsl", "VSmain", "vs_5_0", SHADER_VERTEX_OBJ_LIGHT);
	pipelineMgr->RegisterPixcelShaderWithData(KazFilePathName::PixelShaderPath + "ObjLightPixelShader.hlsl", "PSmain", "ps_5_0", SHADER_PIXCEL_OBJ_LIGHT);


	pipelineMgr->RegisterVertexShaderWithData(KazFilePathName::VertexShaderPath + "InstanceVertexShader.hlsl", "VSmain", "vs_5_0", SHADER_VERTEX_INSTANCE_OBJ);
	pipelineMgr->RegisterPixcelShaderWithData(KazFilePathName::PixelShaderPath + "InstancePixelShader.hlsl", "PSmain", "ps_5_0", SHADER_PIXCEL_INSTANCE_OBJ);

	pipelineMgr->RegisterVertexShaderWithData(KazFilePathName::VertexShaderPath + "InstancePosVertexShader.hlsl", "VSmain", "vs_5_0", SHADER_VERTEX_INSTANCE_COLOR);
	pipelineMgr->RegisterPixcelShaderWithData(KazFilePathName::PixelShaderPath + "InstanceColorPixelShader.hlsl", "PSmain", "ps_5_0", SHADER_PIXCEL_INSTANCE_COLOR);

	pipelineMgr->RegisterVertexShaderWithData(KazFilePathName::VertexShaderPath + "InstanceColorGetShadowVertexShader.hlsl", "VSmain", "vs_5_0", SHADER_VERTEX_INSTANCE_COLOR_GET_SHADOWMAP);
	pipelineMgr->RegisterPixcelShaderWithData(KazFilePathName::PixelShaderPath + "InstanceColorGetShadowPixelShader.hlsl", "PSmain", "ps_5_0", SHADER_PIXCEL_INSTANCE_COLOR_GET_SHADOW);

	//コンピュートシェーダーのコンパイル
	pipelineMgr->RegisterComputeShaderWithData(KazFilePathName::ComputeShaderPath + "TestComputeShader.hlsl", "CSmain", "cs_5_0", SHADER_COMPUTE_TEST);

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
#pragma region PIPELINE_DATA_BACKCARING_NOBLEND
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
		gPipeline.RasterizerState = rasterrize;
		gPipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;

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
		gPipeline.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_ALWAYS;		//小さければOK
		gPipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;							//深度値フォーマット
		GraphicsPipeLineMgr::Instance()->RegisterPipeLineDataWithData(gPipeline, PIPELINE_DATA_NOCARING_ADDBLEND);
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
		gPipeline.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_ALWAYS;
		gPipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;							//深度値フォーマット
		GraphicsPipeLineMgr::Instance()->RegisterPipeLineDataWithData(gPipeline, PIPELINE_DATA_NOCARING_BLENDALPHA_DEPTH_ALWAYS);
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
	GraphicsPipeLineMgr::Instance()->CreateComputePipeLine(
		SHADER_COMPUTE_TEST,
		PIPELINE_COMPUTE_DATA_TEST,
		ROOTSIGNATURE_DATA_SRV_UAV_CBV,
		PIPELINE_COMPUTE_NAME_TEST
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

	//インスタンシング描画色パイプラインにシャドウを入れる
	GraphicsPipeLineMgr::Instance()->CreatePipeLine(
		LAYOUT_POS_NORMAL_TEX,
		SHADER_VERTEX_INSTANCE_COLOR_GET_SHADOWMAP,
		SHADER_PIXCEL_INSTANCE_COLOR_GET_SHADOW,
		PIPELINE_DATA_NOCARING_NOBLEND_R32,
		ROOTSIGNATURE_DATA_DATA1_DATA2,
		PIPELINE_NAME_INSTANCE_COLOR_GET_SHADOWMAP
	);


	//インスタンシング描画色パイプライン
	GraphicsPipeLineMgr::Instance()->CreatePipeLine(
		LAYOUT_POS,
		SHADER_VERTEX_INSTANCE_COLOR,
		SHADER_PIXCEL_INSTANCE_COLOR,
		PIPELINE_DATA_BACKCARING_ALPHABLEND,
		ROOTSIGNATURE_DATA_DATA1,
		PIPELINE_NAME_INSTANCE_COLOR
	);

	//インスタンシング描画色パイプライン
	GraphicsPipeLineMgr::Instance()->CreatePipeLine(
		LAYOUT_POS,
		SHADER_VERTEX_INSTANCE_COLOR,
		SHADER_PIXCEL_INSTANCE_COLOR,
		PIPELINE_DATA_BACKCARING_ALPHABLEND,
		ROOTSIGNATURE_DATA_DATA1,
		PIPELINE_NAME_INSTANCE_COLOR
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
		PIPELINE_DATA_NOCARING_NOBLEND,
		ROOTSIGNATURE_DATA_DRAW_TEX,
		PIPELINE_NAME_SPRITE_NOBLEND
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

	//インスタンシング描画頂点拡大のパイプライン
	GraphicsPipeLineMgr::Instance()->CreatePipeLine(
		LAYOUT_POS_NORMAL_TEX,
		SHADER_VERTEX_INSTANCE_OBJ,
		SHADER_PIXCEL_INSTANCE_OBJ_EXPANTION,
		PIPELINE_DATA_EXPANTION_VERTEX,
		ROOTSIGNATURE_DATA_DATA1_DATA2_DATA3_TEX,
		PIPELINE_NAME_INSTANCE_OBJ_EXPANTION_VERTEX
	);

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
	GraphicsPipeLineMgr::Instance()->CreatePipeLine(
		LAYOUT_POS_NORMAL_TEX,
		SHADER_VERTEX_INSTANCE_GET_SHADOWMAP,
		SHADER_PIXCEL_INSTANCE_GET_SHADOWMAP,
		PIPELINE_DATA_NOCARING_NOBLEND_R32,
		ROOTSIGNATURE_DATA_DATA1_DATA2_DATA3_TEX,
		PIPELINE_NAME_INSTANCE_OBJ_GET_SHADOWMAP
	);

	//シャドウマップを元に影描画用パイプライン
	GraphicsPipeLineMgr::Instance()->CreatePipeLine(
		LAYOUT_POS_NORMAL_TEX,
		SHADER_VERTEX_DRAW_SHADOWMAP,
		SHADER_PIXCEL_DRAW_SHADOWMAP,
		PIPELINE_DATA_NOCARING_NOBLEND,
		ROOTSIGNATURE_DATA_DRAW_TEX_TEX2_DATA1_DATA2,
		PIPELINE_NAME_OBJ_DRAW_SHADOWMAP
	);

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

	//加算合成
	GraphicsPipeLineMgr::Instance()->CreatePipeLine(
		LAYOUT_POS_TEX,
		SHADER_VERTEX_SPRITE,
		SHADER_PIXCEL_SPRITE,
		PIPELINE_DATA_NOCARING_ADDBLEND,
		ROOTSIGNATURE_DATA_DRAW_TEX,
		PIPELINE_NAME_ADDBLEND
	);

	//川瀬式ブルーム
	GraphicsPipeLineMgr::Instance()->CreatePipeLine(
		LAYOUT_POS_TEX,
		SHADER_VERTEX_SPRITE,
		SHADER_PIXCEL_BLOOM,
		PIPELINE_DATA_NOCARING_ADDBLEND,
		ROOTSIGNATURE_DATA_DRAW_TEX4,
		PIPELINE_NAME_BLOOM
	);

	//正規化デバイス上の加算合成画像描画
	GraphicsPipeLineMgr::Instance()->CreatePipeLine(
		LAYOUT_POS_TEX,
		SHADER_VERTEX_MULTIPASS,
		SHADER_PIXCEL_SPRITE,
		PIPELINE_DATA_NOCARING_ADDBLEND,
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
		PIPELINE_DATA_NOCARING_ALPHABLEND_LINE,
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

	//FogLineパイプライン
	GraphicsPipeLineMgr::Instance()->CreatePipeLine(
		LAYOUT_POS,
		SHADER_VERTEX_COLOR,
		SHADER_PIXCEL_LINE_FOG,
		PIPELINE_DATA_NOCARING_ALPHABLEND_LINE,
		ROOTSIGNATURE_MODE_LINE,
		PIPELINE_NAME_FOG_LINE
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

	//マルチパスは「テクスチャ用」、
	//Create2DPipeLineはスプライト
	//CreateGetLightPipelineはスプライト用
	//basicはスプライト用
	//ブレンド用のは一杯用意する必要がある


#pragma endregion







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
