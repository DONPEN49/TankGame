#include "Exp_Billboard.h"
#include "Manager_Billboard.h"
#include "CCamera.h"

void Exp_Billboard::Init() {
	
	SetBillboard(Manager_Billboard::GetInstance().GetBillboardPtr("assets/Texture/Particle01.png"));
	SetSize(50.0f, 50.0f);
	SetColor(DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
	m_isDraw = true;
	m_idU = 0;
	m_idV = 0;
}

void Exp_Billboard::Update() {

}

void Exp_Billboard::Draw() {
	m_idU += ANIMESPEED;
	if (m_idU > 3) {
		m_idV++;
		m_idU = 0;
	}
	if (m_idV > 3) {
		m_idV = 0;
		m_isDraw = false;
	}

	float u[4] = { 0.25f * (int)m_idU, 0.25f * (int)m_idU, 0.25f * ((int)m_idU + 1), 0.25f * ((int)m_idU + 1) };
	float v[4] = { 0.25f * ((int)m_idV + 1), 0.25f * (int)m_idV,0.25f * ((int)m_idV + 1), 0.25f * (int)m_idV };
	SetUV(u, v);
	m_bil->DrawBillBoard(CCamera::GetInstance()->GetCameraMatrix());
}