#include	"psvscommon.hlsl"

Texture2D g_shadowmap : register(t1);

//--------------------------------------------------------------------------------------
//  �s�N�Z���V�F�[�_�[
//--------------------------------------------------------------------------------------
float4 main(VS_OUTPUT input) : SV_Target
{
    float4 col;
    float4 texcol = g_Tex.Sample(g_SamplerLinear, input.Tex);

    float offset = 0.00014f;
    
    float4 N;
    N = input.Normal;
    N.w = 0.0f;
    N = normalize(N);

    float4 L = invLightDirection;
    L = normalize(L);
    L.w = 0.0f;

    float d;
    d = max(0.0f, dot(L, N)); // �����o�[�g�̗]�����ŉA�e��t����

    float4 diffuse;
    diffuse = d * diffuseMaterial;

	// �X�y�L�����̌v�Z
    float s;

    float4 V; // �����x�N�g��
    V = invEyePos - input.LPos;
    V = normalize(V);

    float4 H;
    H = L + V; // �n�[�t�x�N�g��
    H = normalize(H);

    s = pow(max(0.0f, dot(N, H)), 3000);

    float4 specular;
    specular = s;

    col = texcol * diffuse + specular;
    col.a = 1.0f;

	// �V���h�E�}�b�v����Օ����܂ł̋��������o��
    float2 uv = input.ShadowTex.xy / input.ShadowTex.w;
    
    // ���C�g�r���[�X�N���[����Ԃł�Z�l���v�Z����
    float zView = input.ShadowTex.z / input.ShadowTex.w;
    
   
    
    //if (uv.x > 0.0f && uv.x < 1.0f)
    //{
    //    if (uv.y > 0.0f && uv.y < 1.0f)
    //    {
    //        float3 shadowColor = col.xyz * 0.5f;
                   
    //        col.xyz = lerp(col.xyz, shadowColor, shadowRate);
            
    //    }
    //}

    return col;
}