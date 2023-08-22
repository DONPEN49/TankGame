#pragma once
#include <vector>
#include <functional>
#include <algorithm>
#include "Enemy.h"
#include "Player.h"




struct ruleinfo {
	uint32_t	priority;									// �D��x
	std::function<bool(Enemy*, Player*)> rulefunc;			// ���[������
	std::function<void(Enemy*, Player*)> ruleactionfunc;	// ���[�����s

	bool operator<(const ruleinfo& another) const {
		//�����o�ϐ��ł���num1�Ŕ�r�������ʂ�
		//���̍\���̂̔�r�Ƃ���(����)
		return priority < another.priority;
	}
};

ruleinfo ruleselector(Enemy* e, Player* p);

/// <summary>
/// �͈͊O
/// </summary>
bool Checkrule1(Enemy* e, Player* p);
void rule1action(Enemy*, Player*);

/// <summary>
/// �͈͓�
/// </summary>
bool Checkrule2(Enemy* e, Player* p);
void rule2action(Enemy*, Player*);

/// <summary>
/// �ǐ�
/// </summary>
bool Checkrule3(Enemy* e, Player* p);
void rule3action(Enemy*, Player*);


static std::vector<ruleinfo> rulelist = {
	{1,Checkrule1,rule1action},
	{3,Checkrule2,rule2action},
	{2,Checkrule3,rule3action},
};




