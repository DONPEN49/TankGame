#include "Skydome.h"
#include "ModelMgr.h"
#include "DX11util.h"
#include "IScene.h"
#include "Player.h"
/// <summary>
/// コンストラクタ
/// </summary>
Skydome::Skydome() {
	m_tag = "Skydome";
	m_render = DEFAULT;
}
Skydome::Skydome(IScene& _scene) {

	//基底クラスメンバ変数初期化
	m_pScene = &_scene;
	m_tag = "Skydome";
	m_render = DEFAULT;
	m_pScene->AddSceneObject(this);
}

/// <summary>
/// 初期化
/// </summary>
bool Skydome::Init() {

	SetModel(ModelMgr::GetInstance().GetModelPtr("assets/skydome/Field.x"));

	if (m_pModel == nullptr) {
		return false;
	}
	return true;
}


void Skydome::Update() {

	//プレイヤー取得

	if (m_pScene->GetSceneTag() == "Game") {
		auto player = m_pScene->GetSceneObject<Player>("Player");

		SetPos(player->GetPostion());
	}
	
}

void Skydome::Draw() {

	m_mtx._41 = m_pos.x;
	m_mtx._42 = m_pos.y;
	m_mtx._43 = m_pos.z;

	m_pModel->Draw(m_mtx);

}

void Skydome::DrawShadow() {

	m_mtx._41 = m_pos.x;
	m_mtx._42 = m_pos.y;
	m_mtx._43 = m_pos.z;

	GetLight().SetMtxWorld(m_mtx);

	m_pModel->DrawShadow(m_mtx);

}

void Skydome::DrawModel() {

	m_mtx._41 = m_pos.x;
	m_mtx._42 = m_pos.y;
	m_mtx._43 = m_pos.z;

	GetLight().SetMtxWorld(m_mtx);

	m_pModel->DrawModel(m_mtx);

}