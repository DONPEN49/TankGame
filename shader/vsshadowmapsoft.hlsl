#include	"psvscommon.hlsl"

//--------------------------------------------------------------------------------------
// 頂点シェーダー
//--------------------------------------------------------------------------------------
VS_OUTPUT main(float4 Pos : POSITION,
	float4 Normal : NORMAL,
	float2 Tex : TEXCOORD)
{
    VS_OUTPUT output = (VS_OUTPUT) 0;

    output.Pos = mul(Pos, World);
    output.WPos = output.Pos;
    output.Pos = mul(output.Pos, View);
    output.Pos = mul(output.Pos, Projection);

	// ローカル座標をピクセルシェーダーに渡す
    output.LPos = Pos;
    output.LPos.w = 1.0f;

    float4 N = Normal;
    N.w = 0.0f; // 法線はベクトルなのでＷの値を０にする。

    N = normalize(N); // 正規化

    output.Normal = N;

    output.Tex = Tex;

    output.Color = 0;
    output.Color2 = 0;

	// 光源からの距離を計算
    float4 lengthfromlight;
    lengthfromlight = mul(Pos, World);
    lengthfromlight = mul(lengthfromlight, ViewFromLight);
    lengthfromlight = mul(lengthfromlight, ProjectionFromLight);
    output.LengthFromLight = lengthfromlight;

	// シャドウマップを参照するためのＵＶ座標作成
    output.ShadowTex = mul(lengthfromlight, ScreenToUVCoord);

    return output;
}