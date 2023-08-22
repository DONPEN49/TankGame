#pragma once
#include <vector>
#include "CModel.h"
#include "Object.h"

class Stage_Object : public Object{

public:
	bool Init() { return false; };
	bool Init(std::string _modelname);
	void Update() {};
	void Draw();
	void DrawShadow();
	void DrawModel();
	void Finalize();

private:
	CModel* m_pModel;
	void SetModel(CModel* _model) { m_pModel = _model; }
};