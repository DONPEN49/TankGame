#pragma once

#include "DX11util.h"
#include "Singleton.h"

class ShadowMap : public Singleton<ShadowMap>{

private:

	friend class Singleton<ShadowMap>;

	static constexpr int DEPTHTEX_WIDTH = 4096 * 4;
	static constexpr int DEPTHTEX_HEIGHT = 4096 * 4;

	static constexpr int SHADOWMAP_NUM = 3;

	static constexpr int DEPTHTEX[SHADOWMAP_NUM] = { 4096 * 4, 2048, 1024 };

	// �V���h�E�}�b�v�p�萔�o�b�t�@
	struct ConstantBufferShaderShadowmap {
		DirectX::XMFLOAT4X4 ViewFromLight;				// �����̈ʒu�J����
		DirectX::XMFLOAT4X4 ProjectionFromLight;		// �����̈ʒu�J�����ɑΉ������v���W�F�N�V�����ϊ��s��
		DirectX::XMFLOAT4X4 ScreenToUVCoord;			// �X�N���[�����W���e�N�X�`�����W��Ԃɕϊ�
	};

	struct ShadowState {
		ComPtr<ID3D11Texture2D>				DepthMapTex;					// �e�N�X�`��
		ComPtr<ID3D11ShaderResourceView>	DepthMapSRV;					// �V�F�[�_�[���\�[�X�r���[
		ComPtr<ID3D11RenderTargetView>		DepthMapRTV;					// �����_�[�^�[�Q�b�g�r���[
		ComPtr<ID3D11Texture2D>*			DSTex;							// �e�N�X�`���iDepthStencil�j
		ComPtr<ID3D11DepthStencilView>*		DSTexDSV;						// �c�r�u
		ComPtr<ID3D11Buffer>*				ConstantBufferShadowmap;		// �V���h�E�}�b�v�p�萔�o�b�t�@

	};

	// ���_�t�H�[�}�b�g��`
	ComPtr<ID3D11InputLayout>			pVertexLayout;

	ComPtr<ID3D11Texture2D>				DepthMapTex;		// �e�N�X�`��
	ComPtr<ID3D11ShaderResourceView>	DepthMapSRV;		// �V�F�[�_�[���\�[�X�r���[
	ComPtr<ID3D11RenderTargetView>		DepthMapRTV;		// �����_�[�^�[�Q�b�g�r���[

	ComPtr<ID3D11Texture2D>				DSTex;				// �e�N�X�`���iDepthStencil�j
	ComPtr<ID3D11DepthStencilView>		DSTexDSV;			// �c�r�u

	ComPtr<ID3D11Buffer>				ConstantBufferShadowmap;		// �V���h�E�}�b�v�p�萔�o�b�t�@

	//�X�N���[�����W��UV���W�ɕϊ�����s��
	ALIGN16 XMMATRIX uvmatrix;

	/*float cascadeAreaTbl[SHADOWMAP_NUM];*/

	ShadowState shadowState[SHADOWMAP_NUM];

public:

	enum SHADERTYPE {
		SHADOW,
		MODEL,

	};

	bool Init();

	void RenderModel();

	void RenderDepthMap();

	void SetShadowShader(ID3D11DeviceContext*&);

	void Release();
};