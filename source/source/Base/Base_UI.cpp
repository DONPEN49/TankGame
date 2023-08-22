#include "Base_UI.h"
#include "IScene.h"

UI::UI()
	:m_tag("no name")
{
}

UI::UI(IScene& _manager)
	:m_pScene(&_manager)
	,m_tag("no name")
{
	m_pScene->AddSceneUI(this);
}
