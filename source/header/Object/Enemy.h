#pragma once
#include <random>
#include "Object.h"
#include "BoundingSphere_Manager.h"

class Tank;
class IScene;
class Exp_Billboard;

class Enemy : public Object{

public:

	static constexpr float RAYSIZE = 30.0f;
	static constexpr uint32_t RAYLENGTH = 20;
	static constexpr uint32_t RAYJUDGE = 2;

	enum class STATE {
		CHASE,
		FREE,
		CYCLE,
		DESTROY,
		DEAD,
	};

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	Enemy();
	Enemy(IScene*);

	bool Init();
	void Update();
	void Draw();
	void DrawModel();
	void DrawShadow();

	/// <summary>
	/// �G�X�V
	/// </summary>
	void UpdateEnemy(XMFLOAT4X4 _playermtx);

	//���getter,setter
	void SetState(STATE _state) { m_state = _state; };
	const STATE GetState() const { return m_state; };

	void SetRand(std::mt19937 _mt) { m_mt2 = _mt; };

	//���C�t�擾
	const uint16_t& GetLife() const { return m_life; };
	uint16_t		m_life;			//���C�t
private:

	/// <summary>
	/// �GAI
	/// </summary>
	void AutoPirot();

	/// <summary>
	/// �G�U��
	/// </summary>
	void AutoAttack();

	std::mt19937	m_mt2;

	uint32_t		m_keepTime;

	STATE			m_state;			//���
	uint32_t		m_atkInterval;		//�U���Ԋu		
		
	Tank*			m_pTank;			//��Ԗ{��

	Exp_Billboard*	m_pExp;
};
