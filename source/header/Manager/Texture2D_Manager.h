#pragma once
#include <unordered_map>
#include "quad2d.h"
#include "Singleton.h"

using namespace DirectX;


class Manager_Texture2D : public Singleton<Manager_Texture2D> {
private:

	std::unordered_map<
		std::string,						// �L�[
		std::unique_ptr<Quad2D>			// �f�[�^
	> quad2dhashmap;

	friend class Singleton<Manager_Texture2D>;

public:

	/// <summary>
	/// �e�N�X�`����ēǂݍ���
	/// </summary>
	void AllTexture2DLoad();

	/// <summary>
	/// �e�N�X�`�����[�h
	/// </summary>
	/// <param name="_texturefilename">�t�@�C����</param>
	bool LoadTexture2D(std::string _texturefilename) {

		std::unique_ptr<Quad2D> q;

		q = std::make_unique<Quad2D>();

		XMFLOAT2 uv[4] = {
			{0.0f,0.0f},
			{1.0f,0.0f},
			{0.0f,1.0f},
			{1.0f,1.0f}
		};

		//�Ƃ肠�������������Ƃ�
		bool sts = q->Init(0, 0, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), uv);
		if (!sts) {
			return false;
		}

		q->LoadTexture(_texturefilename.c_str());

		// unordered_map�R���e�i�Ɋi�[
		quad2dhashmap[_texturefilename].swap(q);
	}

	/// <summary>
	/// �ǂݍ���ł����e�N�X�`���f�[�^�擾
	/// </summary>
	/// <param name="_key">�t�@�C����</param>
	Quad2D* GetTexture2DPtr(std::string _key) {
		// ���݂��邩���m���߂�
		auto it = quad2dhashmap.find(_key);
		if (it == quad2dhashmap.end()) {
			char str[128];
			sprintf_s<128>(str, "%s", _key.c_str());
			MessageBox(nullptr, str, "Get Texture error", MB_OK);
			assert(false);
			return nullptr;
		}
		return quad2dhashmap[_key].get();
	}

	/// <summary>
	/// �㏈��
	/// </summary>
	void Exit() {
		for (auto itr = quad2dhashmap.begin(); itr != quad2dhashmap.end(); ++itr) {
			(itr->second)->UnInit();
		}
	}
};