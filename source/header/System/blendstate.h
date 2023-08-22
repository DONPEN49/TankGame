#pragma once

enum class BLENDSTATE {
	DEFAULT = 0,
	SRCALPHA,
	ADD,
	SUB
};

// �u�����h�X�e�[�g������
void InitBlendState();

// �\�[�X�A���t�@�u�����h�X�e�[�g����
void CreateBlendStateSrcAlpha();
// ���Z�����u�����h�X�e�[�g����
void CreateBlendStateOne();
// �f�t�H���g�u�����h�X�e�[�g����
void CreateBlendStateDefault();
// ���Z�����u�����h�X�e�[�g����
void CreateBlendStateSubtract();
void SetBlendState(BLENDSTATE bs);
