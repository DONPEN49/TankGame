Texture2D g_Tex : register(t0);				    // テクスチャ
SamplerState g_SamplerLinear : register(s0);	// サンプラー
SamplerComparisonState g_SampleShadowMap : register(s1);

//--------------------------------------------------------------------------------------
// コンスタントバッファ
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
	float4 LightDirection;			// 光の方向
	float4 EyePos;					// 視点位置
	float4 Ambient;
    float4 invLightDirection;
    float4 invEyePos;
}

cbuffer ConstantBufferViewPort : register(b5)
{
    uint4 ViewportWidth;        // ビューポート幅
    uint4 ViewportHeight;       // ビューポート高さ
};

cbuffer ConstantBufferShadowmap : register(b6)
{
    matrix ViewFromLight; // 光源の位置カメラ
    matrix ProjectionFromLight; // 光源の位置カメラに対応したプロジェクション変換行列
    matrix ScreenToUVCoord; // スクリーン座標をテクスチャ座標空間に変換
}

//--------------------------------------------------------------------------------------
// 構造体定義
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
    float4 ShadowTex : TEXCOORD5; // シャドウマップを参照するＵＶ座標
    float4 PosProj : TEXCOORD6;
};

