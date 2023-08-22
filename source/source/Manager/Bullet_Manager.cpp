#include "Bullet_Manager.h"
#include "bullet.h"
#include "ModelMgr.h"
#include "BoundingSphere_Manager.h"

void Bullet_Manager::Init() {

	//���a�ݒ�
	bulletradius = BoundingSphere_Manager::GetInstance().GetBSRadius("assets/shot.fbx");

}

/// <summary>
/// �e����
/// </summary>
/// <param name="_mtx">���˂����I�u�W�F�N�g�̍s��</param>
/// <param name="_type">�ǂ̃I�u�W�F�����˂�����</param> 
void Bullet_Manager::Fire(XMFLOAT4X4 _mtx, Bullet_Object::TYPEOBJ _type) {

	std::unique_ptr<Bullet_Object> b;

	b = std::make_unique<Bullet_Object>(_mtx, _type);
	b->Init();
	b->SetModel(
		ModelMgr::GetInstance().GetModelPtr("assets/shot.fbx")
	);

	if (_type == Bullet_Object::TYPEOBJ::PLAYER) {
		playerlist.emplace_back(std::move(b));
	}
	else {
		enemylist.emplace_back(std::move(b));
	}

}

void Bullet_Manager::Update() {

	for (auto& bul : playerlist)
		bul->Update();

	for (auto& bul : enemylist)
		bul->Update();

	// ����ł�e�폜
	auto it = playerlist.begin();
	while (it != playerlist.end()) {

		if ((*it)->GetStatus() == Bullet_Object::STATUS::DEAD) {
			it = playerlist.erase(it);

		}
		else ++it;
	}

	auto itr = enemylist.begin();
	while (itr != enemylist.end()) {

		if ((*itr)->GetStatus() == Bullet_Object::STATUS::DEAD) {
			itr = enemylist.erase(itr);

		}
		else ++itr;
	}
}

void Bullet_Manager::Draw() {

	/*for (auto& bul : playerlist)
		bul->Draw();*/

	/*for (auto& bul : enemylist)
		bul->Draw();*/
}

void Bullet_Manager::Exit() {
	auto it = playerlist.begin();
	while (it != playerlist.end()) {
		it = playerlist.erase(it);
	}

	auto itr = enemylist.begin();
	while (itr != enemylist.end()) {
		itr = enemylist.erase(itr);
	}
}

/// <summary>
/// �e�̈ʒu��Ԃ�
/// </summary>
std::vector<XMFLOAT3> Bullet_Manager::GetBulPosPlayer() {

	//���̃��X�g
	std::vector<XMFLOAT3> templist;

	for (auto& bul : playerlist) {
		templist.emplace_back(bul->GetPostion());
	}

	return templist;
}


/// <summary>
/// �e�̈ʒu��Ԃ�
/// </summary>
std::vector<XMFLOAT3> Bullet_Manager::GetBulPosEnemy() {

	//���̃��X�g
	std::vector<XMFLOAT3> templist;

	for (auto& bul : enemylist) {
		templist.emplace_back(bul->GetPostion());
	}

	return templist;
}

/// <summary>
/// �e�폜
/// </summary>
/// <param name="_no">NO</param>
/// <param name="_type">���</param>
void Bullet_Manager::DeleteList(int _no, Bullet_Object::TYPEOBJ _type) {

	if (_type == Bullet_Object::TYPEOBJ::PLAYER) {
		playerlist.erase(playerlist.begin() + _no);
	}
	else {
		enemylist.erase(enemylist.begin() + _no);
	}

}

