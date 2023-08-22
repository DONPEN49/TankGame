#pragma once
#include	<d3d11.h>
#include	<DirectXMath.h>
#include	"Shader.h"

class CQuad {
private:
	ID3D11Buffer*		m_pConstantBuffer;	// �萔�o�b�t�@
	ID3D11Buffer*		m_pVertexBuffer;	// ���_�o�b�t�@
	ID3D11Buffer*		m_pIndexBuffer;		// �C���f�b�N�X�o�b�t�@
	ID3D11VertexShader* m_pVertexShader;	// ���_�V�F�[�_�[
	ID3D11PixelShader*  m_pPixelShader;		// �s�N�Z���V�F�[�_�[
	ID3D11InputLayout*  m_pVertexLayout;	// ���_���C�A�E�g

	struct Vertex{
		DirectX::XMFLOAT3	Pos;
		DirectX::XMFLOAT2	TexCoord;
	};

	Vertex				m_Vertex[4];		// ���_���W���S

	void SetVertexData(float x, float y, float width, float height) {
		// ���_���W�Z�b�g
		m_Vertex[0].Pos.x = x;
		m_Vertex[0].Pos.y = y;
		m_Vertex[0].Pos.z = 0.0f;
		m_Vertex[0].TexCoord.x = 0.0f;
		m_Vertex[0].TexCoord.y = 0.0f;

		m_Vertex[1].Pos.x = x + width;
		m_Vertex[1].Pos.y = y;
		m_Vertex[1].Pos.z = 0.0f;
		m_Vertex[1].TexCoord.x = 1.0f;
		m_Vertex[1].TexCoord.y = 0.0f;

		m_Vertex[2].Pos.x = x;
		m_Vertex[2].Pos.y = y - height;
		m_Vertex[2].Pos.z = 0.0f;
		m_Vertex[2].TexCoord.x = 0.0f;
		m_Vertex[2].TexCoord.y = 1.0f;

		m_Vertex[3].Pos.x = x + width;
		m_Vertex[3].Pos.y = y - height;
		m_Vertex[3].Pos.z = 0.0f;
		m_Vertex[3].TexCoord.x = 1.0f;
		m_Vertex[3].TexCoord.y = 1.0f;
	}

public:
	void ChangePosSize(float x, float y, float width, float height, ID3D11DeviceContext* deviceContext){
		D3D11_MAPPED_SUBRESOURCE pData;

		HRESULT hr = deviceContext->Map(m_pVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData);
		if (SUCCEEDED(hr)) {
			SetVertexData(x, y, width, height);
			memcpy_s(pData.pData, pData.RowPitch, (void*)(m_Vertex), sizeof(Vertex)*4);
			deviceContext->Unmap(m_pVertexBuffer, 0);
		}
	}
	void Draw(ID3D11DeviceContext* device) {

		// ���_�o�b�t�@���Z�b�g����
		unsigned int stride = sizeof(Vertex);
		unsigned  offset = 0;
		device->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);

		device->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);			// �C���f�b�N�X�o�b�t�@���Z�b�g
		device->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);		// �g�|���W�[���Z�b�g�i���v���~�e�B�u�^�C�v�j

		// ���_���C�A�E�g�Z�b�g
		device->IASetInputLayout(m_pVertexLayout);

		device->VSSetShader(m_pVertexShader, nullptr, 0);		// ���_�V�F�[�_�[���Z�b�g
		device->PSSetShader(m_pPixelShader, nullptr, 0);		// �s�N�Z���V�F�[�_�[���Z�b�g

		device->DrawIndexed(4,									// �`�悷��C���f�b�N�X��(4)
								0,								// �ŏ��̃C���f�b�N�X�o�b�t�@�̈ʒu
								0);								// ���_�o�b�t�@�̍ŏ�����g��
	}

	bool Init(float x,float y,float width ,float height,ID3D11Device* device) {

		bool sts;

		// ���_���W�Z�b�g	
		SetVertexData(x, y, width, height);

		// ���_�o�b�t�@�쐬
		sts = CreateVertexBufferWrite(
			device,
			sizeof(Vertex),			// �P���_������o�C�g��
			4,						// ���_��
			m_Vertex,				// ���_�f�[�^�i�[�������擪�A�h���X
			&m_pVertexBuffer);		// ���_�o�b�t�@
		if (!sts) {
			MessageBox(NULL, "CreateBuffer(vertex buffer) error", "Error", MB_OK);
			return false;
		}

		unsigned int idx[4] = {0,1,2,3};

		// �C���f�b�N�X�o�b�t�@�쐬
		sts = CreateIndexBuffer(
			device,
			4,				// �C���f�b�N�X��
			idx,			// �C���f�b�N�X�f�[�^�擪�A�h���X
			&m_pIndexBuffer); // �C���f�b�N�X�o�b�t�@
		if (!sts) {
			MessageBox(NULL, "CreateBuffer(index buffer) error", "Error", MB_OK);
			return false;
		}

		// ���_�f�[�^�̒�`
		D3D11_INPUT_ELEMENT_DESC layout[] =
		{
			{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};
		unsigned int numElements = ARRAYSIZE(layout);

		// ���_�V�F�[�_�[�I�u�W�F�N�g�𐶐��A�����ɒ��_���C�A�E�g������
		sts = CreateVertexShader(device,
			"shader/quad.fx",
			"main",
			"vs_5_0",
			layout,
			numElements,
			&m_pVertexShader,
			&m_pVertexLayout);
		if (!sts) {
			MessageBox(nullptr, "CreateVertexShader error", "error", MB_OK);
			return false;
		}

		// �s�N�Z���V�F�[�_�[�𐶐�
		sts = CreatePixelShader(			// �s�N�Z���V�F�[�_�[�I�u�W�F�N�g�𐶐�
			device,							// �f�o�C�X�I�u�W�F�N�g
			"shader/quad.fx",
			"PS",
			"ps_5_0",
			&m_pPixelShader);
		if (!sts) {
			MessageBox(nullptr, "CreatePixelShader error", "error", MB_OK);
			return false;
		}

		return true;
	}

	void Exit() {

		// �s�N�Z���V�F�[�_�[���
		if (m_pPixelShader) {
			m_pPixelShader->Release();
			m_pPixelShader = nullptr;
		}

		// ���_�V�F�[�_�[���
		if (m_pVertexShader) {
			m_pVertexShader->Release();
			m_pVertexShader = nullptr;
		}

		if (m_pVertexLayout) {
			m_pVertexLayout->Release();
			m_pVertexLayout = nullptr;
		}

		if (m_pIndexBuffer) {
			m_pIndexBuffer->Release();
			m_pIndexBuffer = nullptr;
		}

		if (m_pVertexBuffer) {
			m_pVertexBuffer->Release();
			m_pVertexBuffer = nullptr;
		}

	}
};