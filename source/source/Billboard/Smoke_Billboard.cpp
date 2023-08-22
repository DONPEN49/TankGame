#include "Smoke_Billboard.h"
#include "Manager_Billboard.h"
#include "CCamera.h"

/// <summary>
/// コンストラクタ
/// </summary>
Smoke_Billboard::Smoke_Billboard()
	:m_speed(0.05f)
	, m_isDraw(true)
	, m_idU(0)
{

}

Smoke_Billboard::Smoke_Billboard(XMFLOAT3 _pos)
	:m_speed(0.05f)
	, m_pos(_pos)
	, m_isDraw(true)
	, m_idU(0)
{

}


/// <summary>
/// 初期化
/// </summary>
void Smoke_Billboard::Init() {

	SetBillboard(Manager_Billboard::GetInstance().GetBillboardPtr("assets/Texture/tanksmoke.png"));
	SetSize(5.0f, 5.0f);
	SetColor(DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));

}


/// <summary>
/// 更新
/// </summary>
void Smoke_Billboard::Update() {

	float move;

	move = m_speed;

	m_pos.y += m_speed;

	SetPos(m_pos);

}

/// <summary>
/// 描画
/// </summary>
void Smoke_Billboard::Draw() {

	m_idU += ANIMESPEED;
	if (m_idU > 10) {
		m_idV++;
		m_idU = 0;
		m_isDraw = false;
	}


	if (m_isDraw) {
		float u[4] = { 0.1f * (int)m_idU, 0.1f * (int)m_idU, 0.1f * ((int)m_idU + 1), 0.1f * ((int)m_idU + 1) };
		float v[4] = { 1.0f, 0.0f,1.0f, 0.0f };
		SetUV(u, v);
		m_bil->DrawBillBoardAdd(CCamera::GetInstance()->GetCameraMatrix());
	}
}