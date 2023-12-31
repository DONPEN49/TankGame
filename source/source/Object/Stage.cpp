#include "Stage.h"
#include "ModelMgr.h"

/// <summary>
/// 初期化
/// </summary>
/// <param name="modelname"> ファイル名</param>
/// <returns>　初期化できたか</returns>
bool Stage_Object::Init(std::string modelname) {

	SetModel(ModelMgr::GetInstance().GetModelPtr(modelname));
	if (m_pModel == nullptr) {
		char str[128];
		sprintf_s<128>(str, "%s", modelname);
		MessageBox(nullptr, str, "load error", MB_OK);
		assert(false);
		return false;
	}

	return true;

}

/// <summary>
/// 描画
/// </summary>
void Stage_Object::Draw() {

	m_pModel->Draw(this->m_mtx);

}

/// <summary>
/// 描画
/// </summary>
void Stage_Object::DrawShadow() {

	m_pModel->DrawShadow(this->m_mtx);

}

/// <summary>
/// 描画
/// </summary>
void Stage_Object::DrawModel() {

	m_pModel->DrawModel(this->m_mtx);

}

/// <summary>
/// 終了
/// </summary>
void Stage_Object::Finalize() {

}