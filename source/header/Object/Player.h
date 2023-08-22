#pragma once

#include "Object.h"
#include "Smoke_Billboard.h"
#include "tank.h"
#include <vector>

class Player : public Object {
public:

	enum STATUS {
		WAIT = 0,
		LIVE,
		DEAD,
	};

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	Player();
	Player(class IScene&);
	~Player();

	/// <summary>
	/// ������
	/// </summary>
	bool Init();

	/// <summary>
	/// �v���C���[�X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();
	void DrawModel();
	void DrawShadow();

	/// <summary>
	/// ��Ԑݒ�
	/// </summary>
	void SetStatus(STATUS _status) { status = _status; };

	/// <summary>
	/// ��Ԏ擾
	/// </summary>
	const STATUS GetStatus() const { return status; };

	/// <summary>
	/// ���C�t�擾
	/// </summary>
	const uint32_t GetLife()const { return life; };

	/// <summary>
	/// �p�C�v�擾
	/// </summary>
	const XMFLOAT4X4 GetPlayerPipe()const { return tank->GetPipeMtx(); };

private:

	STATUS status;

	uint32_t life;

	Tank* tank;

	Smoke_Billboard* smoke;

	std::vector<std::unique_ptr<Smoke_Billboard>> m_smokeList;

	void SmokeGenerate();

};