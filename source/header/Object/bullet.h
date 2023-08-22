#pragma once
#include "Object.h"


class CModel;
class BoundingSphere;

class Bullet_Object : public Object {
public:

	/// <summary>
	/// �d��
	/// </summary>
	static constexpr float GRAVITY = 9.8f;

	/// <summary>
	/// ��ԗ�
	/// </summary>
	enum STATUS {
		LIVE = 0,
		DEAD
	};

	/// <summary>
	/// �N�̒e��
	/// </summary>
	enum TYPEOBJ {
		PLAYER,
		ENEMY
	};

	Bullet_Object(XMFLOAT4X4 _mtx, TYPEOBJ _type);
	Bullet_Object();
	~Bullet_Object();

	bool Init();

	void Update();

	void Draw();
	void DrawModel() {};
	void DrawShadow() {};

	void Finalize();

	void SetModel(CModel* _model) { m_model = _model; }
	const STATUS GetStatus() const { return m_sts; }
	void SetStatus(const STATUS& _sts) { m_sts = _sts; }
	const TYPEOBJ GetType() const { return m_type; };


private:

	/// <summary>
	/// �e�̏��
	/// </summary>
	STATUS m_sts;

	/// <summary>
	/// �N���ł����e��
	/// </summary>
	TYPEOBJ m_type;

	/// <summary>
	/// ���C�t
	/// </summary>
	uint32_t m_life;

	/// <summary>
	/// ���x
	/// </summary>
	float m_speed;

	/// <summary>
	/// ���f��
	/// </summary>
	CModel* m_model;
	BoundingSphere* m_sphere;

	/// <summary>
	/// �p�x
	/// </summary>
	float m_angle;

	/// <summary>
	/// ����
	/// </summary>
	float m_weight;

	/// <summary>
	/// �T�C�Y
	/// </summary>
	float m_radius;
};