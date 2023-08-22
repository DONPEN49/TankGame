#include	<wrl/client.h>
#include	<d3d11.h>
#include	"DX11util.h"
#include	"blendstate.h"

using Microsoft::WRL::ComPtr;

// �u�����h�X�e�[�g
static ComPtr<ID3D11BlendState>	g_pBlendStateSrcAlpha;
static ComPtr<ID3D11BlendState>	g_pBlendStateOne;
static ComPtr<ID3D11BlendState>	g_pBlendStateDefault;
static ComPtr<ID3D11BlendState>	g_pBlendStateSubtract;

void InitBlendState() {
	CreateBlendStateSrcAlpha();
	// ���Z�����u�����h�X�e�[�g����
	CreateBlendStateOne();
	// �f�t�H���g�u�����h�X�e�[�g����
	CreateBlendStateDefault();
	// ���Z�����u�����h�X�e�[�g����
	CreateBlendStateSubtract();
}

// �\�[�X�A���t�@�u�����h�X�e�[�g����
void CreateBlendStateSrcAlpha() {

	D3D11_BLEND_DESC BlendDesc;

	ZeroMemory(&BlendDesc, sizeof(BlendDesc));
	BlendDesc.AlphaToCoverageEnable = FALSE;
	BlendDesc.IndependentBlendEnable = FALSE;		// �z���0�Ԃ̂ݎg�p

	BlendDesc.RenderTarget[0].BlendEnable = TRUE;

	// RGB�J���[
	BlendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	BlendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	BlendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;

	// �A���t�@�l
	BlendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	BlendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	BlendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;

	BlendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	ID3D11Device* dev;
	dev = GetDX11Device();

	dev->CreateBlendState(&BlendDesc, &g_pBlendStateSrcAlpha);
}

// ���Z�����u�����h�X�e�[�g����
void CreateBlendStateOne() {

	D3D11_BLEND_DESC BlendDesc;

	ZeroMemory(&BlendDesc, sizeof(BlendDesc));
	BlendDesc.AlphaToCoverageEnable = FALSE;
	BlendDesc.IndependentBlendEnable = FALSE;		// �z���0�Ԃ̂ݎg�p

	BlendDesc.RenderTarget[0].BlendEnable = TRUE;

	// RGB�v�Z
	BlendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	BlendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
	BlendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;

	// �A���t�@
	BlendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	BlendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	BlendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	BlendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	ID3D11Device* dev;
	dev = GetDX11Device();

	dev->CreateBlendState(&BlendDesc, &g_pBlendStateOne);
}

// �f�t�H���g�u�����h�X�e�[�g����
void CreateBlendStateDefault() {

	D3D11_BLEND_DESC BlendDesc;

	ZeroMemory(&BlendDesc, sizeof(BlendDesc));

	ZeroMemory(&BlendDesc, sizeof(BlendDesc));
	BlendDesc.AlphaToCoverageEnable = FALSE;
	BlendDesc.IndependentBlendEnable = FALSE;		// �z���0�Ԃ̂ݎg�p

	BlendDesc.RenderTarget[0].BlendEnable = FALSE;
	BlendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	BlendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;
	BlendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	BlendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	BlendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	BlendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	BlendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	ID3D11Device* dev;
	dev = GetDX11Device();

	dev->CreateBlendState(&BlendDesc, &g_pBlendStateDefault);
}

// ���Z�����u�����h�X�e�[�g����
void CreateBlendStateSubtract() {

	D3D11_BLEND_DESC BlendDesc;

	ZeroMemory(&BlendDesc, sizeof(BlendDesc));
	BlendDesc.AlphaToCoverageEnable = FALSE;
	BlendDesc.IndependentBlendEnable = FALSE;		// �z���0�Ԃ̂ݎg�p

	BlendDesc.RenderTarget[0].BlendEnable = TRUE;

	// RGB�J���[
	BlendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	BlendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
	BlendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_REV_SUBTRACT;

	// �A���t�@�l
	BlendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	BlendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	BlendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;

	BlendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	ID3D11Device* dev;
	dev = GetDX11Device();

	dev->CreateBlendState(&BlendDesc, &g_pBlendStateSubtract);
}

void SetBlendState(BLENDSTATE bs) {
	float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

	ID3D11DeviceContext* devcontext;
	devcontext = GetDX11DeviceContext();

	switch (bs) {
		case BLENDSTATE::DEFAULT:
			devcontext->OMSetBlendState(g_pBlendStateDefault.Get(), blendFactor, 0xffffffff);
			break;
		case BLENDSTATE::ADD:
			devcontext->OMSetBlendState(g_pBlendStateOne.Get(), blendFactor, 0xffffffff);
			break;
		case BLENDSTATE::SRCALPHA:
			devcontext->OMSetBlendState(g_pBlendStateSrcAlpha.Get(), blendFactor, 0xffffffff);
			break;
		case BLENDSTATE::SUB:
			devcontext->OMSetBlendState(g_pBlendStateSubtract.Get(), blendFactor, 0xffffffff);
			break;
	}
}

