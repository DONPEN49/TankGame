#pragma once
#include <vector>
#include "IScene.h"
#include "Base_UI.h"
#include "Object.h"

class Title_Scene : public IScene {
private:

	DirectX::XMFLOAT4X4 mtx;					// �`��p�s��
	class Tank* tank;
public:
	~Title_Scene() {
		Dispose();
	}
	void Init();
	void Update();
	void Draw();

	void Dispose();
};