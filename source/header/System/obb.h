#pragma once
#include	<DirectXMath.h>
#include	"CDirectxGraphics.h"
#include	"dx11mathutil.h"
#include	"CModel.h"
#include	"Box.h"

class COBB
{
public:
	// OBB�p�\����
	struct OBBinfo {
		DirectX::XMFLOAT3	vecAxisX;		// X��
		DirectX::XMFLOAT3	vecAxisY;		// Y��
		DirectX::XMFLOAT3	vecAxisZ;		// Z��
		DirectX::XMFLOAT3	center;			// ���S���W�i���[�J�����W�n�j
		DirectX::XMFLOAT3	currentcenter;	// BBOX�̌��݈ʒu
		float	fLengthX;		// �a�a�n�w �w�T�C�Y	
		float	fLengthY;		// �a�a�n�w �x�T�C�Y
		float	fLengthZ;		// �a�a�n�w �y�T�C�Y
	};

private:
	OBBinfo				m_obbinfo;	// OBB BBOX���
	DirectX::XMFLOAT4X4	m_matworld;	// OBB �`��p���[���h�ϊ��s��
	Box			m_boxmesh;			// BBOX mesh

	//bool CompareLength(OBB* pboxA,OBB* pboxB,D3DXVECTOR3* pvecSeparate,D3DXVECTOR3* pvecDistance)
	//�u�����v���u�Q�̉e�̍��v�v���傫���ꍇ�i��Փˁj��FALSE  ���������������ꍇ�i�Փˁj��TRUE
	// ���̊֐��́AOBBCollisionDetection�֐��݂̂ɌĂяo�����T�u���[�`��
	bool CompareLength(
		const OBBinfo& pboxA,
		const OBBinfo& pboxB,
		const DirectX::XMFLOAT3& pvecSeparate,		// ������
		const DirectX::XMFLOAT3& pvecDistance);		// �Q�̃I�u�W�F�N�g�����ԃx�N�g��

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
		DX11MtxIdentity(m_matworld);	// �P�ʍs��
	}

	~COBB() {
		Exit();
	}

	void Init(CModel* pmodel) {

		std::vector<XMFLOAT3> vertices;		// ���_�f�[�^
		std::vector<int> indices;			// �C���f�b�N�X�f�[�^

		const ModelData& md = pmodel->GetModelData();
		const std::vector<Mesh>& meshes = md.GetMeshes();

		// ���_�f�[�^�����o��
		for (auto& m : meshes) {
			for (auto& v : m.m_vertices) {
				vertices.emplace_back(v.m_Pos);
			}
		}

		CalculateBoundingBox(vertices);	// OBB�쐬
		CreateBoxMesh();				// ���b�V���쐬
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