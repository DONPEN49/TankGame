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
/// �R���X�g���N�^
/// </summary>
/// <param name="_mtx"> ���˂����I�u�W�F�N�g�̍s�� </param>
/// <param name="_type"> �N���甭�˂��ꂽ�� </param>
Bullet_Object::Bullet_Object(XMFLOAT4X4 _mtx, TYPEOBJ _type)
	: m_sts(Bullet_Object::STATUS::LIVE)
	, m_speed(SPEED)
	, m_life(100)
	, m_type(_type)
	, m_weight(0)
	, m_radius(5.0f)
{
	//���˂����I�u�W�F�N�g�̍s����Z�b�g
	SetMatrix(_mtx);
	//�ʒu�ݒ�
	SetPos(XMFLOAT3(m_mtx._41, m_mtx._42, m_mtx._43));
	//���a
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
	//���˂����I�u�W�F�N�g�̍s����Z�b�g
	//�ʒu�ݒ�
	SetPos(XMFLOAT3(m_mtx._41, m_mtx._42, m_mtx._43));
	//���a
	m_radius = BoundingSphere_Manager::GetInstance().GetBSRadius("assets/shot.fbx");
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Bullet_Object::~Bullet_Object() {
	/*Finalize();*/
}

bool Bullet_Object::Init() {
	

	return m_sts;
}

/// <summary>
/// �`��
/// </summary>
void Bullet_Object::Draw() {

	if (m_sts == STATUS::LIVE) {
		m_model->DrawModel(m_mtx);
		
	}
}

/// <summary>
/// �X�V
/// </summary>
void Bullet_Object::Update() {

	this->m_life--;

	if (this->m_life > 0) {
		XMFLOAT4 axisZ;

		// Z�������o��
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
/// �㏈��
/// </summary>
void Bullet_Object::Finalize() {
	this->m_model->Uninit();
	
}