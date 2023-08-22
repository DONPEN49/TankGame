#pragma once


#include <vector>
#include "Enemy.h"
#include "Exp_Billboard.h"
#include "obb.h"
#include "Singleton.h"

class IScene;

class Enemy_Manager : public Singleton<Enemy_Manager> {

public:

	static constexpr uint16_t ENEMY_MAX = 3;

	Enemy_Manager();
	Enemy_Manager(IScene*);

	bool Init(IScene*);
	void Update(class Player*);
	void Draw();
	void DrawModel();
	void DrawShadow();
	void Finalize();

	/// <summary>
	/// OBB���X�g�擾
	/// </summary>
	std::vector<COBB::OBBinfo> GetEnemyOBB();

	/// <summary>
	/// �G�̈ʒu�擾
	/// </summary>
	std::vector<DirectX::XMFLOAT3> GetEnemyPoses();

private:

	friend class Singleton<Enemy_Manager>;

	/// <summary>
	/// �G����
	/// </summary>
	bool Generate();

	/// <summary>
	/// ���j�G�t�F�N�g����
	/// </summary>
	void Explosion(DirectX::XMFLOAT3);

	std::vector<Enemy*> enemylist;		//�G�̃��X�g
	std::vector<std::unique_ptr<Exp_Billboard>> explist;	//���j�e�N�X�`�����X�g

	float EnemyRadius;

	IScene* m_scene;

};