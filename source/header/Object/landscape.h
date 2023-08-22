#pragma once
#include "Object.h"

class CModel;
class IScene;

class LandScape : public Object {

public:

	LandScape() {};
	LandScape(IScene&);
	~LandScape();

	bool Init();

	void Update() {};

	void Draw() {};

	void DrawModel();

	void DrawShadow();


	void SetModel(CModel* _model) { m_pModel = _model; }

private:
	CModel* m_pModel;
};
