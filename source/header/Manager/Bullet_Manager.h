#pragma once
#include <DirectXMath.h>
#include <vector>
#include <memory>
#include "Singleton.h"
#include "bullet.h"

using namespace DirectX;

class Bullet_Manager : public  Singleton<Bullet_Manager> {

private:
	std::vector<std::unique_ptr<Bullet_Object>>	playerlist;
	std::vector<std::unique_ptr<Bullet_Object>>	enemylist;

	friend class Singleton<Bullet_Manager>;

	float bulletradius;

public:

	/// <summary>
	/// �e����
	/// </summary>
	/// <param name="mtx">���˂����I�u�W�F�N�g�̍s��</param>
	/// <param name="_type">���˂����I�u�W�F�N�g�̎��</param>
	void Fire(XMFLOAT4X4 mtx, Bullet_Object::TYPEOBJ _type);

	void Init();

	void Update();

	void Draw();

	void Exit();

	/// <summary>
	/// �e�̈ʒu�擾
	/// </summary>
	/// <returns></returns>
	std::vector<XMFLOAT3> GetBulPosPlayer();
	std::vector<XMFLOAT3> GetBulPosEnemy();

	/// <summary>
	/// �e�̔��a�擾
	/// </summary>
	const float GetBulRadius() const { return bulletradius; };

	/// <summary>
	/// �e���X�g����폜
	/// </summary>
	/// <param name="_no">�폜����e��NO</param>
	void DeleteList(int _no, Bullet_Object::TYPEOBJ _type);

};