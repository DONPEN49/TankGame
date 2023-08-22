#pragma once
#include <unordered_map>
#include "BoundingSphere.h"
#include "Singleton.h"
#include "ModelMgr.h"



class BoundingSphere_Manager : public Singleton<BoundingSphere_Manager> {

private:

	friend class Singleton<BoundingSphere_Manager>;

	std::unordered_map<
		std::string,							// �L�[
		std::unique_ptr<BoundingSphere>			// �f�[�^
	> bshashmap;

public:

	/// <summary>
	/// BS��Đ���
	/// </summary>
	void AllBSCreate();

	/// <summary>
	/// BS�𐶐�
	/// </summary>
	/// <param name="_texturefilename">�t�@�C����</param>
	bool CreateBoundingSphere(std::string _filename) {

		std::unique_ptr<BoundingSphere> bs;

		bs = std::make_unique<BoundingSphere>();

		//���f���̒��_�𔲂��o��
		CModel* bsmodel;
		bsmodel = ModelMgr::GetInstance().GetModelPtr(_filename);
		if (bsmodel == nullptr) {
			char str[128];
			sprintf_s<128>(str, "%s", _filename.c_str());
			MessageBox(nullptr, str, "bs create error", MB_OK);
			assert(false);
			return false;
		}

		const ModelData& md = bsmodel->GetModelData();
		const std::vector<Mesh>& meshes = md.GetMeshes();

		std::vector<XMFLOAT3> vertices;

		for (auto m : meshes) {
			for (auto v : m.m_vertices) {
				vertices.emplace_back(v.m_Pos);
			}
		}

		//BS������
		bs->Init(vertices, XMFLOAT4(1, 1, 1, 0.3f));

		//�R���e�i�i�[
		bshashmap[_filename].swap(bs);

		return true;
	}

	/// <summary>
	/// BS�擾
	/// </summary>
	/// <param name="_key">�t�@�C����</param>
	BoundingSphere* GetBoundingSphere(std::string _key) {
		// ���݂��邩���m���߂�
		auto it = bshashmap.find(_key);
		if (it == bshashmap.end()) {

			char str[128];
			sprintf_s<128>(str, "%s", _key.c_str());
			MessageBox(nullptr, str, "Get BS error", MB_OK);
			assert(false);
			return nullptr;

		}
		return bshashmap[_key].get();
	}

	/// <summary>
	/// ���a�擾
	/// </summary>
	/// <param name="_key">�t�@�C����</param>
	/// <returns>���a</returns>
	float GetBSRadius(std::string _key) {
		auto it = bshashmap.find(_key);
		if (it == bshashmap.end()) {
			return 0;
		}
		return bshashmap[_key].get()->GetR();
	}

};
