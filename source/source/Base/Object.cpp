#include "Object.h"
#include "IScene.h"
#include "dx11mathutil.h"

Object::Object()
	: m_isDraw(true)
	, m_tag("no name")
{
	DX11MtxIdentity(m_mtx);
}

Object::Object(IScene*& _manager)
	: m_isDraw(true)
	, m_pScene(_manager)
	, m_tag("no name")
{
	DX11MtxIdentity(m_mtx);
	m_pScene->AddSceneObject(this);
}