#pragma once
#include <string>
#include "quad2d.h"

using namespace DirectX;

class UI {
protected:

	Quad2D* m_quad2d;

	/// <summary>
	/// ��
	/// </summary>
	float m_width;

	/// <summary>
	/// ����
	/// </summary>
	float m_height;

	/// <summary>
	/// �ʒu
	/// </summary>
	XMFLOAT3 m_pos;

	class IScene* m_pScene;

	/// <summary>
	/// �����x
	/// </summary>
	float m_alpha = 1.0f;

public:

	static constexpr uint32_t FULLWIDTH = 36;
	static constexpr uint32_t FULLHEIGHT = 30;

	BLENDSTATE m_blend;

	/// <summary>
	/// �^�O
	/// </summary>
	std::string m_tag;


	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	UI();
	UI(IScene&);

	virtual bool Init() { return true; };
	virtual void Update() {};
	virtual void Draw() {
		TurnOffZbuffer();

		UpdateQuad2D();

		m_quad2d->Draw();

		TurnOnZbuffer();
	};

	virtual void DrawAdd() {
		TurnOffZbuffer();

		UpdateQuad2D();

		m_quad2d->DrawAdd();

		TurnOnZbuffer();
	};

	virtual void Finalize() {};

	/// <summary>
	/// �ʒu�ݒ�
	/// </summary>
	/// <param name="_pos">�ʒu</param>
	void SetPos(const XMFLOAT3 _pos) {
		m_quad2d->SetPosition(_pos.x, _pos.y, _pos.z);
		m_pos = _pos;
	}

	/// <summary>
	/// �T�C�Y�ݒ�
	/// </summary>
	/// <param name="_width">��</param>
	/// <param name="_height">����</param>
	void SetScale(const float _width, const float _height) {
		m_quad2d->SetScale(_width, _height, 1.0f);
		m_width = _width;
		m_height = _height;
	}

	/// <summary>
	/// �f�[�^�ݒ�
	/// </summary>
	void SetQuad2D(Quad2D* _quad2d) {
		m_quad2d = _quad2d;
	}

	/// <summary>
	/// ���_�f�[�^�X�V
	/// </summary>
	/// <remarks> ���ɕύX���Ȃ��ꍇ�̂݌Ăяo�� </remarks>
	void UpdateQuad2D() {

		XMFLOAT2 uv[4] = {
			{0.0f,0.0f},
			{1.0f,0.0f},
			{0.0f,1.0f},
			{1.0f,1.0f}
		};
		m_quad2d->SetPosition(m_pos.x, m_pos.y, m_pos.z);
		m_quad2d->updateVertex(static_cast<uint32_t>(m_width), static_cast<uint32_t>(m_height), XMFLOAT4(1.0f, 1.0f, 1.0f, m_alpha), uv);
		m_quad2d->updateVbuffer();
	}
	
	/// <summary>
	/// �A���t�@�l�Z�b�g
	/// </summary>
	void SetAlpha(float _alphe) { m_alpha = _alphe; };

};