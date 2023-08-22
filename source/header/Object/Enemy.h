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
	/// コンストラクタ
	/// </summary>
	Enemy();
	Enemy(IScene*);

	bool Init();
	void Update();
	void Draw();
	void DrawModel();
	void DrawShadow();

	/// <summary>
	/// 敵更新
	/// </summary>
	void UpdateEnemy(XMFLOAT4X4 _playermtx);

	//状態getter,setter
	void SetState(STATE _state) { m_state = _state; };
	const STATE GetState() const { return m_state; };

	void SetRand(std::mt19937 _mt) { m_mt2 = _mt; };

	//ライフ取得
	const uint16_t& GetLife() const { return m_life; };
	uint16_t		m_life;			//ライフ
private:

	/// <summary>
	/// 敵AI
	/// </summary>
	void AutoPirot();

	/// <summary>
	/// 敵攻撃
	/// </summary>
	void AutoAttack();

	std::mt19937	m_mt2;

	uint32_t		m_keepTime;

	STATE			m_state;			//状態
	uint32_t		m_atkInterval;		//攻撃間隔		
		
	Tank*			m_pTank;			//戦車本体

	Exp_Billboard*	m_pExp;
};
