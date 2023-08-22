#pragma once
#include "Base_Billboard.h"

class Smoke_Billboard : public Billboard {

public:

	Smoke_Billboard();
	Smoke_Billboard(XMFLOAT3);
	~Smoke_Billboard() {};

	void Init();
	void Update();
	void Draw();

	bool m_isDraw;
private:

	static constexpr float ANIMESPEED = 0.1f;
	float		m_speed;
	XMFLOAT3	m_pos;
	float m_idU;
	float m_idV;
};