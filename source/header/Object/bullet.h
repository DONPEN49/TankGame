#pragma once
#include "Object.h"


class CModel;
class BoundingSphere;

class Bullet_Object : public Object {
public:

	/// <summary>
	/// 重力
	/// </summary>
	static constexpr float GRAVITY = 9.8f;

	/// <summary>
	/// 状態列挙
	/// </summary>
	enum STATUS {
		LIVE = 0,
		DEAD
	};

	/// <summary>
	/// 誰の弾か
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
	/// 弾の状態
	/// </summary>
	STATUS m_sts;

	/// <summary>
	/// 誰が打った弾か
	/// </summary>
	TYPEOBJ m_type;

	/// <summary>
	/// ライフ
	/// </summary>
	uint32_t m_life;

	/// <summary>
	/// 速度
	/// </summary>
	float m_speed;

	/// <summary>
	/// モデル
	/// </summary>
	CModel* m_model;
	BoundingSphere* m_sphere;

	/// <summary>
	/// 角度
	/// </summary>
	float m_angle;

	/// <summary>
	/// 質量
	/// </summary>
	float m_weight;

	/// <summary>
	/// サイズ
	/// </summary>
	float m_radius;
};