#include "Scene_Game.h"
#include "Shadowmap.h"
#include "Manager_Camera.h"
#include "ModelMgr.h"
#include "Player.h"
#include "Skydome.h"
#include "landscape.h"
#include "Enemy.h"
#include "HP_UI.h"
#include "Smoke_Billboard.h"
#include "Damage_UI.h"
#include "Crosshair_UI.h"
#include "Enemy_Manager.h"
#include "Bullet_Manager.h"
#include "Lader_UI.h"
#include "CCamera.h"

/// <summary>
/// ������
/// </summary>
void Game_Scene::Init() {

	this->m_tag = "Game";

	//�J�����̈ʒu�ݒ�
	Manager_Camera::GetInstance().SetLocalPos(XMFLOAT3(Manager_Camera::DEFPOSX, Manager_Camera::DEFPOSY, Manager_Camera::DEFPOSZ));

	//�I�u�W�F�N�g�ǉ�
	Skydome* skydome = new Skydome(*this);
	LandScape* land = new LandScape(*this);
	Player* player = new Player(*this);

	//UI�ǉ�
	HP_UI* hp = new HP_UI(*this);
	Crosshair_UI* cross = new Crosshair_UI(*this);
	Lader_UI* lader = new Lader_UI(*this);

	Enemy_Manager::GetInstance().Init(this);

	//������
	bool sts;
	for (auto obj : m_pListObject) {
		sts = obj->Init();
		if (!sts) {
			char str[128];
			sprintf_s<128>(str, "%s", obj->m_tag.c_str());
			MessageBox(nullptr, str, "Init error", MB_OK);
			assert(false);
		}
	}

	for (auto ui : m_pListUI) {
		sts = ui->Init();
		if (!sts) {
			char str[128];
			sprintf_s<128>(str, "%s", ui->m_tag.c_str());
			MessageBox(nullptr, str, "Init error", MB_OK);
			assert(false);
		}
	}

}

/// <summary>
/// ����
/// </summary>
void Game_Scene::Input() {

}

/// <summary>
/// �X�V
/// </summary>
void Game_Scene::Update() {

	//�t���X�^���J�����O�p�s��
	XMFLOAT4X4 view, proj, out;
	view = CCamera::GetInstance()->GetCameraMatrix();
	proj = CCamera::GetInstance()->GetProjectionMatrix();
	DX11MtxMultiply(out, view, proj);

	//��čX�V
	for (auto obj : m_pListObject)
		obj->Update();

	//�t���X�^���J�����O
	for (auto obj : m_pListObject) {
		if (obj->m_tag != "LandScape") {
			obj->m_isDraw = IsInFrustum(obj->GetPostion(), out);
		}
		
	}

	//�J�����̈ʒu�X�V
	auto player = GetSceneObject<Player>("Player");

	for (auto ui : m_pListUI)
		ui->Update();

	Manager_Camera::GetInstance().UpdateCamera(player->GetPlayerPipe());
}

/// <summary>
/// �`��
/// </summary>
void Game_Scene::Draw() {

	ShadowMap::GetInstance().RenderDepthMap();

	for (auto obj : m_pListObject) {
		if (obj->m_render == Object::RENDER::WITHSHADOW || obj->m_render == Object::RENDER::DEFAULTSHADOW) {
			if (obj->m_isDraw) {
				obj->DrawShadow();
			}

		}
	}

	ShadowMap::GetInstance().RenderModel();

	for (auto obj : m_pListObject) {
		if (obj->m_render == Object::RENDER::DEFAULT || obj->m_render == Object::RENDER::DEFAULTSHADOW) {
			if (obj->m_isDraw) {
				obj->Draw();
			}
		}
		else {
			if (obj->m_isDraw) {
				obj->DrawModel();
			}
		}
	}

	for (auto ui : m_pListUI) {

		if (ui->m_blend == BLENDSTATE::ADD) {
			ui->DrawAdd();
		}
		else {
			ui->Draw();
		}
	}
}

/// <summary>
/// �I��
/// </summary>
void Game_Scene::Dispose() {

	auto obj = m_pListObject.begin();
	while (obj != m_pListObject.end()) {
		obj = m_pListObject.erase(obj);
	}
	m_pListObject.clear();

	auto ui = m_pListUI.begin();
	while (ui != m_pListUI.end()) {
		ui = m_pListUI.erase(ui);
	}
	m_pListUI.clear();
}