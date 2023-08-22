#include "bullet.h"
#include "Game_Manager.h"
#include "CModel.h"
#include "Sphere.h"
#include "DX11util.h"
#include "DX11Settransform.h"
#include "BoundingSphere_Manager.h"

using namespace DirectX;

constexpr float SPEED = 10.0f;

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="_mtx"> 発射したオブジェクトの行列 </param>
/// <param name="_type"> 誰から発射されたか </param>
Bullet_Object::Bullet_Object(XMFLOAT4X4 _mtx, TYPEOBJ _type)
	: m_sts(Bullet_Object::STATUS::LIVE)
	, m_speed(SPEED)
	, m_life(100)
	, m_type(_type)
	, m_weight(0)
	, m_radius(5.0f)
{
	//発射したオブジェクトの行列をセット
	SetMatrix(_mtx);
	//位置設定
	SetPos(XMFLOAT3(m_mtx._41, m_mtx._42, m_mtx._43));
	//半径
	m_radius = BoundingSphere_Manager::GetInstance().GetBSRadius("assets/shot.fbx");
}

Bullet_Object::Bullet_Object()
	: m_sts(Bullet_Object::STATUS::LIVE)
	, m_speed(SPEED)
	, m_life(100)
	, m_type(TYPEOBJ::PLAYER)
	, m_weight(0)
	, m_radius(5.0f)
{
	//発射したオブジェクトの行列をセット
	//位置設定
	SetPos(XMFLOAT3(m_mtx._41, m_mtx._42, m_mtx._43));
	//半径
	m_radius = BoundingSphere_Manager::GetInstance().GetBSRadius("assets/shot.fbx");
}

/// <summary>
/// デストラクタ
/// </summary>
Bullet_Object::~Bullet_Object() {
	/*Finalize();*/
}

bool Bullet_Object::Init() {
	

	return m_sts;
}

/// <summary>
/// 描画
/// </summary>
void Bullet_Object::Draw() {

	if (m_sts == STATUS::LIVE) {
		m_model->DrawModel(m_mtx);
		
	}
}

/// <summary>
/// 更新
/// </summary>
void Bullet_Object::Update() {

	this->m_life--;

	if (this->m_life > 0) {
		XMFLOAT4 axisZ;

		// Z軸を取り出す
		axisZ.x = this->m_mtx._31;
		axisZ.y = this->m_mtx._32;
		axisZ.z = this->m_mtx._33;
		axisZ.w = 0.0f;

		this->m_pos.x += axisZ.x*m_speed;
		this->m_pos.y += axisZ.y*m_speed;
		this->m_pos.z += axisZ.z*m_speed;

		this->m_mtx._41 = this->m_pos.x;
		this->m_mtx._42 = this->m_pos.y;
		this->m_mtx._43 = this->m_pos.z;
	}
	else {

 		this->m_sts = STATUS::DEAD;
	}

}

/// <summary>
/// 後処理
/// </summary>
void Bullet_Object::Finalize() {
	this->m_model->Uninit();
	
}