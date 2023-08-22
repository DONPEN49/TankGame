#include "Fire_Billboard.h"
#include "Manager_Billboard.h"
#include "CCamera.h"

Fire_Billboard::Fire_Billboard() {

}

Fire_Billboard::Fire_Billboard(DirectX::XMFLOAT3 _pos)
	: m_idU(0)
	, m_idV(0)
	, m_isDraw(true)
{
	SetBillboard(Manager_Billboard::GetInstance().GetBillboardPtr("assets/Texture/explosion2.png"));
	SetPos(_pos);
	SetSize(13.0f, 13.0f);
	SetColor(DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
}

void Fire_Billboard::Init() {

}

void Fire_Billboard::Update() {

}

void Fire_Billboard::Draw() {
	m_idU += ANIMESPEED;
	if (m_idU > 7) {
		m_idV++;
		m_idU = 0;
	}
	if (m_idV > 7) {
		m_idV = 0;
		m_isDraw = false;
	}

	if (m_isDraw) {
		float u[4] = { 0.125f * (int)m_idU, 0.125f * (int)m_idU, 0.125f * ((int)m_idU + 1), 0.125f * ((int)m_idU + 1) };
		float v[4] = { 0.1667f * ((int)m_idV + 1), 0.1667f * (int)m_idV,0.1667f * ((int)m_idV + 1), 0.1667f * (int)m_idV };
		SetUV(u, v);
		m_bil->DrawBillBoard(CCamera::GetInstance()->GetCameraMatrix());
	}
}