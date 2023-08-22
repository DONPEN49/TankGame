#include "Stage.h"
#include "ModelMgr.h"

/// <summary>
/// ������
/// </summary>
/// <param name="modelname"> �t�@�C����</param>
/// <returns>�@�������ł�����</returns>
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
/// �`��
/// </summary>
void Stage_Object::Draw() {

	m_pModel->Draw(this->m_mtx);

}

/// <summary>
/// �`��
/// </summary>
void Stage_Object::DrawShadow() {

	m_pModel->DrawShadow(this->m_mtx);

}

/// <summary>
/// �`��
/// </summary>
void Stage_Object::DrawModel() {

	m_pModel->DrawModel(this->m_mtx);

}

/// <summary>
/// �I��
/// </summary>
void Stage_Object::Finalize() {

}