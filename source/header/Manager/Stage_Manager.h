#pragma once

#include <vector>
#include <memory>
#include "Singleton.h"
#include "Stage.h"
#include "dx11mathutil.h"


using namespace DirectX;

class Stage_Manager : public Singleton<Stage_Manager> {
public:

	bool Init();
	void Update();
	void Draw();
	void DrawShadow();
	void DrawModel();
	void Exit();

	int GetMapChipNo(float x, float z) {

		int mapx = GetMapX(x);
		int mapz = GetMapZ(z);

		return(map[mapz][mapx]);

	}

	int GetMapX(float x) {
		return (int)floorf((x + MAPCHIPWIDTH / 2) / MAPCHIPWIDTH) + (uint16_t)(MAPSIZEX / 2);
	}

	int GetMapZ(float z) {
		return MAPSIZEZ - 1 - (int)floorf((z + MAPCHIPWIDTH / 2) / MAPCHIPWIDTH) - (uint16_t)(MAPSIZEX / 2);
	}

private:

	static int map[15][15];

	static constexpr float MAPCHIPWIDTH = 30.0f;
	static constexpr uint16_t MAPSIZEX = 15;
	static constexpr uint16_t MAPSIZEZ = 15;
	static constexpr int MODELMAX = 1;

	CModel* modelmapchip;

	//�ǂ̔��a
	float wallradius;

	const char* model_data[MODELMAX + 1] =
	{
		nullptr,
		"assets/dice/Wall.x"
	};

	void Chip3DDraw(int x, int z);



	/// <summary>
	/// �X�e�[�W�I�u�W�F�N�g����
	/// </summary>
	/// <param name="filename">�I�u�W�F�N�g�t�@�C����</param>
	/// <param name="_mtx"> ���[���h�s�� </param>
	/// <returns>�@�������ł�����</returns>
	bool Generate(std::string _filename, XMFLOAT4X4 _mtx);

	/// <summary>
	/// �X�e�[�W�I�u�W�F�N�g�i�[�p���X�g
	/// </summary>
	std::vector<std::unique_ptr<Stage_Object>> stagelist;

	friend class Singleton<Stage_Manager>;



};