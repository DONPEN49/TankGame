//--------------------------------------------------------------------------------------
// ps.fx
//--------------------------------------------------------------------------------------
#include	"psvscommon.hlsl"

Texture2D g_depthmap : register(t1);

//--------------------------------------------------------------------------------------
//  ピクセルシェーダー
//--------------------------------------------------------------------------------------
float4 main(VS_OUTPUT input) : SV_Target
{
    
    float2 uv = input.PosProj.xy * float2(0.5f, -0.5f) + 0.5f;
    
    float2 uvoffset[8] =
    {
        float2( 0.0f            ,  1.0f / 720.0f),
        float2( 0.0f            , -1.0f / 720.0f),
        float2( 1.0f / 1280.0f  ,           0.0f),
        float2(-1.0f / 1280.0f  ,           0.0f),
        float2( 1.0f / 1280.0f  ,  1.0f / 720.0f),
        float2(-1.0f / 1280.0f  ,  1.0f / 720.0f),
        float2( 1.0f / 1280.0f  , -1.0f / 720.0f),
        float2(-1.0f / 1280.0f  , -1.0f / 720.0f),
    };
    
    float depth = g_depthmap.Sample(g_SamplerLinear, uv).x;
    float depth2 = 0.0f;
    
    for (int i = 0; i < 8; i++)
    {
        depth2 += g_depthmap.Sample(g_SamplerLinear, uv + uvoffset[i]).x;
    }
    
    depth2 /= 8.0f;
    
    if (abs(depth - depth2) > 0.00005f)
    {
        return float4(0.0f, 0.0f, 0.0f, 1.0f);
    }
    
    float4 col;
    float4 texcol = g_Tex.Sample(g_SamplerLinear, input.Tex);

    float4 N;
    N = input.Normal;
    N.w = 0.0f;
    N = normalize(N);

    float4 L = invLightDirection;
    L = normalize(L);
    L.w = 0.0f;

    float d;
    d = max(0.0f, dot(L, N)); // ランバートの余弦則で陰影を付ける

    float4 diffuse;
    diffuse = d * diffuseMaterial;

	// スペキュラの計算
    float s;

    float4 V; // 視線ベクトル
    V = invEyePos - input.LPos;
    V = normalize(V);

    float4 H;
    H = L + V; // ハーフベクトル
    H = normalize(H);

    s = pow(max(0.0f, dot(N, H)), 3000);

    float4 specular;
    specular = s;

    col = texcol * diffuse + specular;
    col.a = 1.0f;
    return col;
}