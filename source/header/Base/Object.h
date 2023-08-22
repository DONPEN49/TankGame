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
		DEFAULTSHADOW,	//�e��������
		NOSHADOW,		//�e�����Ȃ�
		WITHSHADOW		//�e������i���f���ɂ����f�j
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
	
	//�`�悷�邩
	bool m_isDraw;

	//�`��`��
	RENDER m_render;

	//�^�O
	std::string m_tag;

	//�}�l�[�W���[
	IScene* m_pScene;

protected:

	/// <summary>
	/// ���[���h�s��
	/// </summary>
	XMFLOAT4X4 m_mtx;

	/// <summary>
	/// �ʒu
	/// </summary>
	XMFLOAT3 m_pos;
};



