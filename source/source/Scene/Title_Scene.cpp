#include "Title_Scene.h"
#include "Texture2D_Manager.h"
#include "Skydome.h"
#include "tank.h"
#include "CModel.h"
#include "ModelMgr.h"
#include "Shadowmap.h"
#include "landscape.h"
#include "Manager_Camera.h"

/// <summary>
/// 初期化
/// </summary>
void Title_Scene::Init() {

	this->m_tag = "Title";

	// 単位行列化
	DX11MtxIdentity(mtx);

	//PLAY
	{
		UI* play = new UI(*this);
		play->m_tag = "play";
		play->SetQuad2D(Manager_Texture2D::GetInstance().GetTexture2DPtr("assets/texture/play.png"));
		play->SetPos(XMFLOAT3(Application::CLIENT_WIDTH / 4, Application::CLIENT_HEIGHT / 4 * 3.5f, 0));
		play->m_blend = BLENDSTATE::DEFAULT;
		play->SetScale(18, 12);
	}

	//EXIT
	{
		UI* exit = new UI(*this);
		exit->m_tag = "exit";
		exit->SetQuad2D(Manager_Texture2D::GetInstance().GetTexture2DPtr("assets/texture/exit.png"));
		exit->SetPos(XMFLOAT3(Application::CLIENT_WIDTH / 4 * 3, Application::CLIENT_HEIGHT / 4 * 3.5f, 0));
		exit->m_blend = BLENDSTATE::DEFAULT;
		exit->SetScale(18, 12);
	}

	//TITLE
	{
		UI* title = new UI(*this);
		title->m_tag = "title";
		title->SetQuad2D(Manager_Texture2D::GetInstance().GetTexture2DPtr("assets/texture/title.png"));
		title->SetPos(XMFLOAT3(Application::CLIENT_WIDTH / 4 * 2, Application::CLIENT_HEIGHT / 4 * 1.25f, 0));
		title->m_blend = BLENDSTATE::DEFAULT;
		title->SetScale(28, 22);
	}


	//オブジェクト追加
	Skydome* skydome = new Skydome(*this);

	LandScape* land = new LandScape(*this);

	tank = new Tank(this);
	//m_pTank->SetPos(XMFLOAT3(-15.4419365, 0.0f, -168.9f));

	//初期化
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

	return;
}

/// <summary>
/// 更新
/// </summary>
void Title_Scene::Update() {

	/*for (auto obj : listObject)
		obj->Update();

	for (auto ui : listUI)
		ui->Update();*/

	//auto m_pTank = GetSceneObject<Tank>("Tank");
	//m_pTank->UpdateSelect();

	static float azimuth = 0.0f;

	azimuth += 0.3f;

	Manager_Camera::GetInstance().UpdateSphereCamera(30.0f, -60.0f, azimuth, XMFLOAT3(0.0f, 0.0f, 0.0f));
		
}

/// <summary>
/// 描画
/// </summary>
void Title_Scene::Draw() {

	static float alpha;

	alpha += 0.05f;

	auto play = GetSceneUI<UI>("play");

	play->SetAlpha(sinf(alpha));

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
/// 終了
/// </summary>
void Title_Scene::Dispose() {

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