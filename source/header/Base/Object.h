#pragma once
#include <DirectXMath.h>
#include <string>

using namespace DirectX;
class IScene;

class Object
{
public:

	enum RENDER {
		DEFAULT,		
		DEFAULTSHADOW,	//影だけつける
		NOSHADOW,		//影をつけない
		WITHSHADOW		//影をつける（モデルにも反映）
	};

	Object();
	Object(IScene*&);
	virtual ~Object() {};

	virtual bool Init() = 0;
	
	virtual void Update() = 0;

	virtual void Draw() = 0;

	virtual void DrawModel() = 0;

	virtual void DrawShadow() = 0;

	//getter,setter
	const XMFLOAT3& GetPostion() const { return m_pos; }
	void SetPos(const XMFLOAT3& _pos) { m_pos = _pos; }
	const XMFLOAT4X4& GetMatrix() const { return m_mtx; }
	void SetMatrix(const XMFLOAT4X4& _mtx) { m_mtx = _mtx; }
	
	//描画するか
	bool m_isDraw;

	//描画形態
	RENDER m_render;

	//タグ
	std::string m_tag;

	//マネージャー
	IScene* m_pScene;

protected:

	/// <summary>
	/// ワールド行列
	/// </summary>
	XMFLOAT4X4 m_mtx;

	/// <summary>
	/// 位置
	/// </summary>
	XMFLOAT3 m_pos;
};



