#include "rule_AI.h"
#include "tank.h"
#include "Stage_Manager.h"

using namespace DirectX;


ruleinfo ruleselector(Enemy* e, Player* p) {

	std::vector<ruleinfo> ignitionlist;

	int index = 0;
	// ���[���̏����`�F�b�N
	for (auto rule : rulelist) {
		bool sts = rule.rulefunc(e, p);
		if (sts) {
			// ���΂������[���̃C���f�b�N�X�ԍ���ۑ�
			ignitionlist.emplace_back(rule);
		}
		index++;
	}

	// ���΂������[����D�揇�ʁi�����j�Ń\�[�g
	std::sort(ignitionlist.begin(), ignitionlist.end());

	// �擪�̃��[����ԋp
	return ignitionlist[0];

}

/// <summary>
/// �͈͊O��
/// </summary>
bool Checkrule1(Enemy* e, Player* p) {

	//XMFLOAT3 pos = e->GetPostion();

	////float lng;

	////DX11Vec3Length(pos, lng);

	//int no = Stage_Manager::GetInstance().GetMapChipNo(pos.x, pos.z);

	////�͈͊O�Ȃ�true
	///*if (lng > Enemy::RANGE_STAGE) {
	//	return true;
	//}*/

	//if (no == 9 || no == 4) {
	//	return true;
	//}
	return false;

}


/// <summary>
/// ���΂�����
/// </summary>
void rule1action(Enemy* e, Player* p) {

	e->SetState(Enemy::STATE::CYCLE);

}

/// <summary>
/// �͈͓�
/// </summary>
bool Checkrule2(Enemy* e, Player* p) {

	//XMFLOAT3 pos = e->GetPostion();

	////float lng;

	////DX11Vec3Length(pos, lng);

	//int no = Stage_Manager::GetInstance().GetMapChipNo(pos.x, pos.z);

	//////�͈͊O�Ȃ�true
	////if (lng < Enemy::RANGE_STAGE) {
	////	return true;
	////}

	//if (no != 9) {
	//	return true;
	//}
	return false;
}


/// <summary>
/// �t���[
/// </summary>
void rule2action(Enemy* e, Player* p) {

	e->SetState(Enemy::STATE::FREE);

}

bool Checkrule3(Enemy* e, Player* p) {

	//XMFLOAT4X4 enemtx = e->GetMatrix();
	//XMFLOAT3 enepos = e->GetPostion();

	//XMFLOAT3 playerpos = p->GetPostion();

	//XMFLOAT3 axisz = { -enemtx._31,-enemtx._32 ,-enemtx._33 };

	//float eneradius = e->m_radius;

	//float lng;

	////���^�̃��C���΂�
	////�߂�����Ɣ��肵�Ȃ�
	//for (int i = Enemy::RAYJUDGE; i < Enemy::RAYLENGTH; i++)
	//{
	//	//���C�̈ʒu���v�Z
	//	XMFLOAT3 raypos = { enepos.x + axisz.x * Enemy::RAYSIZE * i, enepos.y + axisz.y * Enemy::RAYSIZE * i , enepos.z + axisz.z * Enemy::RAYSIZE * i };

	//	DX11p2pLength(playerpos, raypos, lng);

	//	//�v���C���[����
	//	if (lng < Enemy::RAYSIZE + eneradius) {
	//		return true;
	//	}
	//}

	return false;
}

void rule3action(Enemy* e, Player* p) {
	e->SetState(Enemy::STATE::CHASE);
}
