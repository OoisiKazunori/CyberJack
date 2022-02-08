#include "FbxModelResourceMgr.h"
#include"../Loader/TextureResourceMgr.h"
#include"../Helper/KazHelper.h"
#include"../Helper/ResourceFilePass.h"

FbxModelResourceMgr::FbxModelResourceMgr()
{
	fbxManager = FbxManager::Create();
	FbxIOSettings *ios = FbxIOSettings::Create(fbxManager, IOSROOT);
	fbxManager->SetIOSettings(ios);



	fbxImporter = FbxImporter::Create(fbxManager, "");
	fbxScene = FbxScene::Create(fbxManager, "fbxScene");
	handle.reset(new HandleMaker);

}

FbxModelResourceMgr::~FbxModelResourceMgr()
{
	fbxImporter->Destroy();
	fbxScene->Destroy();
	fbxManager->Destroy();

	for (int i = 0; i < modelResource.size(); i++)
	{
		delete modelResource[i];
	}
	modelResource.shrink_to_fit();
	modelResource.clear();
}

short FbxModelResourceMgr::LoadModel(const string &MODEL_NAME)
{
	fbxImporter->Initialize(MODEL_NAME.c_str());
	fbxImporter->Import(fbxScene);
	//モデルの生成
	Model *model = new Model;
	model->name = MODEL_NAME;

	//Fbxノードの数を取得
	int nodeCount = fbxScene->GetNodeCount();

	//あらかじめ必要数分のメモリを確保する事でアドレスがずれるのを予防する
	model->nodes.reserve(nodeCount);
	//ルートノードから順に解析してモデルに流し込む
	ParseNodeRecursive(model, fbxScene->GetRootNode());




	//リソースに保管
	unsigned short vertByte = model->vertices.size() * sizeof(Model::VertexPosNormalUvSkin);
	unsigned short indexByte = model->indices.size() * sizeof(unsigned short);


	short num = handle->GetHandle();

	modelResource.push_back(new FbxResourceD);
	modelResource[num]->buffers.reset(new CreateGpuBuffer);
	short vertBuffetHandle = modelResource[num]->buffers->CreateBuffer(KazBufferHelper::SetVertexBufferData(vertByte));
	short indexBufferHandle = modelResource[num]->buffers->CreateBuffer(KazBufferHelper::SetIndexBufferData(indexByte));

	//バッファ転送-----------------------------------------------------------------------------------------------------
	modelResource[num]->buffers->TransData(vertBuffetHandle, model->vertices.data(), vertByte);
	modelResource[num]->buffers->TransData(indexBufferHandle, model->indices.data(), indexByte);
	//バッファ転送-----------------------------------------------------------------------------------------------------

	//バッファビュー設定-----------------------------------------------------------------------------------------------------
	modelResource[num]->vertexBufferView = KazBufferHelper::SetVertexBufferView(modelResource[num]->buffers->GetGpuAddress(vertBuffetHandle), vertByte, sizeof(model->vertices[0]));
	modelResource[num]->indexBufferView = KazBufferHelper::SetIndexBufferView(modelResource[num]->buffers->GetGpuAddress(indexBufferHandle), indexByte);
	//バッファビュー設定-----------------------------------------------------------------------------------------------------

	modelResource[num]->textureHandle = model->textureHandle;
	
	modelResource[num]->diffuse = model->diffuse;
	modelResource[num]->ambient = model->ambient;

	modelResource[num]->indicisNum = model->indices.size();

	modelResource[num]->bone = model->bones;



	//アニメーションの数を取得
	int animaStackCount = fbxImporter->GetAnimStackCount();


	for (int i = 0; i < animaStackCount; i++)
	{
		FbxAnimStack *animStack = nullptr;
		animStack = fbxScene->GetSrcObject<FbxAnimStack>(i);

		const char *animStackName = animStack->GetName();
		FbxTakeInfo *takeInfo = fbxScene->GetTakeInfo(animStackName);
		fbxScene->SetCurrentAnimationStack(animStack);

		modelResource[num]->startTime.push_back(takeInfo->mLocalTimeSpan.GetStart());
		modelResource[num]->endTime.push_back(takeInfo->mLocalTimeSpan.GetStop());
	}


	delete model;
	return num;
}

const FbxResourceD* FbxModelResourceMgr::GetResourceData(const short &HANDLE)
{
	if (KazHelper::IsitInAnArray(HANDLE, modelResource.size()))
	{
		return modelResource[HANDLE];
	}
}

void FbxModelResourceMgr::ParseNodeRecursive(Model *MODEL, FbxNode *FBX_NODE, NewNode *PARENT)
{
	//モデルにノードを追加
	MODEL->nodes.emplace_back();
	NewNode &node = MODEL->nodes.back();
	//ノード名を取得
	string name = FBX_NODE->GetName();

	//Fbxノードのローカル移動情報
	FbxDouble3 rotation = FBX_NODE->LclRotation.Get();
	FbxDouble3 scaling = FBX_NODE->LclScaling.Get();
	FbxDouble3 translation = FBX_NODE->LclTranslation.Get();

	//形式変換して代入
	node.rotation = FbxDoubleToXMVECTOR(rotation);
	node.scaling = FbxDoubleToXMVECTOR(scaling);
	node.translation = FbxDoubleToXMVECTOR(translation);

	//スケール、回転、平行移動の計算
	XMMATRIX matScaling, matRotatin, matTranslation;
	matScaling = XMMatrixScalingFromVector(node.scaling);
	matRotatin = XMMatrixRotationRollPitchYawFromVector(node.rotation);
	matTranslation = XMMatrixTranslationFromVector(node.translation);

	//ローカル変換行列の計算
	node.transform = XMMatrixIdentity();
	node.transform *= matScaling;
	node.transform *= matRotatin;
	node.transform *= matTranslation;

	//グローバル変換行列の計算
	node.globalTransform = node.transform;
	if (PARENT)
	{
		node.parent = PARENT;
		//親の変形を乗算
		node.globalTransform *= PARENT->globalTransform;
	}

	//FBXノードの情報を解析してノードに記録
	//FBXノードのメッシュ情報を解析
	FbxNodeAttribute *fbxNodeAttribute = FBX_NODE->GetNodeAttribute();

	if (fbxNodeAttribute)
	{
		if (fbxNodeAttribute->GetAttributeType() == FbxNodeAttribute::eMesh)
		{
			MODEL->meshNode = &node;
			ParseMesh(MODEL, FBX_NODE);
		}
	}

	//子ノードに対して再帰呼び出し
	for (int i = 0; i < FBX_NODE->GetChildCount(); i++)
	{
		ParseNodeRecursive(MODEL, FBX_NODE->GetChild(i), &node);
	}

}

void FbxModelResourceMgr::ParseMesh(Model *MODEL, FbxNode *FBX_NODE)
{
	//ノードのメッシュを取得
	FbxMesh *fbxMesh = FBX_NODE->GetMesh();

	//頂点座標読み取り
	ParseMeshVertices(MODEL, fbxMesh);
	//面を構成するデータの読み取り
	ParseMeshFaces(MODEL, fbxMesh);
	//マテリアルの読み取り
	ParseMaterial(MODEL, FBX_NODE);
	//ボーンの読み取り
	ParseSkin(MODEL, fbxMesh);
}

void FbxModelResourceMgr::ParseMeshVertices(Model *MODEL, FbxMesh *FBX_MESH)
{
	auto &vertices = MODEL->vertices;

	//頂点座標データの数
	const int controlPointsCount = FBX_MESH->GetControlPointsCount();

	//必要数だけ頂点データ配列を確保
	Model::VertexPosNormalUvSkin vert{};
	MODEL->vertices.resize(controlPointsCount, vert);

	//FBXメッシュの頂点座標配列を取得
	FbxVector4 *pCoord = FBX_MESH->GetControlPoints();

	for (int i = 0; i < controlPointsCount; i++)
	{
		Model::VertexPosNormalUvSkin &vertex = vertices[i];

		//座標のコピー
		vertex.pos.x = (float)pCoord[i][0];
		vertex.pos.y = (float)pCoord[i][1];
		vertex.pos.z = (float)pCoord[i][2];
	}
}

void FbxModelResourceMgr::ParseMeshFaces(Model *MODEL, FbxMesh *FBX_MESH)
{
	auto &vertices = MODEL->vertices;
	auto &indices = MODEL->indices;

	//1ファイルに複数メッシュのモデルは非対応
	assert(indices.size() == 0);
	//面の数
	const int polygonCount = FBX_MESH->GetPolygonCount();
	//UVデータの数
	const int textureUVCount = FBX_MESH->GetTextureUVCount();
	//UV名リスト
	FbxStringList uvNames;
	FBX_MESH->GetUVSetNames(uvNames);

	//面ごとの情報読み取り
	for (int i = 0; i < polygonCount; i++)
	{
		//面を構成する頂点の数を取得(3なら三角形ポリゴン)
		const int polygonSize = FBX_MESH->GetPolygonSize(i);
		assert(polygonSize <= 4);

		//1頂点ずつ処理
		for (int j = 0; j < polygonSize; j++)
		{
			unsigned short index = FBX_MESH->GetPolygonVertex(i, j);
			assert(index >= 0);

			//頂点法線読み込み
			Model::VertexPosNormalUvSkin &vertex = vertices[index];
			FbxVector4 normal;
			if (FBX_MESH->GetPolygonVertexNormal(i, j, normal))
			{
				vertex.normal.x = (float)normal[0];
				vertex.normal.y = (float)normal[1];
				vertex.normal.z = (float)normal[2];
			}

			//テクスチャUV読み込み
			if (textureUVCount > 0)
			{
				FbxVector2 uvs;
				bool lUnmappedUV;
				//0番決め打ちで読み込み
				if (FBX_MESH->GetPolygonVertexUV(i, j, uvNames[0], uvs, lUnmappedUV))
				{
					vertex.uv.x = (float)uvs[0];
					vertex.uv.y = (float)uvs[1];
				}
			}

			//インデックス配列に頂点インデックス追加
			//3頂点目までなら
			if (j < 3)
			{
				//1点追加し、他の2点と三角形を構築する
				indices.push_back(index);
			}
			//4頂点目
			else
			{
				//3点追加し、
				//四角形の0,1,2,3の内,2,3,0で三角形を構築する
				int index2 = indices[indices.size() - 1];
				int index3 = index;
				int index0 = indices[indices.size() - 3];
				indices.push_back(index2);
				indices.push_back(index3);
				indices.push_back(index0);
			}
		}
	}

}

void FbxModelResourceMgr::ParseMaterial(Model *MODEL, FbxNode *FBX_NODE)
{
	const int materialCount = FBX_NODE->GetMaterialCount();
	if (materialCount > 0)
	{
		//先頭のマテリアルを取得
		FbxSurfaceMaterial *material = FBX_NODE->GetMaterial(0);
		//テクスチャ読み込みしたかどうか
		bool textureLoaded = false;

		if (material)
		{
			//FbxSurfaceLamvertクラスかどうかを調べる
			if (material->GetClassId().Is(FbxSurfaceLambert::ClassId))
			{
				FbxSurfaceLambert *lambert = static_cast<FbxSurfaceLambert *>(material);

				//環境光係数
				FbxPropertyT<FbxDouble3> ambient = lambert->Ambient;
				MODEL->ambient.x = (float)ambient.Get()[0];
				MODEL->ambient.y = (float)ambient.Get()[1];
				MODEL->ambient.z = (float)ambient.Get()[2];

				//拡散反射光係数
				FbxPropertyT<FbxDouble3> diffuse = lambert->Diffuse;
				MODEL->diffuse.x = (float)diffuse.Get()[0];
				MODEL->diffuse.y = (float)diffuse.Get()[1];
				MODEL->diffuse.z = (float)diffuse.Get()[2];
			}

			//ディヒューズテクスチャを取り出す
			const FbxProperty diffuseProperty = material->FindProperty(FbxSurfaceMaterial::sDiffuse);
			if (diffuseProperty.IsValid())
			{
				const FbxFileTexture *texture = diffuseProperty.GetSrcObject<FbxFileTexture>();
				if (texture)
				{
					const char *filePath = texture->GetRelativeFileName();
					//ファイルパスからファイル名抽出
					string path_str(filePath);
					string name = ExtractFileName(path_str);

					//テクスチャ読み込み
					LoadTexture(MODEL, name);
					textureLoaded = true;
				}
			}
		}

		//テクスチャが無い場合
		if (!textureLoaded)
		{
			short handle = TextureResourceMgr::Instance()->LoadGraph(KazFilePathName::TestPath + "white1x1.png");
			MODEL->textureHandle.push_back(handle);
		}
	}
}


void FbxModelResourceMgr::LoadTexture(Model *MODEL, const string &FULL_PATH)
{
	// ファイル名を取得
	std::string file_path = FULL_PATH;

	// ファイル分解
	char buffer[256];
	ZeroMemory(buffer, sizeof(char) * 256);
	memcpy(buffer, file_path.c_str(), sizeof(char) * 256);

	char *file_name = buffer;
	string rename = file_name;
	string rename2 = MODEL->name;

	rename.erase(0, 3);
	rename.insert(0, rename2);

	//名前の変更
	short handle = TextureResourceMgr::Instance()->LoadGraph(rename);
	MODEL->textureHandle.push_back(handle);
}

XMVECTOR FbxModelResourceMgr::FbxDoubleToXMVECTOR(const FbxDouble3 &DOUBLE_3)
{
	XMVECTOR result;
	result =
	{
		static_cast<float>(DOUBLE_3[0]),
		static_cast<float>(DOUBLE_3[1]),
		static_cast<float>(DOUBLE_3[2]),
		0.0f
	};

	return result;
}

string FbxModelResourceMgr::ExtractFileName(const string &PATH)
{
	size_t pos1;

	//区切り文字'￥￥'が出てくる一番最後の部分を検索
	pos1 = PATH.rfind('¥¥');
	if (pos1 != string::npos)
	{
		return PATH.substr(pos1 + 1, PATH.size() - pos1 - 1);
	}

	//区切り文字'/'が出てくる一番最後の部分を検索
	pos1 = PATH.rfind('/');
	if (pos1 != string::npos)
	{
		return PATH.substr(pos1 + 1, PATH.size() - pos1 - 1);
	}

	//パスを含まないファイル名
	return PATH;
}

void FbxModelResourceMgr::ConvertMatrixFromFbx(XMMATRIX *DST, const FbxAMatrix &SRC)
{
}

void FbxModelResourceMgr::ParseSkin(Model *MODEL, FbxMesh *FBX_MESH)

{
	//スキニング情報読み込み
	FbxSkin *fbxSkin = static_cast<FbxSkin *>(FBX_MESH->GetDeformer(0, FbxDeformer::eSkin));
	//スキニング情報が無ければ終了
	if (fbxSkin == nullptr)
	{
		for (int i = 0; i < MODEL->vertices.size(); i++)
		{
			MODEL->vertices[i].boneIndex[0] = 0;
			MODEL->vertices[i].boneWeight[0] = 1.0f;
		}
		return;
	}
	//ボーン配列の参照
	vector<Model::Bone> &bones = MODEL->bones;

	//ボーンの数
	int clusterCount = fbxSkin->GetClusterCount();
	bones.reserve(clusterCount);


	//初期姿勢行列の読み込み--------------------------------------------------
	for (int i = 0; i < clusterCount; i++)
	{
		//Fbxボーン情報
		FbxCluster *fbxCluster = fbxSkin->GetCluster(i);

		//ボーン自体の名前を取得
		const char *boneName = fbxCluster->GetLink()->GetName();

		//新しくボーンを追加し、追加したボーンの参照を得る....問題
		bones.emplace_back(Model::Bone(boneName));
		Model::Bone &bone = bones.back();

		//自作ボーンとfbxのボーンを紐付ける
		bone.fbxCluster = fbxCluster;

		//Fbxから初期姿勢行列を取得する
		FbxAMatrix fbxMat;
		fbxCluster->GetTransformLinkMatrix(fbxMat);

		//XMMATRIX型に変換
		XMMATRIX initialPose;
		KazMath::ConvertMatrixFromFbx(&initialPose, fbxMat);

		//初期姿勢行列の逆行列を得る
		bone.invInitialPose = XMMatrixInverse(nullptr, initialPose);
	}


	//スキンウェイト読み取り--------------------------------------------------

	//二次元配列(ジャグ配列)
	//list、頂点が影響を受けるボーンの全リスト
	//vector、それを全頂点分
	//問題
	vector<list<NewWeightSet>> weightLists(MODEL->vertices.size());

	//全てのボーンについて
	for (int i = 0; i < clusterCount; i++)
	{
		//Fbxのボーン情報
		FbxCluster *fbxCluster = fbxSkin->GetCluster(i);

		//このボーンに影響を受ける頂点の数
		int controlPointIndicesCount = fbxCluster->GetControlPointIndicesCount();

		//このボーンに影響を受ける頂点の配列
		int *controlPointIndices = fbxCluster->GetControlPointIndices();
		double *controlPointWeights = fbxCluster->GetControlPointWeights();

		//影響を受ける全頂点について
		for (int j = 0; j < controlPointIndicesCount; j++)
		{
			//頂点番号
			int vertIndex = controlPointIndices[j];
			//スキンウェイト
			float weight = (float)controlPointWeights[j];
			//その頂点の影響を受けるボーンリストに、ボーンとウェイトのペアを追加
			weightLists[vertIndex].emplace_back(NewWeightSet{ (UINT)i,weight });
		}
	}


	//スキンウェイトの整理--------------------------------------------------

	//頂点配列書き換え用の参照
	auto &vertices = MODEL->vertices;

	//各頂点についての処理
	for (int i = 0; i < vertices.size(); i++)
	{
		//頂点のウェイトから最も大きい4つを選択
		auto &weightList = weightLists[i];
		//大小比較用のラムダ式を指定して降順にソート
		weightList.sort([](auto const &lhs, auto const &rhs)
			{
				return lhs.weight > rhs.weight;
			}
		);

		int weightArrayIndex = 0;
		//降順ソート済みのウェイトリストから
		for (auto &weightSet : weightList)
		{
			//頂点データに書き込み
			vertices[i].boneIndex[weightArrayIndex] = weightSet.index;
			vertices[i].boneWeight[weightArrayIndex] = weightSet.weight;

			if (++weightArrayIndex >= Model::MAX_BONE_INDICES)
			{
				float weight = 0.0f;
				//二個目以降のウェイトを合計
				for (int j = 0; j < Model::MAX_BONE_INDICES; j++)
				{
					weight += vertices[i].boneWeight[j];
				}

				//合計で1.0f(100％)になるように調整
				vertices[i].boneWeight[0] = 1.0f - weight;
				break;
			}
		}
	}


	bones[0].fbxCluster;

}
