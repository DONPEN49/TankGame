#include "Enemy.h"

#include <random>
#include "tank.h"
#include "Game_Manager.h"
#include "Bullet_Manager.h"
#include "Stage_Manager.h"
#include "IScene.h"
#include "Player.h"
#include "Exp_Billboard.h"

/// <summary>
/// コンストラクタ
/// </summary>
Enemy::Enemy()
{
	m_life = 3;
	m_pTank->m_type = Tank::TYPE::ENEMY;
	m_pTank->m_speed = -0.4f;
	m_keepTime = 10;
	m_state = STATE::FREE;
	m_atkInterval = 10;
	m_render = RENDER::DEFAULTSHADOW;
}

Enemy::Enemy(IScene* _scene)
	:m_life(3)
	,m_state(STATE::FREE)
	,m_atkInterval(10)
	,m_keepTime(10)
{
	m_pScene = _scene;
	m_tag = "Enemy";
	m_pScene->AddSceneObject(this);
	m_render = RENDER::DEFAULTSHADOW;
}

/// <summary>
/// 初期化
/// </summary>
/// <returns></returns>
bool Enemy::Init() {

	m_pTank = new Tank(Tank::TYPE::ENEMY);
	m_pTank->Init();
	m_pTank->m_speed = -0.4f;
	return true;
}

/// <summary>
/// 更新
/// </summary>
void Enemy::Update() {

	auto player = m_pScene->GetSceneObject<Player>("Player");
	UpdateEnemy(player->GetMatrix());
}

/// <summary>
/// 描画
/// </summary>
void Enemy::Draw() {
	m_pTank->Draw();
}

void Enemy::DrawShadow() {
	m_pTank->DrawShadow();
}

void Enemy::DrawModel() {
	m_pTank->DrawModel();
}

/// <summary>
/// 敵AI
/// </summary>
void Enemy::AutoPirot() {

	////乱数生成
	//std::uniform_int_distribution<int32_t> rand4(0, static_cast<int>(Action::MAX_ACTION) - 1);
	//std::uniform_int_distribution<uint32_t> rand10(30, 50);


	//m_pTank->m_angle.x = m_pTank->m_angle.y = m_pTank->m_angle.z = 0.0f;
	//m_speed = 1.0f;

	
	//if (lng > RANGE_STAGE) {
	//	m_speed = 0.0f;
	//}

	//int no = Stage_Manager::GetInstance().GetMapChipNo(pos.x, pos.z);

	/*if (no == 9) {
		m_speed = 0.0f;
	}*/

	/*switch (action)
	{
	case Enemy::Action::ACT_LEFT:
		m_pTank->m_angle.y -= m_pTank->ROTATIONRATE;
		break;
	case Enemy::Action::ACT_RIGHT:
		m_pTank->m_angle.y += m_pTank->ROTATIONRATE;
		break;
	case Enemy::Action::ACT_STRAIGHT:

		break;
	default:
		break;
	}

	if (--m_keepTime <= 0 && m_state != Enemy::STATE::DESTROY) {
		action = static_cast<Action>(rand4(m_mt2));
		m_keepTime = rand10(m_mt2);
	}*/
}

/// <summary>
/// 攻撃
/// </summary>
void Enemy::AutoAttack() {

	std::uniform_int_distribution<uint32_t> rand10(10, 50);

	//弾発射
	if (--m_atkInterval <= 0) {
		
		XMFLOAT4X4 tempmtx = m_pTank->GetTankBarrelMtx();

		//戦車の姿勢が逆のため反対にする
		tempmtx._31 = -tempmtx._31;
		tempmtx._32 = -tempmtx._32;
		tempmtx._33 = -tempmtx._33;
		tempmtx._34 = -tempmtx._34;

	
		Bullet_Manager::GetInstance().Fire(tempmtx, Bullet_Object::TYPEOBJ::ENEMY);


		m_atkInterval = rand10(m_mt2);
	}

}

/// <summary>
/// 敵更新
/// </summary>
/// <param name="_playermtx">自機行列</param>
void Enemy::UpdateEnemy(XMFLOAT4X4 _playermtx) {

	if (m_life <= 0 && m_state != Enemy::STATE::DESTROY) {
		SetState(Enemy::STATE::DESTROY);
		m_pTank->PrepareDestroy();
	}

	float lng;
	m_pos = m_pTank->GetPostion();
	/*DX11Vec3Length(pos, lng);*/

	uint16_t bullet_no = 0;

	//当たり判定
	for (auto b : Bullet_Manager::GetInstance().GetBulPosPlayer()) {

		DX11p2pLength(m_pos, XMFLOAT3(b.x, b.y, b.z), lng);

		//ヒット
		if (lng < 100) {

			//弾削除
			Bullet_Manager::GetInstance().DeleteList(bullet_no, Bullet_Object::TYPEOBJ::PLAYER);
			m_life--;
			if (m_life <= 0) {
				
			}
		}
		bullet_no++;
	}
	switch (m_state)
	{
		//追跡と攻撃
	case Enemy::STATE::CHASE:
		m_pTank->UpdateChase(_playermtx);
		AutoAttack();
		break;
		//索敵
	case Enemy::STATE::FREE:
		AutoPirot();
		m_pTank->UpdateFree();
		break;
	case Enemy::STATE::CYCLE:
		m_pTank->UpdateCycle();
		break;
	case Enemy::STATE::DEAD:
		m_isDraw = false;
		break;
	case Enemy::STATE::DESTROY:
		bool sts = m_pTank->DestroyEffect();

		//破壊エフェクトが終わったら消す
		if (sts) {
			SetState(Enemy::STATE::DEAD);
		}
		break;
	}

	
}