#include "RaderEne_Manager.h"
#include "Enemy_Manager.h"

/// <summary>
/// 初期化
/// </summary>
void RaderEne_Manager::Init() {

	for (int i = 0; i < Enemy_Manager::ENEMY_MAX; i++){
		Generate();
	}

}

/// <summary>
/// 生成
/// </summary>
void RaderEne_Manager::Generate() {

	std::unique_ptr<LaderEne_UI> e;

	e = std::make_unique<LaderEne_UI>();

	e->Init();

	enelist.emplace_back(move(e));

}

/// <summary>
/// 更新
/// </summary>
void RaderEne_Manager::Update(DirectX::XMFLOAT4X4 _playermtx) {

	////敵の位置取得
	//std::vector<XMFLOAT3> eneposes = Enemy_Manager::GetInstance().GetEnemyPoses();

	////万が一数が合わなかった場合減らす
	//for (int i = eneposes.size(); i < enelist.size(); i++){
	//	enelist.erase(enelist.begin());
	//}

	//for (int i = 0; i < eneposes.size(); i++){
	//	enelist.at(i)->SetEnePos(eneposes.at(i));
	//	enelist.at(i)->Update(_playermtx);
	//}
}

/// <summary>
/// 描画
/// </summary>
void RaderEne_Manager::Draw() {

	for (auto& e : enelist) {
		e->Draw();
	}
}