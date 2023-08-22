#pragma once
#include "IScene.h"

class Game_Scene : public IScene {

public:
	void Init();
	void Input();
	void Update();
	void Draw();
	void Dispose();


private:

	//�}�l�[�W���[
	class Enemy_Manager* m_enemyManager;

};