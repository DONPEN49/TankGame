#include "Shadowmap.h"
#include "DX11util.h"
#include "DX11Settransform.h"
#include "shaderhashmap.h"
#include "CModel.h"
#include "ModelMgr.h"

// ���_�V�F�[�_�[
const char* vsshadowfilename[] = {
	"shader/vsshadowmap.hlsl",
	"shader/vsshadowmapsoft.hlsl",
};

// �s�N�Z���V�F�[�_�[
const char* psshadowfilename[] = {
	"shader/psshadowmap.hlsl",
	"shader/psshadowsoft.hlsl",
};


/// <summary>
/// �V���h�E�}�b�v������
/// </summary>
/// <returns></returns>
bool ShadowMap::Init() {

	uvmatrix = XMMATRIX(
		0.5f, 0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.0f, 1.0f
	);

	// ���_�f�[�^�̒�`
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",		0, DXGI_FORMAT_R32G32B32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	unsigned int numElements = ARRAYSIZE(layout);

	// �s�N�Z���V�F�[�_�[����
	for (int i = 0; i < 2; i++) {
		bool sts = ShaderHashmap::GetInstance()->SetPixelShader(psshadowfilename[i]);
		if (!sts) {
			std::string f(psshadowfilename[i]);
			std::string errormessage;
			errormessage = f + "SetPixelShader error";
			MessageBox(nullptr, errormessage.c_str(), "error", MB_OK);
			return false;
		}
	}

	// ���_�V�F�[�_�[����
	for (int i = 0; i < 2; i++) {
		bool sts = ShaderHashmap::GetInstance()->SetVertexShader(
			vsshadowfilename[i],			// ���_�V�F�[�_�[�t�@�C����
			layout,					// ���_���C�A�E�g
			numElements);			// �G�������g��
		if (!sts) {
			std::string f(vsshadowfilename[i]);
			std::string errormessage;
			errormessage = f + " " + "SetVertexShader error";
			MessageBox(nullptr, errormessage.c_str(), "error", MB_OK);
			return false;
		}
	}

	ID3D11Device* device;
	device = CDirectXGraphics::GetInstance()->GetDXDevice();

	//for (int i = 0; i < SHADOWMAP_NUM; i++) {

	//	//�[�x�}�b�v�e�N�X�`���[���쐬

	//	D3D11_TEXTURE2D_DESC desc;
	//	ZeroMemory(&desc, sizeof(D3D11_TEXTURE2D_DESC));
	//	desc.Width = DEPTHTEX[i];
	//	desc.Height = DEPTHTEX[i];
	//	desc.MipLevels = 1;
	//	desc.ArraySize = 1;
	//	desc.Format = DXGI_FORMAT_R32_FLOAT;
	//	desc.CPUAccessFlags = 0;	// CPU����̓A�N�Z�X���Ȃ�
	//	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	//	desc.MiscFlags = 0;			// ���̑��̐ݒ�Ȃ�
	//	desc.SampleDesc.Count = 1;
	//	desc.SampleDesc.Quality = 0;
	//	desc.Usage = D3D11_USAGE_DEFAULT;      // �f�t�H���g�g�p�@

	//	// �V�F�[�_ ���\�[�X �r���[�̍쐬
	//	D3D11_SHADER_RESOURCE_VIEW_DESC srDesc;
	//	srDesc.Format = DXGI_FORMAT_R32_FLOAT;					// �t�H�[�}�b�g
	//	srDesc.ViewDimension = D3D_SRV_DIMENSION_TEXTURE2D;		// 2D�e�N�X�`��
	//	srDesc.Texture2D.MostDetailedMip = 0;					// �ŏ��̃~�b�v�}�b�v ���x��
	//	srDesc.Texture2D.MipLevels = -1;						// ���ׂẴ~�b�v�}�b�v ���x��

	//	// 2D�e�N�X�`���𐶐�
	//	HRESULT hr = device->CreateTexture2D(
	//		&desc,							// �쐬����2D�e�N�X�`���̐ݒ�
	//		nullptr,						// 
	//		&shadowState[i].DepthMapTex);					// �쐬�����e�N�X�`�����󂯎��ϐ�
	//	if (FAILED(hr))
	//		MessageBox(nullptr, "CreateTexture error", "Error", MB_OK);

	//	// �V�F�[�_ ���\�[�X �r���[�̍쐬
	//	hr = device->CreateShaderResourceView(
	//		shadowState[i].DepthMapTex.Get(),			// �A�N�Z�X����e�N�X�`�� ���\�[�X
	//		&srDesc,						// �V�F�[�_ ���\�[�X �r���[�̐ݒ�
	//		&shadowState[i].DepthMapSRV);				// �󂯎��ϐ�
	//	if (FAILED(hr))
	//		MessageBox(nullptr, "SRV error", "Error", MB_OK);

	//	// �����_�[�^�[�Q�b�g�r���[����
	//	hr = device->CreateRenderTargetView(shadowState[i].DepthMapTex.Get(),
	//		nullptr,
	//		&shadowState[i].DepthMapRTV);
	//	if (FAILED(hr))
	//		MessageBox(nullptr, "RTV error", "Error", MB_OK);
	//}

	//�[�x�}�b�v�e�N�X�`���[���쐬
	{
		D3D11_TEXTURE2D_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_TEXTURE2D_DESC));
		desc.Width = DEPTHTEX_WIDTH;
		desc.Height = DEPTHTEX_HEIGHT;
		desc.MipLevels = 1;
		desc.ArraySize = 1;
		desc.Format = DXGI_FORMAT_R32_FLOAT;
		desc.CPUAccessFlags = 0;	// CPU����̓A�N�Z�X���Ȃ�
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
		desc.MiscFlags = 0;			// ���̑��̐ݒ�Ȃ�
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.Usage = D3D11_USAGE_DEFAULT;      // �f�t�H���g�g�p�@

		// �V�F�[�_ ���\�[�X �r���[�̍쐬
		D3D11_SHADER_RESOURCE_VIEW_DESC srDesc;
		srDesc.Format = DXGI_FORMAT_R32_FLOAT; // �t�H�[�}�b�g

		srDesc.ViewDimension = D3D_SRV_DIMENSION_TEXTURE2D;		// 2D�e�N�X�`��
		srDesc.Texture2D.MostDetailedMip = 0;					// �ŏ��̃~�b�v�}�b�v ���x��
		srDesc.Texture2D.MipLevels = -1;						// ���ׂẴ~�b�v�}�b�v ���x��

		// 2D�e�N�X�`���𐶐�
		HRESULT hr = device->CreateTexture2D(
			&desc,							// �쐬����2D�e�N�X�`���̐ݒ�
			nullptr,						// 
			&DepthMapTex);				// �쐬�����e�N�X�`�����󂯎��ϐ�
		if (FAILED(hr))
			MessageBox(nullptr, "CreateTexture error", "Error", MB_OK);

		// �V�F�[�_ ���\�[�X �r���[�̍쐬
		hr = device->CreateShaderResourceView(
			DepthMapTex.Get(),			// �A�N�Z�X����e�N�X�`�� ���\�[�X
			&srDesc,						// �V�F�[�_ ���\�[�X �r���[�̐ݒ�
			&DepthMapSRV);				// �󂯎��ϐ�
		if (FAILED(hr))
			MessageBox(nullptr, "SRV error", "Error", MB_OK);

		// �����_�[�^�[�Q�b�g�r���[����
		hr = device->CreateRenderTargetView(DepthMapTex.Get(),
			nullptr,
			&DepthMapRTV);
		if (FAILED(hr))
			MessageBox(nullptr, "RTV error", "Error", MB_OK);


		//�f�v�X�X�e���V���r���[�p�̃e�N�X�`���[���쐬
		D3D11_TEXTURE2D_DESC descDepth;
		descDepth.Width = DEPTHTEX_WIDTH;
		descDepth.Height = DEPTHTEX_HEIGHT;
		descDepth.MipLevels = 1;
		descDepth.ArraySize = 1;
		descDepth.Format = DXGI_FORMAT_D32_FLOAT;

		descDepth.SampleDesc.Count = 1;
		descDepth.SampleDesc.Quality = 0;
		descDepth.Usage = D3D11_USAGE_DEFAULT;
		descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		descDepth.CPUAccessFlags = 0;
		descDepth.MiscFlags = 0;

		hr = device->CreateTexture2D(&descDepth, nullptr, &DSTex);
		if (FAILED(hr)) {
			MessageBox(nullptr, "CreateTexture2D error", "error", MB_OK);
			return false;
		}

		//���̃e�N�X�`���[�ɑ΂��f�v�X�X�e���V���r���[(DSV)���쐬
		hr = device->CreateDepthStencilView(DSTex.Get(), nullptr, &DSTexDSV);
		if (FAILED(hr)) {
			MessageBox(nullptr, "CreateDepthStencilView error", "error", MB_OK);
			return false;
		}
	}

	// �萔�o�b�t�@����
	bool sts = CreateConstantBufferWrite(
		device,
		sizeof(ConstantBufferShaderShadowmap),
		&ConstantBufferShadowmap);			// �V���h�E�}�b�v�p�萔�o�b�t�@
}

/// <summary>
/// �`�揈��
/// </summary>
void ShadowMap::RenderModel() {

	// �^�[�Q�b�g�o�b�t�@�N���A	
	float ClearColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };	//red,green,blue,alpha

	// �f�o�C�X�R���e�L�X�g�擾
	ID3D11DeviceContext* devcontext;
	devcontext = CDirectXGraphics::GetInstance()->GetImmediateContext();

	// �����_�����O�^�[�Q�b�g�r���[�A�f�v�X�X�e���V���r���[��ݒ�
	ID3D11RenderTargetView* rtv[] = { CDirectXGraphics::GetInstance()->GetRenderTargetView() };
	devcontext->OMSetRenderTargets(1, rtv, CDirectXGraphics::GetInstance()->GetDepthStencilView());

	// �r���[�|�[�g��ݒ�
	D3D11_VIEWPORT vp;

	vp.Width = static_cast<float>(CDirectXGraphics::GetInstance()->GetViewPortWidth());
	vp.Height = static_cast<float>(CDirectXGraphics::GetInstance()->GetViewPortHeight());
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	devcontext->RSSetViewports(1, &vp);

	// �^�[�Q�b�g�o�b�t�@�N���A
	devcontext->ClearRenderTargetView(CDirectXGraphics::GetInstance()->GetRenderTargetView(), ClearColor);
	// Z�o�b�t�@�N���A
	devcontext->ClearDepthStencilView(CDirectXGraphics::GetInstance()->GetDepthStencilView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	// �萔�o�b�t�@���U�ԃX���b�g�ɃZ�b�g
	devcontext->PSSetConstantBuffers(6, 1, ConstantBufferShadowmap.GetAddressOf());
	devcontext->VSSetConstantBuffers(6, 1, ConstantBufferShadowmap.GetAddressOf());

	// depthmap���Z�b�g
	devcontext->PSSetShaderResources(1, 1, DepthMapSRV.GetAddressOf());

	ID3D11InputLayout* layout;
	layout = ShaderHashmap::GetInstance()->GetVertexLayout(vsshadowfilename[MODEL]);
	ID3D11VertexShader* vsh;
	vsh = ShaderHashmap::GetInstance()->GetVertexShader(vsshadowfilename[MODEL]);
	ID3D11PixelShader* psh;
	psh = ShaderHashmap::GetInstance()->GetPixelShader(psshadowfilename[MODEL]);

	// ���_�t�H�[�}�b�g���Z�b�g
	devcontext->IASetInputLayout(layout);
	// ���_�V�F�[�_�[���Z�b�g
	devcontext->VSSetShader(vsh, nullptr, 0);
	// �s�N�Z���V�F�[�_�[���Z�b�g
	devcontext->PSSetShader(psh, nullptr, 0);

}

/// <summary>
/// �[�x�o�b�t�@��`��
/// </summary>
void ShadowMap::RenderDepthMap() {
	// �^�[�Q�b�g�o�b�t�@�N���A	
	float ClearColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f }; //red,green,blue,alpha

	// �f�o�C�X�R���e�L�X�g�擾
	ID3D11DeviceContext* devcontext;
	devcontext = CDirectXGraphics::GetInstance()->GetImmediateContext();

	// �����_�����O�^�[�Q�b�g�r���[�A�f�v�X�X�e���V���r���[��ݒ�
	ID3D11RenderTargetView* rtv[] = { DepthMapRTV.Get() };
	devcontext->OMSetRenderTargets(1, rtv, DSTexDSV.Get());

	// �r���[�|�[�g��ݒ�
	D3D11_VIEWPORT vp;
	vp.Width = DEPTHTEX_WIDTH;
	vp.Height = DEPTHTEX_HEIGHT;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	devcontext->RSSetViewports(1, &vp);

	// �^�[�Q�b�g�o�b�t�@�N���A
	devcontext->ClearRenderTargetView(DepthMapRTV.Get(), ClearColor);
	// Z�o�b�t�@�N���A
	devcontext->ClearDepthStencilView(DSTexDSV.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	// �[�x�o�b�t�@�𐶐�����ׂ̃V�F�[�_�[�����擾
	ID3D11InputLayout* layout;
	layout = ShaderHashmap::GetInstance()->GetVertexLayout(vsshadowfilename[SHADOW]);
	ID3D11VertexShader* vsh;
	vsh = ShaderHashmap::GetInstance()->GetVertexShader(vsshadowfilename[SHADOW]);
	ID3D11PixelShader* psh;
	psh = ShaderHashmap::GetInstance()->GetPixelShader(psshadowfilename[SHADOW]);

	// ���_�t�H�[�}�b�g���Z�b�g
	devcontext->IASetInputLayout(layout);
	// ���_�V�F�[�_�[���Z�b�g
	devcontext->VSSetShader(vsh, nullptr, 0);
	// �s�N�Z���V�F�[�_�[���Z�b�g
	devcontext->PSSetShader(psh, nullptr, 0);

	// �J�����̈ʒu
	XMFLOAT3 LightPos;
	/*static float m_angle = 0;*/
	LightPos.x = 400/* * cos(m_angle)*/;
	LightPos.z = 100/* * sin(m_angle)*/;
	LightPos.y = 500;

	XMFLOAT4 l;
	l.x = LightPos.x;
	l.y = LightPos.y;
	l.z = LightPos.z;
	l.w = 1.0f;
	GetLight().SetLightPos(l);
	GetLight().Update();

	/*m_angle += XM_PI / 180.0f;*/

	ALIGN16 XMVECTOR Eye = XMVectorSet(LightPos.x, LightPos.y, LightPos.z, 0.0f);
	ALIGN16 XMVECTOR At = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	ALIGN16 XMVECTOR Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	ALIGN16 XMMATRIX camera;
	camera = XMMatrixTranspose(XMMatrixLookAtLH(Eye, At, Up));

	// �r���[�ϊ��s����Z�b�g�i�����ʒu����̃J�����j
	ConstantBufferShaderShadowmap cb;
	XMStoreFloat4x4(&cb.ViewFromLight, camera);

	// �����J�����p�̃v���W�F�N�V�����ϊ��s��
	float	nearclip = 1.0f;
	float	farclip = 1000.0f;	// �t�@�[�N���b�v�̒l�����������ăo�O����
	float	Aspect = 1.0f;
	float	Fov = XM_PI / 3;

	ALIGN16 XMMATRIX ProjectionFromLight;

	ProjectionFromLight = XMMatrixTranspose(XMMatrixPerspectiveFovLH(Fov, Aspect, nearclip, farclip));
	XMStoreFloat4x4(&cb.ProjectionFromLight, ProjectionFromLight);		// �v���W�F�N�V�����ϊ��s����Z�b�g

	// �X�N���[�����W���t�u���W�ɕϊ�����s��
	XMStoreFloat4x4(&cb.ScreenToUVCoord, XMMatrixTranspose(uvmatrix));

	// �萔�o�b�t�@�ɔ��f������
	D3D11_MAPPED_SUBRESOURCE pData;

	HRESULT hr = devcontext->Map(ConstantBufferShadowmap.Get(),
		0,
		D3D11_MAP_WRITE_DISCARD,
		0,
		&pData);
	if (SUCCEEDED(hr)) {
		memcpy_s(pData.pData, pData.RowPitch, (void*)(&cb), sizeof(ConstantBufferShaderShadowmap));
		devcontext->Unmap(ConstantBufferShadowmap.Get(), 0);
	}

	// �萔�o�b�t�@���U�ԃX���b�g�ɃZ�b�g
	devcontext->PSSetConstantBuffers(6, 1, ConstantBufferShadowmap.GetAddressOf());
	devcontext->VSSetConstantBuffers(6, 1, ConstantBufferShadowmap.GetAddressOf());
}

void ShadowMap::SetShadowShader(ID3D11DeviceContext*& _device) {

	ID3D11InputLayout* layout;
	layout = ShaderHashmap::GetInstance()->GetVertexLayout(vsshadowfilename[MODEL]);
	ID3D11VertexShader* vsh;
	vsh = ShaderHashmap::GetInstance()->GetVertexShader(vsshadowfilename[MODEL]);
	ID3D11PixelShader* psh;
	psh = ShaderHashmap::GetInstance()->GetPixelShader(psshadowfilename[MODEL]);

	// ���_�t�H�[�}�b�g���Z�b�g
	_device->IASetInputLayout(layout);
	// ���_�V�F�[�_�[���Z�b�g
	_device->VSSetShader(vsh, nullptr, 0);
	// �s�N�Z���V�F�[�_�[���Z�b�g
	_device->PSSetShader(psh, nullptr, 0);
}

void ShadowMap::Release() {
	
}