#include "Player.h"
#include "tank.h"
#include "CDirectInput.h"
#include "Bullet_Manager.h"
#include "IScene.h"
#include "dx11mathutil.h"
#include "Manager_Camera.h"

/// <summary>
/// コンストラクタ
/// </summary>
Player::Player() {
	status = LIVE;
	life = 5;
	m_tag = "Player";
}

Player::Player(IScene& _scene)
	:life(5)
	,status(LIVE)
{
	//基底クラスメンバ変数設定
	m_pScene = &_scene;
	m_tag = "Player";
	m_render = DEFAULTSHADOW;
	m_pScene->AddSceneObject(this);
}

/// <summary>
/// デストラクタ
/// </summary>
Player::~Player() {
	delete tank;
	delete smoke;
}


/// <summary>
/// 初期化
/// </summary>
bool Player::Init() {

    tank = new Tank(Tank::TYPE::PLAYER);
	smoke = new Smoke_Billboard(GetPostion());
	bool sts = tank->Init();
	smoke->Init();
	return sts;
}


void Player::Update() {

	if (status == LIVE) {
		tank->Update();
	}

	smoke->Update();

	//位置と行列を更新
	this->m_mtx = tank->GetMatrix();
	m_pos.x = m_mtx._41;
	m_pos.y = m_mtx._42;
	m_pos.z = m_mtx._43;

	// 砲台を動かす
	if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_SPACE)) {
		tank->Fire();
		Bullet_Manager::GetInstance().Fire(m_mtx,Bullet_Object::TYPEOBJ::PLAYER);
	}

	//弾のNo（削除用）
	int bulno = 0;

	//弾との当たり判定
	for (auto b : Bullet_Manager::GetInstance().GetBulPosEnemy()) {

		float lng;

		DX11p2pLength(XMFLOAT3(b.x, b.y, b.z), tank->GetPostion(), lng);

		if (lng < tank->m_radius + Bullet_Manager::GetInstance().GetBulRadius()) {

			//m_life--;
			Bullet_Manager::GetInstance().DeleteList(bulno, Bullet_Object::TYPEOBJ::ENEMY);
		}

		bulno++;
	}

	if (life <= 0) {
		status = DEAD;
		tank->m_isDraw = false;
	}

	
}

void Player::Draw() {
	tank->Draw();
	smoke->Draw();

	for (auto& sm : m_smokeList)
		sm->Draw();

}

void Player::DrawModel() {

	tank->DrawModel();

}

void Player::DrawShadow() {

	tank->DrawShadow();
}


void Player::SmokeGenerate() {

	std::unique_ptr<Smoke_Billboard> sm;

	sm = std::make_unique<Smoke_Billboard>(m_pos);

	sm->Init();

	m_smokeList.emplace_back(move(sm));

}