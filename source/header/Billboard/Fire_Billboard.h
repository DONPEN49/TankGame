#pragma once
#include "Base_Billboard.h"

class Fire_Billboard : public Billboard {

public:

	Fire_Billboard();
	Fire_Billboard(DirectX::XMFLOAT3 _pos);
	~Fire_Billboard() {};

	void Init();
	void Update();
	void Draw();

	bool m_isDraw;

private:

	static constexpr float ANIMESPEED = 1.2f;

	float m_idU;
	float m_idV;
};
