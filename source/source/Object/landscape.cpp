#include "landscape.h"
#include "IScene.h"
#include "CModel.h"
#include "ModelMgr.h"
#include "DX11util.h"
#include "StageHit.h"

/// <summary>
/// コンストラクタ
/// </summary>
LandScape::LandScape(IScene& _scene) {
	m_pScene = &_scene;
	m_pScene->AddSceneObject(this);
	m_tag = "LandScape";
	m_render = NOSHADOW;
}

/// <summary>
/// デストラクタ
/// </summary>
LandScape::~LandScape() {
	StageHitExit();
}

/// <summary>
/// 初期化
/// </summary>
/// <returns></returns>
bool LandScape::Init() {

	if (m_pScene->GetSceneTag() == "Title") {
		SetModel(ModelMgr::GetInstance().GetModelPtr("assets/stage/land.x"));
		DX11MtxScale(0.01f, 0.01f, 0.01f, m_mtx);
	}
	else {
		//SetModel(ModelMgr::GetInstance().GetModelPtr("assets/stage/land.x"));
		SetModel(ModelMgr::GetInstance().GetModelPtr("assets/land/stage_plane_plane_1.x"));
		StageHitInit(m_pModel);
	}
	
	if (m_pModel == nullptr) {
		return false;
	}

	return true;

}

/// <summary>
/// モデル描画
/// </summary>
void LandScape::DrawModel() {
	
	GetLight().SetMtxWorld(m_mtx);

	m_pModel->DrawModel(m_mtx);
}

void LandScape::DrawShadow() {

	GetLight().SetMtxWorld(m_mtx);

	m_pModel->DrawShadow(m_mtx);
}


