#include "HP_UI.h"
#include "IScene.h"
#include "Texture2D_Manager.h"
#include "Player.h"

HP_UI::HP_UI(IScene& _scene) {
	m_pScene = &_scene;
	m_pScene->AddSceneUI(this);
	m_blend = BLENDSTATE::DEFAULT;
	m_tag = "HP";
}

bool HP_UI::Init() {

	SetQuad2D(Manager_Texture2D::GetInstance().GetTexture2DPtr("assets/texture/hp_2.png"));
	SetPos(XMFLOAT3(Application::CLIENT_WIDTH / 4 * 3 + 100, Application::CLIENT_HEIGHT / 2 - 300, 0));
	SetScale(20, 9);
	return true;
}

void HP_UI::Update() {

	const Player* player = m_pScene->GetSceneObject<Player>("Player");
	
}
