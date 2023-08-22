#include <cinttypes>
#include "dx11util.h"
#include "CModel.h"
#include "CCamera.h"
#include "DX11Settransform.h"
#include "dx11mathutil.h"
#include "Application.h"
#include "CDirectInput.h"
#include "ModelMgr.h"
#include "quad2d.h"
#include "2dsystem.h"
#include "blendstate.h"
#include "SceneManager.h"
#include "Title_Scene.h"
#include "Scene_Game.h"
#include "Shadowmap.h"
#include "Texture2D_Manager.h"
#include "Manager_Billboard.h"
#include "BoundingSphere_Manager.h"
#include "time.h"

SceneManager sceneManager;

void GameInit() {
	// DX11������
	DX11Init(
		Application::Instance()->GetHWnd(),
		Application::CLIENT_WIDTH,
		Application::CLIENT_HEIGHT,
		false);
	
	Init2D();

	InitBlendState();

	// �J�������K�v
	DirectX::XMFLOAT3 eye(0, 30, 75);		// �J�����̈ʒu
	DirectX::XMFLOAT3 lookat(0, 0, 0);		// �����_
	DirectX::XMFLOAT3 up(0, 1, 0);			// �J�����̏�����x�N�g��

	CCamera::GetInstance()->Init(
		10.0f,								// �j�A�N���b�v
		1000.0f,							// �t�@�[�N���b�v
		XM_PI / 4.0f,						// ����p
		Application::CLIENT_WIDTH,			// �X�N���[����
		Application::CLIENT_HEIGHT,			// �X�N���[���̍���
		eye, lookat, up);					// �J�����̃f�[�^

	// ���s�������Z�b�g
	DX11LightInit(
		DirectX::XMFLOAT4(1, 1, -1, 0));

	// �A���t�@�u�����h�L����
	TurnOnAlphablend();

	// DirectT INPUT ������
	CDirectInput::GetInstance().Init(
		Application::Instance()->GetHInst(),
		Application::Instance()->GetHWnd(),
		Application::CLIENT_WIDTH,
		Application::CLIENT_HEIGHT);

	// ���f���̓ǂݍ���
	ModelMgr::GetInstance().AllModelLoad();

	//�e�N�X�`���ǂݍ���
	Manager_Texture2D::GetInstance().AllTexture2DLoad();

	//�o�E���f�B���O�X�t�B�A����
	BoundingSphere_Manager::GetInstance().AllBSCreate();

	//�r���{�[�h�ǂݍ���
	Manager_Billboard::GetInstance().LoadAllBillboard();

	ShadowMap::GetInstance().Init();

	sceneManager.add<Title_Scene>("Title");
	sceneManager.add<Game_Scene>("Game");
	sceneManager.setCurrentScene("Title");

}

void GameInput(uint64_t dt) {

	CDirectInput::GetInstance().GetKeyBuffer();
	
	//�V�[���J��
	if (CDirectInput::GetInstance().CheckKeyBufferTrigger(DIK_SPACE)) {
		if (sceneManager.GetCurrentSceneKey() == "Title") {
			sceneManager.SetFadeColor(XMFLOAT4(0, 0, 0, 1));
			sceneManager.changeScene<Game_Scene>("Game", 1000, false);
		}
	}

}

void GameUpdate(uint64_t dt) {

	sceneManager.Update();
}

void GameRender(uint64_t dt) {

	float col[4] = { 1,0,0,1 };

	// �`��O����
	DX11BeforeRender(col);

	XMFLOAT4X4 mtx;

	// �v���W�F�N�V�����ϊ��s��擾
	mtx = CCamera::GetInstance()->GetProjectionMatrix();
	DX11SetTransform::GetInstance()->SetTransform(DX11SetTransform::TYPE::PROJECTION, mtx);

	// �r���[�ϊ��s����擾
	mtx = CCamera::GetInstance()->GetCameraMatrix();
	DX11SetTransform::GetInstance()->SetTransform(DX11SetTransform::TYPE::VIEW, mtx);

	sceneManager.Draw();

	// �`��㏈��
	DX11AfterRender();
}

void GameDispose() {

	

	DX11Uninit();
}

