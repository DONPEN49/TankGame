#pragma once
#include	<memory>
#include	<wrl/client.h>
#include	<unordered_map>
#include	"CModel.h"

using Microsoft::WRL::ComPtr;



class ModelMgr {
private:
	ModelMgr(){
	}

	std::unordered_map<
		std::string,					// �L�[
		std::unique_ptr<CModel>			// �f�[�^
	> m_modelhashmap;

public:
	ModelMgr(const ModelMgr&) = delete;
	ModelMgr& operator=(const ModelMgr&) = delete;
	ModelMgr(ModelMgr&&) = delete;
	ModelMgr& operator=(ModelMgr&&) = delete;

	static ModelMgr& GetInstance() {
		static ModelMgr Instance;
		return Instance;
	}

	/// <summary>
	/// ���f����ēǂݍ���
	/// </summary>
	void AllModelLoad();

	virtual ~ModelMgr() {
		Finalize();
	}

	// ���f���̌�n��
	void Finalize() {
		for (auto itr = m_modelhashmap.begin(); 
			itr != m_modelhashmap.end(); 
			++itr) {

			(itr->second)->Uninit();

		}
	}

	// ���f�������[�h����
	bool LoadModel(std::string modelfilename, 
		std::string vsfilename,
		std::string psfilename,
		std::string texfilefolder) {

		std::unique_ptr<CModel>	p;

		p = std::make_unique<CModel>();

		// ���f���ǂݍ���
		bool sts = p->Init(
			modelfilename.c_str(), 
			vsfilename.c_str(), 
			psfilename.c_str(), 
			texfilefolder.c_str());
		if (!sts) {
			return false;
		}

		// unordered_map�R���e�i�Ɋi�[
		m_modelhashmap[modelfilename].swap(p);

		return true;
	}

	// ���f���̃|�C���^���擾����
	CModel* GetModelPtr(std::string key) {
		// ���݂��邩���m���߂�
		auto it = m_modelhashmap.find(key);
		if (it == m_modelhashmap.end()) {
			char str[128];
			sprintf_s<128>(str, "%s", key.c_str());
			MessageBox(nullptr, str, "Get Model error", MB_OK);
			assert(false);
			return nullptr;
		}
		return m_modelhashmap[key].get();
	}

};

