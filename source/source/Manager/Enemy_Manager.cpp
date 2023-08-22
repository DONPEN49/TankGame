#include <random>
#include <stdio.h>
#include "Enemy_Manager.h"
#include "CCamera.h"
#include "Player.h"
#include "rule_AI.h"
#include "Bullet_Manager.h"
#include "obb.h"
#include "IScene.h"

//�����p�ϐ�
std::random_device rnddev;
std::mt19937 rnd;
std::uniform_real_distribution<float> distx(-150, 150);
std::uniform_real_distribution<float> distz(-150, 150);

Enemy_Manager::Enemy_Manager() {

}

Enemy_Manager::Enemy_Manager(IScene* _scene)
	:m_scene(_scene)
{
	
}


/// <summary>
/// ������
/// </summary>
/// <returns>�������ł�����</returns>
bool Enemy_Manager::Init(IScene* _scene) {

	m_scene = _scene;

	//�v�f�폜
	enemylist.clear();

	//�G����
	for (int i = 0; i < ENEMY_MAX; i++) {
		bool sts = Generate();
		if (!sts) {
			char str[128];
			sprintf_s<128>(str, "%s", "enemy");
			MessageBox(nullptr, str, "Generate error", MB_OK);
			assert(false);
			return false;
		}
	}

	//���a�ݒ�
	EnemyRadius = BoundingSphere_Manager::GetInstance().GetBSRadius("assets/tank/tank10_base.x");

	return true;
}

/// <summary>
///�@�G����
/// </summary>
bool Enemy_Manager::Generate() {
	//�V�[�h�l���Z�b�g
	rnd.seed(rnddev());

	Enemy* e;

	e = new Enemy(m_scene);
	bool sts = e->Init();
	if (!sts) {
		char str[128];
		sprintf_s<128>(str, "%s", "enemy");
		MessageBox(nullptr, str, "init error", MB_OK);
		assert(false);
	}

	e->SetRand(rnd);
	e->SetPos(DirectX::XMFLOAT3(distx(rnd), 0.0f, distz(rnd)));

	enemylist.emplace_back(e);

	return sts;
}

/// <summary>
/// �X�V
/// </summary>
void Enemy_Manager::Update(Player* _player) {

	float lng;		//�e�Ƃ̋���

	//�e�̔��a
	float bulrad = Bullet_Manager::GetInstance().GetBulRadius();

	//�e�̔ԍ�
	int bullet_no = 0;

	//�t���X�^���J�����O�p�s��
	XMFLOAT4X4 view, proj, out;
	view = CCamera::GetInstance()->GetCameraMatrix();
	proj = CCamera::GetInstance()->GetProjectionMatrix();
	DX11MtxMultiply(out, view, proj);

	//��čX�V
	for (auto& e : enemylist) {
		
		//�j�󉉏o���ȊO
		if (e->GetState() != Enemy::STATE::DESTROY) {

			//AI�ݒ�
			ruleinfo rule = ruleselector(e, _player);
			rule.ruleactionfunc(e, _player);

			//�����蔻��
			for (auto b : Bullet_Manager::GetInstance().GetBulPosPlayer()) {

				DX11p2pLength(e->GetPostion(), XMFLOAT3(b.x, b.y, b.z), lng);

				//�q�b�g
				if (lng < bulrad + EnemyRadius) {

					//�e�폜
					Bullet_Manager::GetInstance().DeleteList(bullet_no, Bullet_Object::TYPEOBJ::PLAYER);
					e->m_life--;
					if (e->m_life <= 0) {
						Explosion(e->GetPostion());
					}
				}
				bullet_no++;
			}
		}
		e->UpdateEnemy(_player->GetMatrix());

		if (e->GetState() != Enemy::STATE::DEAD) {

			
			e->m_isDraw = IsInFrustum(e->GetPostion(), out);
		}

		bullet_no = 0;
	}

	auto ene = enemylist.begin();
	//����ł����珜�O����
	while (ene != enemylist.end()) {
		if ((*ene)->GetState() == Enemy::STATE::DEAD) {
			ene = enemylist.erase(ene);
		}
		else ++ene;
	}

	auto exp = explist.begin();
	
	while (exp != explist.end()) {
		if (!(*exp)->m_isDraw) {
			exp = explist.erase(exp);
		}
		else ++exp;
	}

}

/// <summary>
/// �`��
/// </summary>
void Enemy_Manager::Draw() {

	for (auto& exp : explist)
		exp->Draw();

	for (auto& e : enemylist)
		e->Draw();

}

/// <summary>
/// �`��
/// </summary>
void Enemy_Manager::DrawModel() {

	for (auto& exp : explist)
		exp->Draw();

	for (auto& e : enemylist) {

		e->DrawModel();
	}


}

/// <summary>
/// �`��
/// </summary>
void Enemy_Manager::DrawShadow() {

	for (auto& exp : explist)
		exp->Draw();

	for (auto& e : enemylist) {

		e->DrawShadow();
	}
		

}

/// <summary>
/// �GOBB���X�g�擾
/// </summary>
/// <returns>�GOBB���X�g</returns>
std::vector<COBB::OBBinfo> Enemy_Manager::GetEnemyOBB() {

	std::vector<COBB::OBBinfo> obblist;

	/*for (auto& e : enemylist) {
		obblist.emplace_back(e->GetOBB());
	}*/

	return obblist;
}

/// <summary>
/// �G�̈ʒu�擾
/// </summary>
std::vector<DirectX::XMFLOAT3> Enemy_Manager::GetEnemyPoses() {

	std::vector<DirectX::XMFLOAT3> templist;

	for (auto& e : enemylist)
		templist.emplace_back(e->GetPostion());


	return templist;
}

void Enemy_Manager::Finalize() {
	auto it = enemylist.begin();
	while (it != enemylist.end()) {

		it = enemylist.erase(it);
	}

	enemylist.clear();
}

void Enemy_Manager::Explosion(XMFLOAT3 _enepos) {

	std::unique_ptr<Exp_Billboard> exp;

	exp = std::make_unique<Exp_Billboard>();

	exp->Init();
	exp->SetPos(_enepos);

	explist.emplace_back(move(exp));
}