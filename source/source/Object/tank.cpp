#include	<random>
#include    <vector>
#include    <list>
#include    "Game_Manager.h"
#include	"tank.h"
#include	"drawaxis.h"
#include	"dx11mathutil.h"
#include	"ModelMgr.h"
#include	"CDirectInput.h"
#include    "Bullet_Manager.h"
#include    "Manager_Camera.h"
#include    "BoundingSphere_Manager.h"
#include    "Stage_Manager.h"
#include    "StageHit.h"
#include	"Fire_Billboard.h"
#include	"IScene.h"

// 戦車の親子関係及び初期位置・初期角度を定義したデータ
Tank::TankInitData				Tank::ObjectInitData[] = {
	//   親								モデル番号				初期位置					初期角度		// 注意：かならず子供より親が先にならぶこと
	{ Tank::NONE,				Tank::TANK_CATERPILLAR,			{ 0, 0, 0 },				{ 0, 0, 0 } },	// 戦車 キャタピラー
	{ Tank::TANK_CATERPILLAR,	Tank::TANK_BASE,				{ 0, 0, 0 },				{ 0, 0, 0 } },	// 戦車 ベース
	{ Tank::TANK_BASE,			Tank::TANK_TOP,					{ 0, 3.82394f, -1.15362f }, { 0, 0, 0 } },	// 戦車 砲台
	{ Tank::TANK_TOP,			Tank::TANK_BARREL,				{ 0, 0.684f, -3.53283f },	{ 0, 0, 0 } },	// 戦車 砲身
	{ Tank::END,				Tank::END,						{ 0, 0, 0 },				{ 0, 0, 0 } },	// 終端コード
};

// モデルファイル名リスト
Tank::TankModelData				Tank::XFileData[] = {
	{ Tank::TANK_CATERPILLAR,	"assets/tank/tank10_cat" },				// 戦車のキャタピラー
	{ Tank::TANK_BASE,			"assets/tank/tank10_base" },			// 戦車の土台
	{ Tank::TANK_TOP,			"assets/tank/tank10_top" },				// 戦車の砲台
	{ Tank::TANK_BARREL,		"assets/tank/tank10_pipe" },			// 戦車の砲身
	{ Tank::END,				nullptr }								// 終端コード
};

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="_game">ゲーム</param>
Tank::Tank()
	: m_speed(0.0f)
{

}

Tank::Tank(TYPE _type)
	: m_speed(0.0f)
	, m_type(_type)
{

}

/// <summary>
/// コンストラクタ
/// </summary>
/// <remarks>タイトル時のみ使用</remarks>
Tank::Tank(IScene* _scene)
	: m_speed(0.0f)
	, m_type(TYPE::PLAYER)
{
	m_tag = "tank";
	m_render = RENDER::DEFAULTSHADOW;
	m_isDraw = true;
	m_pScene = _scene;
	m_pScene->AddSceneObject(this);
}


/// <summary>
/// 初期化
/// </summary>
/// <returns></returns>
bool Tank::Init() {

	std::string x = { ".x" };
	// 行列初期化
	DX11MtxIdentity(m_mtx);					// モデルを読み込む
	// モデルポインターをセット
	if (m_type == TYPE::PLAYER) {
		for (int i = 0; i < static_cast<int>(Tank::PARTSMAX) - 2; i++) {
			SetModel(ModelMgr::GetInstance().GetModelPtr(XFileData[i].XfileName + x), i);
		}
	}
	else {
		std::string enemy = { "_enemy" };
		for (int i = 0; i < static_cast<int>(Tank::PARTSMAX) - 2; i++) {
			SetModel(ModelMgr::GetInstance().GetModelPtr(XFileData[i].XfileName + enemy + x), i);
		}
	}

	// プレイヤ行列初期化(初期位置)（自分のことだけを考えた行列を作成）
	for (int i = 0; i < static_cast<int>(TANKPARTS::PARTSMAX) - 2; i++) {
		DX11MakeWorldMatrix(m_mtxlocalpose[i], ObjectInitData[i].FirstAngle, ObjectInitData[i].FirstPosition);
	}

	// 親子関係を考慮した行列を計算する
	CaliculateParentChildMtx();					// 親の行列を掛け算してグローバルポーズを作る

	m_obb = new COBB();
	//OBB初期化
	m_obb->Init(m_pmodels[TANKPARTS::TANK_BASE]);

	m_radius = BoundingSphere_Manager::GetInstance().GetBSRadius(XFileData[TANKPARTS::TANK_BASE].XfileName + x);

	////煙を一回出しとく
	//GenerateSmoke();

	return true;
}

///<summary>
///描画
///</summary>
void Tank::Draw() {

	// モデル描画
//	drawaxis(m_mtx, 200, m_pos);

	if (m_isDraw) {
		for (int i = 0; i < static_cast<int>(TANKPARTS::PARTSMAX) - 2; i++) {

			m_pmodels[i]->Draw(m_mtxParentChild[i]);
		}
	}

	//m_obb->Draw();


	for (auto& fire : m_fireList)
		fire->Draw();


	/*for (auto& smoke : m_smokeList)
		smoke->Draw();*/
}

///<summary>
///描画
///</summary>
void Tank::DrawShadow() {

	// モデル描画
//	drawaxis(m_mtx, 200, m_pos);

	if (m_isDraw) {
		for (int i = 0; i < static_cast<int>(TANKPARTS::PARTSMAX) - 2; i++) {
			GetLight().SetMtxWorld(m_mtxParentChild[i]);
			m_pmodels[i]->DrawShadow(m_mtxParentChild[i]);
		}
	}

	//m_obb->Draw();

}

///<summary>
///描画
///</summary>
void Tank::DrawModel() {

	// モデル描画
//	drawaxis(m_mtx, 200, m_pos);

	if (m_isDraw) {
		for (int i = 0; i < static_cast<int>(TANKPARTS::PARTSMAX) - 2; i++) {
			GetLight().SetMtxWorld(m_mtxParentChild[i]);
			m_pmodels[i]->DrawModel(m_mtxParentChild[i]);
		}
	}

	/*for (auto& fire : m_fireList)
		fire->Draw();*/

	for (auto& smoke : m_smokeList)
		smoke->Draw();

	//m_obb->Draw();

}

/// <summary>
/// 演出準備
/// </summary>
void Tank::PrepareDestroy() {
	//乱数生成
	std::uniform_real_distribution<float> rotx(2, 5);
	std::uniform_real_distribution<float> roty(2, 5);
	std::uniform_real_distribution<float> rotz(2, 5);
	std::uniform_real_distribution<float> speed(1, 2);

	//値セット
	m_destroyRotx = rotx(m_mt);
	m_destroyRoty = roty(m_mt);
	m_destroyRotz = roty(m_mt);
	m_destroySpeed = speed(m_mt);
}

/// <summary>
/// 爆発時の演出
/// </summary>
bool Tank::DestroyEffect() {

	if (m_destroyCnt > 60) {
		return true;
	}

	//モデルを飛ばす
	for (int i = 0; i < static_cast<int>(TANKPARTS::PARTSMAX) - 2; i++) {

		XMFLOAT4 qt;	// クオータニオン

		XMFLOAT4 axisX;
		XMFLOAT4 axisY;
		XMFLOAT4 axisZ;
		XMFLOAT3 oldPos;

		GetAxisXfromMatrix(m_mtxParentChild[i], axisX);
		GetAxisYfromMatrix(m_mtxParentChild[i], axisY);
		GetAxisZfromMatrix(m_mtxParentChild[i], axisZ);

		oldPos.x = m_mtxParentChild[i]._41;
		oldPos.y = m_mtxParentChild[i]._42;
		oldPos.z = m_mtxParentChild[i]._43;

		// 行列からクオータニオンを生成
		DX11GetQtfromMatrix(m_mtxParentChild[i], qt);

		XMFLOAT4 qtx, qty, qtz;		// クオータニオン

		// 指定軸回転のクオータニオンを生成
		DX11QtRotationAxis(qtx, axisX, m_destroyRotx);
		DX11QtRotationAxis(qty, axisY, m_destroyRoty);
		DX11QtRotationAxis(qtz, axisZ, m_destroyRotz);

		// クオータニオンを合成
		XMFLOAT4 tempqt1;
		DX11QtMul(tempqt1, qt, qtx);

		XMFLOAT4 tempqt2;
		DX11QtMul(tempqt2, qty, qtz);

		XMFLOAT4 tempqt3;
		DX11QtMul(tempqt3, tempqt1, tempqt2);

		// クオータニオンをノーマライズ
		DX11QtNormalize(tempqt3, tempqt3);

		// クオータニオンから行列を作成
		DX11MtxFromQt(m_mtxParentChild[i], tempqt3);

		//各方向に飛ばす
		switch (i)
		{
		case 0:
			m_mtxParentChild[i]._41 = oldPos.x + m_destroySpeed;
			m_mtxParentChild[i]._43 = oldPos.z + m_destroySpeed;
			break;
		case 1:
			m_mtxParentChild[i]._41 = oldPos.x - m_destroySpeed;
			m_mtxParentChild[i]._43 = oldPos.z + m_destroySpeed;
			break;
		case 2:
			m_mtxParentChild[i]._41 = oldPos.x + m_destroySpeed;
			m_mtxParentChild[i]._43 = oldPos.z - m_destroySpeed;
			break;
		case 3:
			m_mtxParentChild[i]._41 = oldPos.x - m_destroySpeed;
			m_mtxParentChild[i]._43 = oldPos.z - m_destroySpeed;
			break;
		}


		m_mtxParentChild[i]._42 = oldPos.y + m_destroySpeed;

	}

	m_destroyCnt++;

	return false;
}

void Tank::UpdateSelect() {
	XMFLOAT4 axisX;
	XMFLOAT4 axisY;
	XMFLOAT4 axisZ;

	//　本体の移動処理
	//軸を取り出す
	GetAxisXfromMatrix(m_mtx, axisX);
	GetAxisYfromMatrix(m_mtx, axisY);
	GetAxisZfromMatrix(m_mtx, axisZ);

	XMFLOAT4 qt;	// クオータニオン

	// 行列からクオータニオンを生成
	DX11GetQtfromMatrix(m_mtx, qt);

	XMFLOAT4 qtx, qty, qtz;		// クオータニオン

	m_angle.y = ROTATIONRATE;

	// 指定軸回転のクオータニオンを生成
	DX11QtRotationAxis(qtx, axisX, m_angle.x);
	DX11QtRotationAxis(qty, axisY, m_angle.y);
	DX11QtRotationAxis(qtz, axisZ, m_angle.z);

	// クオータニオンを合成
	XMFLOAT4 tempqt1;
	DX11QtMul(tempqt1, qt, qtx);

	XMFLOAT4 tempqt2;
	DX11QtMul(tempqt2, qty, qtz);

	XMFLOAT4 tempqt3;
	DX11QtMul(tempqt3, tempqt1, tempqt2);

	// クオータニオンをノーマライズ
	DX11QtNormalize(tempqt3, tempqt3);

	// クオータニオンから行列を作成
	DX11MtxFromQt(m_mtx, tempqt3);

	UpdateEneLocalpose();

	// 親子関係を考慮した行列を計算する
	CaliculateParentChildMtx();
}

/// <summary>
/// ローカルの位置更新
/// </summary>
void Tank::UpdateLocalpose() {

	// 各パーツの回転角度(前フレームからの変位量)
	XMFLOAT3			partsangle[static_cast<int>(TANKPARTS::PARTSMAX)];
	XMFLOAT3			partstrans[static_cast<int>(TANKPARTS::PARTSMAX)];

	// どのパーツを動かすか
	int idx = 0;

	// 角度と移動量を初期化
	for (int i = 0; i < static_cast<int>(TANKPARTS::PARTSMAX); i++) {
		partsangle[i].x = 0.0f;
		partsangle[i].y = 0.0f;
		partsangle[i].z = 0.0f;
		partstrans[i].x = 0.0f;
		partstrans[i].y = 0.0f;
		partstrans[i].z = 0.0f;
	}

	// パーツのローカルポーズを決める為のキー操作
	// 砲身を動かす
	if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_UP)) {
		idx = TANK_BARREL;
		partsangle[idx].x += 1.0f;
	}

	// 砲身を動かす
	if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_DOWN)) {
		idx = TANK_BARREL;
		partsangle[idx].x -= 1.0f;
	}

	// 砲台を動かす
	if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_LEFT)) {
		idx = TANK_TOP;
		partsangle[idx].y += 1.0f;
	}

	// 砲台を動かす
	if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_RIGHT)) {
		idx = TANK_TOP;
		partsangle[idx].y -= 1.0f;
	}

	//弾発射
	if (CDirectInput::GetInstance().CheckKeyBufferTrigger(DIK_SPACE)) {


		XMFLOAT4X4 tempmtx = m_mtxParentChild[TANKPARTS::TANK_BARREL];

		//戦車の姿勢が逆のため反対にする
		tempmtx._31 = -tempmtx._31;
		tempmtx._32 = -tempmtx._32;
		tempmtx._33 = -tempmtx._33;
		tempmtx._34 = -tempmtx._34;

		//発射
	/*	Bullet_Manager::GetInstance().Fire(tempmtx, Bullet_Object::TYPEOBJ::PLAYER);
		Manager_Camera::GetInstance().shakeflag = true;*/
	}

	m_mtxlocalpose[Tank::TANK_CATERPILLAR] = m_mtx;

	// パーツの角度ローカルポーズを表す行列を計算
	XMFLOAT4X4 partsmtx;
	DX11MakeWorldMatrix(partsmtx, partsangle[idx], partstrans[idx]);
	DX11MtxMultiply(m_mtxlocalpose[idx], partsmtx, m_mtxlocalpose[idx]);

	m_mtxlocalpose[Tank::TANK_CATERPILLAR]._41 += m_mtxlocalpose[Tank::TANK_CATERPILLAR]._31 * m_speed;
	m_mtxlocalpose[Tank::TANK_CATERPILLAR]._42 += m_mtxlocalpose[Tank::TANK_CATERPILLAR]._32 * m_speed;
	m_mtxlocalpose[Tank::TANK_CATERPILLAR]._43 += m_mtxlocalpose[Tank::TANK_CATERPILLAR]._33 * m_speed;

	//一番下の階層の行列と位置をセット
	m_pos.x = m_mtxlocalpose[Tank::TANK_CATERPILLAR]._41;
	m_pos.y = m_mtxlocalpose[Tank::TANK_CATERPILLAR]._42;
	m_pos.z = m_mtxlocalpose[Tank::TANK_CATERPILLAR]._43;
	m_mtx = m_mtxlocalpose[Tank::TANK_CATERPILLAR];

	//OBB更新
	//m_obb->Update(mtx);
}

void Tank::UpdateEnemyLocalpose(XMFLOAT4X4 _playermtx) {

	// 各パーツの回転角度(前フレームからの変位量)
	XMFLOAT3			partsangle[static_cast<int>(TANKPARTS::PARTSMAX)];
	XMFLOAT3			partstrans[static_cast<int>(TANKPARTS::PARTSMAX)];

	XMFLOAT3 Axis;

	//プレイヤーの情報を取り出す
	XMFLOAT3 playerpos = { _playermtx._41,_playermtx._42,_playermtx._43 };

	//目標とのベクトル
	XMFLOAT3 targetvec3 = { playerpos.x - m_pos.x, playerpos.y - m_pos.y , playerpos.z - m_pos.z };
	XMFLOAT2 targetvec2 = { playerpos.x - m_pos.x, playerpos.z - m_pos.z };

	//自機ベクトル
	/*XMFLOAT3 myvec3 = { m_mtxParentChild[TANK_TOP]._31 * 2.0f, m_mtxParentChild[TANK_TOP]._32 * 2.0f, m_mtxParentChild[TANK_TOP]._33 * 2.0f };
	XMFLOAT2 myvec2 = { m_mtxParentChild[TANK_TOP]._31 * 2.0f ,m_mtxParentChild[TANK_TOP]._33 * 2.0f };*/

	XMFLOAT3 myvec3 = { m_mtxlocalpose[Tank::TANK_CATERPILLAR]._31 * 2.0f, m_mtxlocalpose[Tank::TANK_CATERPILLAR]._32 * 2.0f, m_mtxlocalpose[Tank::TANK_CATERPILLAR]._33 * 2.0f };
	XMFLOAT2 myvec2 = { m_mtxlocalpose[Tank::TANK_CATERPILLAR]._31 * 2.0f ,m_mtxlocalpose[Tank::TANK_CATERPILLAR]._33 * 2.0f };

	//正規化
	DX11Vec2Normalize(targetvec2, targetvec2);
	DX11Vec2Normalize(myvec2, myvec2);

	//外積で軸を求める
	DX11Vec3Cross(Axis, targetvec3, myvec3);

	//内積からcosθを計算
	float rot = targetvec2.x * myvec2.x + targetvec2.y * myvec2.y;

	//かくかくなるのを補正
	if (fabsf(rot) > 0.99f) {
		rot = 1.0f;
	}

	//ラジアン計算
	float rotdiff = acosf(rot);


	//軸によって角度をマイナスにする
	if (Axis.y < 0.0f) {
		rotdiff = -rotdiff;
	}

	// どのパーツを動かすか
	int idx = 0;

	// 角度と移動量を初期化
	for (int i = 0; i < static_cast<int>(TANKPARTS::PARTSMAX); i++) {
		partsangle[i].x = 0.0f;
		partsangle[i].y = 0.0f;
		partsangle[i].z = 0.0f;
		partstrans[i].x = 0.0f;
		partstrans[i].y = 0.0f;
		partstrans[i].z = 0.0f;
	}

	idx = TANK_CATERPILLAR;
	partsangle[idx].y += rotdiff;

	m_mtxlocalpose[Tank::TANK_CATERPILLAR] = m_mtx;

	// パーツの角度ローカルポーズを表す行列を計算
	XMFLOAT4X4 partsmtx;
	DX11MakeWorldMatrix(partsmtx, partsangle[idx], partstrans[idx]);
	DX11MtxMultiply(m_mtxlocalpose[idx], partsmtx, m_mtxlocalpose[idx]);

	m_mtxlocalpose[Tank::TANK_CATERPILLAR]._41 += m_mtxlocalpose[Tank::TANK_CATERPILLAR]._31 * m_speed;
	m_mtxlocalpose[Tank::TANK_CATERPILLAR]._42 += m_mtxlocalpose[Tank::TANK_CATERPILLAR]._32 * m_speed;
	m_mtxlocalpose[Tank::TANK_CATERPILLAR]._43 += m_mtxlocalpose[Tank::TANK_CATERPILLAR]._33 * m_speed;

	//一番下の階層の行列と位置をセット
	m_pos.x = m_mtxlocalpose[Tank::TANK_CATERPILLAR]._41;
	m_pos.y = m_mtxlocalpose[Tank::TANK_CATERPILLAR]._42;
	m_pos.z = m_mtxlocalpose[Tank::TANK_CATERPILLAR]._43;
	m_mtx = m_mtxlocalpose[Tank::TANK_CATERPILLAR];

	//OBB更新
	//m_obb->Update(mtx);

}

void Tank::UpdateEneLocalpose() {
	// 各パーツの回転角度(前フレームからの変位量)
	XMFLOAT3			partsangle[static_cast<int>(TANKPARTS::PARTSMAX)];
	XMFLOAT3			partstrans[static_cast<int>(TANKPARTS::PARTSMAX)];

	// どのパーツを動かすか
	int idx = 0;

	// 角度と移動量を初期化
	for (int i = 0; i < static_cast<int>(TANKPARTS::PARTSMAX); i++) {
		partsangle[i].x = 0.0f;
		partsangle[i].y = 0.0f;
		partsangle[i].z = 0.0f;
		partstrans[i].x = 0.0f;
		partstrans[i].y = 0.0f;
		partstrans[i].z = 0.0f;
	}


	m_mtxlocalpose[Tank::TANK_CATERPILLAR] = m_mtx;

	// パーツの角度ローカルポーズを表す行列を計算
	XMFLOAT4X4 partsmtx;
	DX11MakeWorldMatrix(partsmtx, partsangle[idx], partstrans[idx]);
	DX11MtxMultiply(m_mtxlocalpose[idx], partsmtx, m_mtxlocalpose[idx]);

	m_mtxlocalpose[Tank::TANK_CATERPILLAR]._41 += m_mtxlocalpose[Tank::TANK_CATERPILLAR]._31 * m_speed;
	m_mtxlocalpose[Tank::TANK_CATERPILLAR]._42 += m_mtxlocalpose[Tank::TANK_CATERPILLAR]._32 * m_speed;
	m_mtxlocalpose[Tank::TANK_CATERPILLAR]._43 += m_mtxlocalpose[Tank::TANK_CATERPILLAR]._33 * m_speed;

	//一番下の階層の行列と位置をセット
	m_pos.x = m_mtxlocalpose[Tank::TANK_CATERPILLAR]._41;
	m_pos.y = m_mtxlocalpose[Tank::TANK_CATERPILLAR]._42;
	m_pos.z = m_mtxlocalpose[Tank::TANK_CATERPILLAR]._43;
	m_mtx = m_mtxlocalpose[Tank::TANK_CATERPILLAR];

}

/// <summary>
/// 弾発射
/// </summary>
void Tank::Fire() {

	std::unique_ptr<Fire_Billboard> fire;

	XMFLOAT4X4 temp = m_mtxParentChild[Tank::TANK_BARREL];

	temp._31 = -temp._31;
	temp._32 = -temp._32;
	temp._33 = -temp._33;
	temp._34 = -temp._34;

	XMFLOAT3 pipepos = { temp._41 + temp._31 * PIPELENGTH, temp._42 + temp._32 * PIPELENGTH, temp._43 + temp._33 * PIPELENGTH };

	fire = std::make_unique<Fire_Billboard>(pipepos);
	fire->Init();

	m_fireList.emplace_back(std::move(fire));

	m_fireFlg = true;
	m_fireCnt = 0;

}


/// <summary>
/// 更新
/// </summary>
void Tank::Update() {

	// 移動量及び角度を０にする
	m_speed = 0.0f;
	m_angle.x = m_angle.y = m_angle.z = 0.0f;

	//煙削除
	auto itr = m_smokeList.begin();
	while (itr != m_smokeList.end()) {

		if ((*itr)->m_isDraw == false) {
			itr = m_smokeList.erase(itr);

		}
		else ++itr;
	}

	//煙を出す
	for (auto& smoke : m_smokeList) {
		smoke->Update();
	}

	static uint16_t l_smokecnt = 0;

	/*if (l_smokecnt > 20) {
		GenerateSmoke();
		l_smokecnt = 0;
	}
	l_smokecnt++;*/

	XMFLOAT4 axisX;
	XMFLOAT4 axisY;
	XMFLOAT4 axisZ;
	//軸を取り出す
	GetAxisXfromMatrix(m_mtx, axisX);
	GetAxisYfromMatrix(m_mtx, axisY);
	GetAxisZfromMatrix(m_mtx, axisZ);

	//爆発系処理
	if (m_fireCnt > m_fireInterval) {
		m_fireFlg = false;
	}

	if (m_fireFlg) {

		float t;

		t = static_cast<float>(m_fireCnt + 1) / static_cast<float>(m_fireInterval / 2);

		XMFLOAT3 pos;

		pos = Lerp(lerpstart, lerpend, t);

		m_fireCnt++;
		//m_move += ACCEL;
		m_keyInput = true;
	}
	else {
		m_fireMtx = m_mtxParentChild[Tank::TANK_BARREL];
	}


	auto it = m_fireList.begin();
	while (it != m_fireList.end()) {

		if ((*it)->m_isDraw == false) {
			it = m_fireList.erase(it);

		}
		else ++it;
	}


	if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_A)) {
		m_angle.y -= ROTATIONRATE;
		m_keyInput = true;
	}

	if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_D)) {
		m_angle.y += ROTATIONRATE;
		m_keyInput = true;
	}

	if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_W)) {
		//m_speed = -0.3f;
		m_move -= ACCEL;
		m_keyInput = true;
	}

	if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_S)) {
		//m_speed = 0.3f;
		m_move += ACCEL;
		m_keyInput = true;
	}



	static float lengthx = m_obb->GetOBB().fLengthX / 3;
	static float lengthz = m_obb->GetOBB().fLengthZ / 3;

	//ソート用リスト
	std::vector<XMFLOAT3> temp;

	//４つの支点の位置を計算
	for (int i = 0; i < 4; i++) {

		switch (i) {
		case RIGHTUP:
			m_ikpos[i].x = m_pos.x + axisZ.x * lengthx;
			m_ikpos[i].z = m_pos.z + axisZ.z * lengthz;
			break;
		case LEFTUP:
			m_ikpos[i].x = m_pos.x + axisZ.x * lengthx;
			m_ikpos[i].z = m_pos.z + axisZ.z * -lengthz;
			break;
		case RIGHTDOWN:
			m_ikpos[i].x = m_pos.x + axisZ.x * -lengthx;
			m_ikpos[i].z = m_pos.z + axisZ.z * lengthz;
			break;
		case LEFTDOWN:
			m_ikpos[i].x = m_pos.x + axisZ.x * -lengthx;
			m_ikpos[i].z = m_pos.z + axisZ.z * -lengthz;
			break;
		}

		bool sts = SearchAllSurface(m_ikpos[i].x, m_ikpos[i].z, m_ikpos[i]);
		if (sts) {
			temp.emplace_back(m_ikpos[i]);
		}
	}

	//ソートして入れ替える
	for (int i = 0; i < temp.size(); i++) {
		for (int j = i + 1; j < temp.size(); j++) {
			if (temp.at(i).y < temp.at(j).y) {
				XMFLOAT3 tmp = temp.at(i);
				temp.at(i) = temp.at(j);
				temp.at(j) = tmp;
			}
		}
	}

	//３点から法線生成
	XMFLOAT3 normal = CreateNormal(temp.at(0), temp.at(1), temp.at(2));

	//上方向のベクトル
	XMFLOAT3 up = { axisY.x,axisY.y,axisY.z };

	XMFLOAT3 dir;
	DX11Vec3Cross(dir, up, normal);
	DX11Vec3Normalize(dir, dir);

	float dot;

	DX11Vec3Dot(dot, up, dir);

	float rad = acosf(dot);

	XMFLOAT4 axisup = { up.x, up.y, up.z, 0.0f };

	XMFLOAT4 qtnor = { 0,0,0,1 };

	DX11QtRotationAxis(qtnor, axisup, rad);

	//m_angle.x = 45.0f;
	//float a = 0;
	// キー入力があった場合
	if (m_keyInput) {
		XMFLOAT4 qt;	// クオータニオン

		// 行列からクオータニオンを生成
		DX11GetQtfromMatrix(m_mtx, qt);

		XMFLOAT4 qtx, qty, qtz;		// クオータニオン

		// 指定軸回転のクオータニオンを生成
		DX11QtRotationAxis(qtx, axisX, m_angle.x);
		DX11QtRotationAxis(qty, axisY, m_angle.y);
		DX11QtRotationAxis(qtz, axisZ, m_angle.z);

		// クオータニオンを合成
		XMFLOAT4 tempqt1;
		DX11QtMul(tempqt1, qt, qtx);

		XMFLOAT4 tempqt2;
		DX11QtMul(tempqt2, qty, qtz);

		XMFLOAT4 tempqt3;
		DX11QtMul(tempqt3, tempqt1, tempqt2);

		XMFLOAT4 tempqt4;
		DX11QtMul(tempqt4, tempqt3, qtnor);

		// クオータニオンをノーマライズ
		DX11QtNormalize(tempqt3, tempqt3);

		// クオータニオンから行列を作成
		DX11MtxFromQt(m_mtx, tempqt3);

		/*a = 1 - tempqt3.x*tempqt3.x * 2 - tempqt3.z*tempqt3.z * 2;*/
	}
	else {
		//キーが押されてないときはスピードを下げていく
		if (m_move > 0) {
			m_move -= ACCEL;
		}
		else if (m_move < 0) {
			m_move += ACCEL;
		}

		if (fabsf(m_move) < 0.02f) {
			m_move = 0.0f;
		}
	}

	//最高速度に達したら
	if (m_move > MAXSPEED) {
		m_move = MAXSPEED;
	}
	else if (m_move < -MAXSPEED) {
		m_move = -MAXSPEED;
	}

	m_speed = m_move;

	//スピードによってカメラの位置を変える
	/*float crate = m_move / MAXSPEED;
	crate = fabsf(crate);
	XMFLOAT3 camerapos = { Manager_Camera::DEFPOSX, Manager_Camera::DEFPOSY, Manager_Camera::DEFPOSZ };
	camerapos.z = 10 * crate + Manager_Camera::DEFPOSZ;

	Manager_Camera::GetInstance().SetLocalPos(camerapos);*/

	// Z軸を取り出す
	axisZ.x = m_mtx._31;
	axisZ.y = m_mtx._32;
	axisZ.z = m_mtx._33;
	axisZ.w = 0.0f;

	// 中心位置を更新
	m_pos.x += axisZ.x*m_speed;
	m_pos.y += axisZ.y*m_speed;
	m_pos.z += axisZ.z*m_speed;

	XMFLOAT3 ans;

	bool sts = SearchAllSurface(m_pos.x, m_pos.z, ans);
	if (sts) {
		// 交点が見つかった場合
		m_mtx._41 = m_pos.x;
		m_mtx._42 = ans.y;
		//mtx._42 = pos.y;
		m_mtx._43 = m_pos.z;
	}
	else {
		// 交点が見つからなかった場合
		m_mtx._41 = m_pos.x;
		m_mtx._42 = m_pos.y;					// 高さ０
		m_mtx._43 = m_pos.z;
	}

	// ローカルポーズを更新する
	UpdateLocalpose();

	// 親子関係を考慮した行列を計算する
	CaliculateParentChildMtx();



	m_keyInput = false;
}

/// <summary>
/// 追跡更新
/// </summary>
/// <param name="_playermtx">自機姿勢</param>
/// <remarks>敵のみ</remarks>
void Tank::UpdateChase(XMFLOAT4X4 _playermtx) {

	XMFLOAT4 axisX;
	XMFLOAT4 axisY;
	XMFLOAT4 axisZ;

	/*static bool keyinput = true;*/

	//　本体の移動処理
	//軸を取り出す
	GetAxisXfromMatrix(m_mtx, axisX);
	GetAxisYfromMatrix(m_mtx, axisY);
	GetAxisZfromMatrix(m_mtx, axisZ);

	m_speed = -0.5f;

	XMFLOAT4 qt;	// クオータニオン

	// 行列からクオータニオンを生成
	DX11GetQtfromMatrix(m_mtx, qt);

	XMFLOAT4 qtx, qty, qtz;		// クオータニオン

	// 指定軸回転のクオータニオンを生成
	DX11QtRotationAxis(qtx, axisX, m_angle.x);
	DX11QtRotationAxis(qty, axisY, m_angle.y);
	DX11QtRotationAxis(qtz, axisZ, m_angle.z);

	// クオータニオンを合成
	XMFLOAT4 tempqt1;
	DX11QtMul(tempqt1, qt, qtx);

	XMFLOAT4 tempqt2;
	DX11QtMul(tempqt2, qty, qtz);

	XMFLOAT4 tempqt3;
	DX11QtMul(tempqt3, tempqt1, tempqt2);

	// クオータニオンをノーマライズ
	DX11QtNormalize(tempqt3, tempqt3);

	// クオータニオンから行列を作成
	DX11MtxFromQt(m_mtx, tempqt3);


	// Z軸を取り出す
	axisZ.x = m_mtx._31;
	axisZ.y = m_mtx._32;
	axisZ.z = m_mtx._33;
	axisZ.w = 0.0f;

	//最高速度に達したら
	if (m_move > MAXSPEED) {
		m_move = MAXSPEED;
	}
	else if (m_move < -MAXSPEED) {
		m_move = -MAXSPEED;
	}
	m_move -= ACCEL;
	m_speed = m_move;

	// 位置を更新
	m_pos.x += axisZ.x*m_speed;
	m_pos.y += axisZ.y*m_speed;
	m_pos.z += axisZ.z*m_speed;

	XMFLOAT3 ans;

	bool sts = SearchAllSurface(m_pos.x, m_pos.z, ans);
	if (sts) {
		// 交点が見つかった場合
		m_mtx._41 = m_pos.x;
		m_mtx._42 = ans.y;
		m_mtx._43 = m_pos.z;
	}
	else {
		// 交点が見つからなかった場合
		m_mtx._41 = m_pos.x;
		m_mtx._42 = m_pos.y;					// 高さ０
		m_mtx._43 = m_pos.z;
	}

	// ローカルポーズを更新する
	UpdateEnemyLocalpose(_playermtx);

	// 親子関係を考慮した行列を計算する
	CaliculateParentChildMtx();

	m_keyInput = false;
}

/// <summary>
/// 索敵更新
/// </summary>
/// <remarks>敵のみ</remarks>
void Tank::UpdateFree() {
	XMFLOAT4 axisX;
	XMFLOAT4 axisY;
	XMFLOAT4 axisZ;

	//　本体の移動処理
	//軸を取り出す
	GetAxisXfromMatrix(m_mtx, axisX);
	GetAxisYfromMatrix(m_mtx, axisY);
	GetAxisZfromMatrix(m_mtx, axisZ);

	XMFLOAT4 qt;	// クオータニオン

	// 行列からクオータニオンを生成
	DX11GetQtfromMatrix(m_mtx, qt);

	XMFLOAT4 qtx, qty, qtz;		// クオータニオン

	// 指定軸回転のクオータニオンを生成
	DX11QtRotationAxis(qtx, axisX, m_angle.x);
	DX11QtRotationAxis(qty, axisY, m_angle.y);
	DX11QtRotationAxis(qtz, axisZ, m_angle.z);

	// クオータニオンを合成
	XMFLOAT4 tempqt1;
	DX11QtMul(tempqt1, qt, qtx);

	XMFLOAT4 tempqt2;
	DX11QtMul(tempqt2, qty, qtz);

	XMFLOAT4 tempqt3;
	DX11QtMul(tempqt3, tempqt1, tempqt2);

	// クオータニオンをノーマライズ
	DX11QtNormalize(tempqt3, tempqt3);

	// クオータニオンから行列を作成
	DX11MtxFromQt(m_mtx, tempqt3);


	// Z軸を取り出す
	axisZ.x = m_mtx._31;
	axisZ.y = m_mtx._32;
	axisZ.z = m_mtx._33;
	axisZ.w = 0.0f;

	m_pos.x += axisZ.x*m_speed;
	m_pos.y += axisZ.y*m_speed;
	m_pos.z += axisZ.z*m_speed;

	XMFLOAT3 ans;

	bool sts = SearchAllSurface(m_pos.x, m_pos.z, ans);
	if (sts) {
		// 交点が見つかった場合
		m_mtx._41 = m_pos.x;
		m_mtx._42 = ans.y;
		m_mtx._43 = m_pos.z;
	}
	else {
		// 交点が見つからなかった場合
		m_mtx._41 = m_pos.x;
		m_mtx._42 = m_pos.y;					// 高さ０
		m_mtx._43 = m_pos.z;
	}


	UpdateEneLocalpose();

	// 親子関係を考慮した行列を計算する
	CaliculateParentChildMtx();

}

/// <summary>
///　旋回更新
/// </summary>
void Tank::UpdateCycle() {
	// 各パーツの回転角度(前フレームからの変位量)
	XMFLOAT3			partsangle[static_cast<int>(TANKPARTS::PARTSMAX)];
	XMFLOAT3			partstrans[static_cast<int>(TANKPARTS::PARTSMAX)];

	XMFLOAT3 Axis;

	//目標とのベクトル
	//逆方向
	XMFLOAT3 targetvec3 = { m_mtxlocalpose[Tank::TANK_CATERPILLAR]._31 * -2.0f, m_mtxlocalpose[Tank::TANK_CATERPILLAR]._32 * -2.0f, m_mtxlocalpose[Tank::TANK_CATERPILLAR]._33 * -2.0f };
	XMFLOAT2 targetvec2 = { m_mtxlocalpose[Tank::TANK_CATERPILLAR]._31 * -2.0f ,m_mtxlocalpose[Tank::TANK_CATERPILLAR]._33 * -2.0f };


	XMFLOAT3 myvec3 = { m_mtxlocalpose[Tank::TANK_CATERPILLAR]._31 * 2.0f, m_mtxlocalpose[Tank::TANK_CATERPILLAR]._32 * 2.0f, m_mtxlocalpose[Tank::TANK_CATERPILLAR]._33 * 2.0f };
	XMFLOAT2 myvec2 = { m_mtxlocalpose[Tank::TANK_CATERPILLAR]._31 * 2.0f ,m_mtxlocalpose[Tank::TANK_CATERPILLAR]._33 * 2.0f };

	//正規化
	DX11Vec2Normalize(targetvec2, targetvec2);
	DX11Vec2Normalize(myvec2, myvec2);

	//外積で軸を求める
	DX11Vec3Cross(Axis, targetvec3, myvec3);

	//内積からcosθを計算
	float rot = targetvec2.x * myvec2.x + targetvec2.y * myvec2.y;

	//かくかくなるのを補正
	if (fabsf(rot) > 0.99f) {
		rot = -1.0f;
	}

	//ラジアン計算
	float rotdiff = acosf(rot);

	//軸によって角度をマイナスにする
	if (Axis.y < 0.0f) {
		rotdiff = -rotdiff;
	}

	// どのパーツを動かすか
	int idx = 0;

	// 角度と移動量を初期化
	for (int i = 0; i < static_cast<int>(TANKPARTS::PARTSMAX); i++) {
		partsangle[i].x = 0.0f;
		partsangle[i].y = 0.0f;
		partsangle[i].z = 0.0f;
		partstrans[i].x = 0.0f;
		partstrans[i].y = 0.0f;
		partstrans[i].z = 0.0f;
	}

	idx = TANK_CATERPILLAR;
	partsangle[idx].y += rotdiff;

	m_mtxlocalpose[Tank::TANK_CATERPILLAR] = m_mtx;

	// パーツの角度ローカルポーズを表す行列を計算
	XMFLOAT4X4 partsmtx;
	DX11MakeWorldMatrix(partsmtx, partsangle[idx], partstrans[idx]);
	DX11MtxMultiply(m_mtxlocalpose[idx], partsmtx, m_mtxlocalpose[idx]);

	m_mtxlocalpose[Tank::TANK_CATERPILLAR]._41 += m_mtxlocalpose[Tank::TANK_CATERPILLAR]._31 * m_speed;
	m_mtxlocalpose[Tank::TANK_CATERPILLAR]._42 += m_mtxlocalpose[Tank::TANK_CATERPILLAR]._32 * m_speed;
	m_mtxlocalpose[Tank::TANK_CATERPILLAR]._43 += m_mtxlocalpose[Tank::TANK_CATERPILLAR]._33 * m_speed;

	XMFLOAT3 ans;

	//一番下の階層の行列と位置をセット
	m_pos.x = m_mtxlocalpose[Tank::TANK_CATERPILLAR]._41;
	m_pos.y = m_mtxlocalpose[Tank::TANK_CATERPILLAR]._42;
	m_pos.z = m_mtxlocalpose[Tank::TANK_CATERPILLAR]._43;
	m_mtx = m_mtxlocalpose[Tank::TANK_CATERPILLAR];

	bool sts = SearchAllSurface(m_pos.x, m_pos.z, ans);
	if (sts) {
		// 交点が見つかった場合
		m_mtx._41 = m_pos.x;
		m_mtx._42 = ans.y;
		m_mtx._43 = m_pos.z;
	}
	else {
		// 交点が見つからなかった場合
		m_mtx._41 = m_pos.x;
		m_mtx._42 = m_pos.y;					// 高さ０
		m_mtx._43 = m_pos.z;
	}

	CaliculateParentChildMtx();
}

/// <summary>
/// 後処理
/// </summary>
void Tank::Finalize() {

	delete m_obb;
}

/// <summary>
/// 親の行列から子の行列を生成
/// </summary>
void Tank::CaliculateParentChildMtx() {

	// ローカルポーズを親方向に掛けていく
	// キャタピラ
	m_mtxParentChild[Tank::TANK_CATERPILLAR] = m_mtxlocalpose[Tank::TANK_CATERPILLAR];

	// 本体
	DX11MtxMultiply(m_mtxParentChild[Tank::TANK_BASE], m_mtxlocalpose[Tank::TANK_BASE], m_mtxlocalpose[Tank::TANK_CATERPILLAR]);

	// 砲台						   
	DX11MtxMultiply(m_mtxParentChild[Tank::TANK_TOP], m_mtxlocalpose[Tank::TANK_TOP], m_mtxlocalpose[Tank::TANK_BASE]);
	DX11MtxMultiply(m_mtxParentChild[Tank::TANK_TOP], m_mtxParentChild[Tank::TANK_TOP], m_mtxlocalpose[Tank::TANK_CATERPILLAR]);

	// 砲身						   
	DX11MtxMultiply(m_mtxParentChild[Tank::TANK_BARREL], m_mtxlocalpose[Tank::TANK_BARREL], m_mtxlocalpose[Tank::TANK_TOP]);
	DX11MtxMultiply(m_mtxParentChild[Tank::TANK_BARREL], m_mtxParentChild[Tank::TANK_BARREL], m_mtxlocalpose[Tank::TANK_BASE]);
	DX11MtxMultiply(m_mtxParentChild[Tank::TANK_BARREL], m_mtxParentChild[Tank::TANK_BARREL], m_mtxlocalpose[Tank::TANK_CATERPILLAR]);

}

const XMFLOAT4X4 Tank::GetPipeMtx() const {

	if (m_fireFlg) {
		return m_mtxParentChild[Tank::TANK_BARREL];
		//return m_fireMtx;

	}
	else {
		return m_mtxParentChild[Tank::TANK_BARREL];
	}
}

void Tank::GenerateSmoke() {

	std::unique_ptr<Smoke_Billboard> smoke;

	XMFLOAT4 axisZ;
	XMFLOAT3 temppos[2];

	GetAxisZfromMatrix(m_mtx, axisZ);

	static float lengthx = m_obb->GetOBB().fLengthX / 2;
	static float lengthz = m_obb->GetOBB().fLengthZ / 2;

	temppos[0].x = m_pos.x + axisZ.x * lengthx;
	temppos[0].y = 1.0f;
	temppos[0].z = m_pos.z + axisZ.z * lengthz;

	temppos[1].x = m_pos.x + axisZ.x * lengthx;
	temppos[1].z = m_pos.z - axisZ.z * lengthz;

	smoke = std::make_unique<Smoke_Billboard>(temppos[0]);
	smoke->Init();
	m_smokeList.emplace_back(std::move(smoke));

	//smoke = std::make_unique<Smoke_Billboard>(temppos[1]);
	//smoke->Init();
	//m_smokeList.emplace_back(std::m_move(smoke));
}