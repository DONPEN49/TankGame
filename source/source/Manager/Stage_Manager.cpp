#include "Stage_Manager.h"
#include "ModelMgr.h"
#include "dx11mathutil.h"
#include "Texture2D_Manager.h"
#include "BoundingSphere_Manager.h"
#include "CCamera.h"
#include "StageHit.h"

int  Stage_Manager::map[Stage_Manager::MAPSIZEZ][Stage_Manager::MAPSIZEX] = {
	
		4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,
		4,9,9,9,9,9,9,9,9,9,9,9,9,9,4,
		4,9,0,0,0,0,0,0,0,0,0,0,0,9,4,
		4,9,0,9,9,9,0,0,0,9,9,9,0,9,4,
		4,9,0,9,4,9,0,0,0,9,4,9,0,9,4,
		4,9,0,9,9,9,0,0,0,9,9,9,0,9,4,
		4,9,0,0,0,0,0,0,0,0,0,0,0,9,4,
		4,9,0,0,0,0,0,0,0,0,0,0,0,9,4,
		4,9,0,0,0,0,0,0,0,0,0,0,0,9,4,
		4,9,0,9,9,9,0,0,0,9,9,9,0,9,4,
		4,9,0,9,4,9,0,0,0,9,4,9,0,9,4,
		4,9,0,9,9,9,0,0,0,9,9,9,0,9,4,
		4,9,0,0,0,0,0,0,0,0,0,0,0,9,4,
		4,9,9,9,9,9,9,9,9,9,9,9,9,9,4,
		4,4,4,4,4,4,4,4,4,4,4,4,4,4,4
};

/// <summary>
/// �X�e�[�W�I�u�W�F�N�g����
/// </summary>
/// <returns></returns>
bool Stage_Manager::Generate(std::string _filename, XMFLOAT4X4 _mtx) {

	std::unique_ptr<Stage_Object> s;

	s = std::make_unique<Stage_Object>();

	bool sts = s->Init(_filename);
	if (!sts) {
		char str[128];
		sprintf_s<128>(str, "%s", _filename.c_str());
		MessageBox(nullptr, str, "Generate error", MB_OK);
		assert(false);
		return false;
	}

	//���[���h�s����Z�b�g
	s->SetMatrix(_mtx);

	stagelist.emplace_back(move(s));

	return true;
}

/// <summary>
/// ������
/// </summary>
/// <returns>�������ł�����</returns>
bool Stage_Manager::Init() {

	bool sts = true;

	XMFLOAT4X4 worldmtx;
	XMFLOAT3 angle = { 0,0,0 };
	XMFLOAT3 trans = { 0,0,0 };

	DX11MtxIdentity(worldmtx);

	/*sts = Generate("assets/land/land.x", worldmtx);
	if (!sts) {
		char str[128];
		sprintf_s<128>(str, "%s", "Stage_Manager");
		MessageBox(nullptr, str, "Init error", MB_OK);
		assert(false);
		return false;
	}*/

	/*modelmapchip = ModelMgr::GetInstance().GetModelPtr("assets/dice/Wall.x");

	wallradius = BoundingSphere_Manager::GetInstance().GetBSRadius("assets/dice/Wall.x");*/

	StageHitInit(ModelMgr::GetInstance().GetModelPtr("assets/land/Stage_StandByRoom_ver10.x"));

	return sts;
}

/// <summary>
/// �X�V
/// </summary>
void Stage_Manager::Update() {

	for (auto& stage : stagelist)
		stage->Update();
}

/// <summary>
/// �`��
/// </summary>
void Stage_Manager::Draw() {

	for (auto& stage : stagelist)
		stage->Draw();

	//�}�b�v�`�b�v�I�u�W�F�N�g�`��
	for (int z = -MAPSIZEZ / 2; z <= MAPSIZEZ / 2; z++) {
		for (int x = -MAPSIZEX / 2; x <= MAPSIZEX / 2; x++) {

			Chip3DDraw(z, x);
		}
	}
}

/// <summary>
/// �`��
/// </summary>
void Stage_Manager::DrawModel() {

	for (auto& stage : stagelist)
		stage->DrawModel();

	////�}�b�v�`�b�v�I�u�W�F�N�g�`��
	//for (int z = -MAPSIZEZ / 2; z <= MAPSIZEZ / 2; z++) {
	//	for (int x = -MAPSIZEX / 2; x <= MAPSIZEX / 2; x++) {

	//		Chip3DDraw(z, x);
	//	}
	//}
}

/// <summary>
/// �`��
/// </summary>
void Stage_Manager::DrawShadow() {

	for (auto& stage : stagelist)
		stage->DrawShadow();

	////�}�b�v�`�b�v�I�u�W�F�N�g�`��
	//for (int z = -MAPSIZEZ / 2; z <= MAPSIZEZ / 2; z++) {
	//	for (int x = -MAPSIZEX / 2; x <= MAPSIZEX / 2; x++) {

	//		Chip3DDraw(z, x);
	//	}
	//}
}


/// <summary>
/// �I��
/// </summary>
void Stage_Manager::Exit() {

	//���X�g�폜
	auto it = stagelist.begin();
	while (it != stagelist.end()) {
		it = stagelist.erase(it);
	}

	stagelist.clear();
}

void Stage_Manager::Chip3DDraw(int x, int z) {

	XMFLOAT4X4 mtx;
	DX11MtxIdentity(mtx);

	// ���s�ړ���
	XMFLOAT3 trans = { MAPCHIPWIDTH*x ,0.0f , MAPCHIPWIDTH*z };
	
	DX11MtxTranslation(trans, mtx);
	mtx._11 *= 2.0f;
	mtx._22 *= 2.0f;
	mtx._33 *= 2.0f;

	// �}�b�v�`�b�v�t���A�P�̂h�c�ԍ����擾
	int modelno = GetMapChipNo(trans.x, trans.z);

	// �}�b�v�`�b�v�`��
	if (modelno == 4) {

		//�t���X�^���J�����O
		XMFLOAT4X4 view, proj, out;
		view = CCamera::GetInstance()->GetCameraMatrix();
		proj = CCamera::GetInstance()->GetProjectionMatrix();
		DX11MtxMultiply(out, view, proj);

		if (IsInFrustum(XMFLOAT3(mtx._41, mtx._42, mtx._43), wallradius, out)) {
			modelmapchip->Draw(mtx);
		}


	}

}