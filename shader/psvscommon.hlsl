Texture2D g_Tex : register(t0);				    // �e�N�X�`��
SamplerState g_SamplerLinear : register(s0);	// �T���v���[
SamplerComparisonState g_SampleShadowMap : register(s1);

//--------------------------------------------------------------------------------------
// �R���X�^���g�o�b�t�@
//--------------------------------------------------------------------------------------
cbuffer ConstantBufferWorld : register(b0)
{
	matrix World;
}

cbuffer ConstantBufferView : register(b1)
{
	matrix View;
}

cbuffer ConstantBufferProjection : register(b2)
{
	matrix Projection;
}

cbuffer ConstantBufferMaterial : register(b3)
{
	float4	ambientMaterial;
	float4	diffuseMaterial;
	float4	specularMaterial;
}

cbuffer ConstantBufferLight : register(b4)
{
	float4 LightDirection;			// ���̕���
	float4 EyePos;					// ���_�ʒu
	float4 Ambient;
    float4 invLightDirection;
    float4 invEyePos;
}

cbuffer ConstantBufferViewPort : register(b5)
{
    uint4 ViewportWidth;        // �r���[�|�[�g��
    uint4 ViewportHeight;       // �r���[�|�[�g����
};

cbuffer ConstantBufferShadowmap : register(b6)
{
    matrix ViewFromLight; // �����̈ʒu�J����
    matrix ProjectionFromLight; // �����̈ʒu�J�����ɑΉ������v���W�F�N�V�����ϊ��s��
    matrix ScreenToUVCoord; // �X�N���[�����W���e�N�X�`�����W��Ԃɕϊ�
}

//--------------------------------------------------------------------------------------
// �\���̒�`
//--------------------------------------------------------------------------------------
struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;
    float4 Color : COLOR0;
    float4 Color2 : COLOR1;
    float2 Tex : TEXCOORD;
    float4 WPos : TEXCOORD1;
    float4 Normal : TEXCOORD2;
    float4 LPos : TEXCOORD3;
    float4 LengthFromLight : TEXCOORD4;
    float4 ShadowTex : TEXCOORD5; // �V���h�E�}�b�v���Q�Ƃ���t�u���W
    float4 PosProj : TEXCOORD6;
};

