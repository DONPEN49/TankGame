#pragma once
#include <string>
#include "quad2d.h"

using namespace DirectX;

class UI {
protected:

	Quad2D* m_quad2d;

	/// <summary>
	/// 幅
	/// </summary>
	float m_width;

	/// <summary>
	/// 高さ
	/// </summary>
	float m_height;

	/// <summary>
	/// 位置
	/// </summary>
	XMFLOAT3 m_pos;

	class IScene* m_pScene;

	/// <summary>
	/// 透明度
	/// </summary>
	float m_alpha = 1.0f;

public:

	static constexpr uint32_t FULLWIDTH = 36;
	static constexpr uint32_t FULLHEIGHT = 30;

	BLENDSTATE m_blend;

	/// <summary>
	/// タグ
	/// </summary>
	std::string m_tag;


	/// <summary>
	/// コンストラクタ
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
	/// 位置設定
	/// </summary>
	/// <param name="_pos">位置</param>
	void SetPos(const XMFLOAT3 _pos) {
		m_quad2d->SetPosition(_pos.x, _pos.y, _pos.z);
		m_pos = _pos;
	}

	/// <summary>
	/// サイズ設定
	/// </summary>
	/// <param name="_width">幅</param>
	/// <param name="_height">高さ</param>
	void SetScale(const float _width, const float _height) {
		m_quad2d->SetScale(_width, _height, 1.0f);
		m_width = _width;
		m_height = _height;
	}

	/// <summary>
	/// データ設定
	/// </summary>
	void SetQuad2D(Quad2D* _quad2d) {
		m_quad2d = _quad2d;
	}

	/// <summary>
	/// 頂点データ更新
	/// </summary>
	/// <remarks> 特に変更がない場合のみ呼び出す </remarks>
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
	/// アルファ値セット
	/// </summary>
	void SetAlpha(float _alphe) { m_alpha = _alphe; };

};