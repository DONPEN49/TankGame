#pragma once
#include	<DirectXMath.h>
#include	"CDirectxGraphics.h"
#include	"dx11mathutil.h"
#include	"CModel.h"
#include	"Box.h"

class COBB
{
public:
	// OBB用構造体
	struct OBBinfo {
		DirectX::XMFLOAT3	vecAxisX;		// X軸
		DirectX::XMFLOAT3	vecAxisY;		// Y軸
		DirectX::XMFLOAT3	vecAxisZ;		// Z軸
		DirectX::XMFLOAT3	center;			// 中心座標（ローカル座標系）
		DirectX::XMFLOAT3	currentcenter;	// BBOXの現在位置
		float	fLengthX;		// ＢＢＯＸ Ｘサイズ	
		float	fLengthY;		// ＢＢＯＸ Ｙサイズ
		float	fLengthZ;		// ＢＢＯＸ Ｚサイズ
	};

private:
	OBBinfo				m_obbinfo;	// OBB BBOX情報
	DirectX::XMFLOAT4X4	m_matworld;	// OBB 描画用ワールド変換行列
	Box			m_boxmesh;			// BBOX mesh

	//bool CompareLength(OBB* pboxA,OBB* pboxB,D3DXVECTOR3* pvecSeparate,D3DXVECTOR3* pvecDistance)
	//「距離」が「２つの影の合計」より大きい場合（非衝突）はFALSE  等しいか小さい場合（衝突）はTRUE
	// この関数は、OBBCollisionDetection関数のみに呼び出されるサブルーチン
	bool CompareLength(
		const OBBinfo& pboxA,
		const OBBinfo& pboxB,
		const DirectX::XMFLOAT3& pvecSeparate,		// 分離軸
		const DirectX::XMFLOAT3& pvecDistance);		// ２つのオブジェクトを結ぶベクトル

	void CreateBoxMesh() {
		m_boxmesh.Init(
			m_obbinfo.fLengthX,
			m_obbinfo.fLengthY,
			m_obbinfo.fLengthZ
		);
	}

	void CalculateBoundingBox(std::vector<XMFLOAT3>& vertices);

public:

	COBB() {
		DX11MtxIdentity(m_matworld);	// 単位行列化
	}

	~COBB() {
		Exit();
	}

	void Init(CModel* pmodel) {

		std::vector<XMFLOAT3> vertices;		// 頂点データ
		std::vector<int> indices;			// インデックスデータ

		const ModelData& md = pmodel->GetModelData();
		const std::vector<Mesh>& meshes = md.GetMeshes();

		// 頂点データを取り出す
		for (auto& m : meshes) {
			for (auto& v : m.m_vertices) {
				vertices.emplace_back(v.m_Pos);
			}
		}

		CalculateBoundingBox(vertices);	// OBB作成
		CreateBoxMesh();				// メッシュ作成
	}

	void Exit() {
		m_boxmesh.Exit();
	}

	void Update(DirectX::XMFLOAT4X4 matworld);

	void Draw();

	bool Collision(COBB& obb);

	DirectX::XMFLOAT3 GetPosition() {
		return m_obbinfo.currentcenter;
	}

	OBBinfo GetOBB() {
		return m_obbinfo;
	}
};