

//------------------------------------------------------
//		�T���v���[
//------------------------------------------------------

texture Texture;
sampler DecaleSamp = sampler_state
{
    Texture = <Texture>;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    MipFilter = LINEAR;

    AddressU = Wrap;
    AddressV = Wrap;
};


texture NormalDepthTex;
sampler NormalDepthSamp = sampler_state
{
    Texture = <NormalDepthTex>;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    MipFilter = NONE;

    AddressU = CLAMP;
    AddressV = CLAMP;
};


texture RoughnessLightMapTex;
sampler RoughnessLightMapSamp = sampler_state
{
    Texture = <RoughnessLightMapTex>;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    MipFilter = NONE;

    AddressU = BORDER;
    AddressV = BORDER;
};




texture ShadowDepthTex;
sampler ShadowDepthSamp = sampler_state
{
    Texture = <ShadowDepthTex>;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    MipFilter = NONE;

    AddressU = CLAMP;
    AddressV = CLAMP;
};


texture ShadowDepthTex2;
sampler ShadowDepthSamp2 = sampler_state
{
    Texture = <ShadowDepthTex2>;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    MipFilter = NONE;

    AddressU = CLAMP;
    AddressV = CLAMP;
};

//------------------------------------------------------
//	�萔
//------------------------------------------------------

float4x4 g_W_mat;
float4x4 g_P_mat;
float4x4 g_VP_mat;
float4x4 g_V_inv_mat;
float4x4 g_P_inv_mat;

float4x4 g_Shadow_VP_mat;

float3 g_LightPos;
float3 g_LightVec;
float3 g_LightColor;
float3 g_LightGroundColor;
float  g_LightSize;
float  g_LightMaxAngle;

#define g_OcclusionSampNum 8

float  g_OcclusionRatio = 1.0f / g_OcclusionSampNum;
float  g_OcclusionSampSize;
float  g_OcclusionCalcDiffZ;

float g_TexOffsetX;
float g_TexOffsetY;


float2 g_OcclusionSampVec[] =
{
    float2(-0.5f, 0.5f),
    float2(0.5f, 0.5f),
    float2(-0.5f, -0.5f),
    float2(0.5f, -0.5f),

    float2(1, 0),
    float2(-1, 0),
    float2(0, 1),
    float2(0, -1)
};


#include "Function.inc"

//------------------------------------------------------
//	
//------------------------------------------------------

//���s���s�N�Z���V�F�[�_�[�C���v�b�g
struct PS_DirLight_INPUT
{
    float4 Pos		: POSITION;
    float2 Tex		: TEXCOORD0;
};

//�����s�N�Z���V�F�[�_�[�C���v�b�g
struct PS_AmbientLight_INPUT
{
    float4 Pos		: POSITION;
    float2 Tex		: TEXCOORD0;
};

//�_�����o�[�e�b�N�X�V�F�[�_�A�E�g�v�b�g
struct VS_PointLight_OUTPUT
{
    float4 Pos   : POSITION;
    float3 pXYW  : TEXCOORD0;
    float  CenterLen  : TEXCOORD1;
};

//�X�|�b�g���C�g�o�[�e�b�N�X�V�F�[�_�A�E�g�v�b�g
struct VS_SpotLight_OUTPUT
{
    float4 Pos   : POSITION;
    float3 pXYW  : TEXCOORD0;
};

//Z�l�������݃o�[�e�b�N�X�V�F�[�_�A�E�g�v�b�g
struct VS_WriteZ_OUTPUT
{
    float4 Pos   : POSITION;
    float2 ZW    : TEXCOORD0;
    float2 Tex   : TEXCOORD1;
};
//���ʏo��
struct PS_Light_OUTPUT
{
    float4 Def   : COLOR0;
    float4 Sp    : COLOR1;
};

//------------------------------------------------------
//		���s��
//------------------------------------------------------

PS_Light_OUTPUT PS_DirLight(PS_DirLight_INPUT In) 
{
    const float4 NormalDepth   = tex2D(NormalDepthSamp,   In.Tex);
    
    const float3 Normal = CreateNormal(NormalDepth.rg * 2.0f - 1.0f);
    const float3 Pos = CalcViewPosition(In.Tex, NormalDepth.zw);
    const float2 RoughnessLightMap = tex2D(RoughnessLightMapSamp, In.Tex).rg;

    PS_Light_OUTPUT Out;

    //�X�y�L����
    float Sp = CalcSpecular(
            Pos,
            Normal,
            g_LightVec,
            RoughnessLightMap.r * 255
        );


    //�v�Z���ʂ���
    Out.Sp.rgb = Sp  * g_LightColor * RoughnessLightMap.g;

    Out.Def.rgb =
        g_LightColor *
        max(dot(Normal, -g_LightVec), 0) *
        RoughnessLightMap.g;

    Out.Def.a = 1;
    Out.Sp.a = 1;


    return Out;
}


technique DirLight
{
    pass P0
    {
        AlphaBlendEnable = true;
        BlendOp = Add;
        SrcBlend = SrcAlpha;
        DestBlend = One;
        CullMode = CCW;
        ZEnable = false;
        ZWriteEnable = false;

        PixelShader = compile ps_2_0 PS_DirLight();
    }
}

//------------------------------------------------------
//		���s���i�e����j
//------------------------------------------------------

PS_Light_OUTPUT PS_DirLightShadow(PS_DirLight_INPUT In)
{
    const float4 NormalDepth = tex2D(NormalDepthSamp, In.Tex);
    const float3 Normal = CreateNormal(NormalDepth.rg * 2.0f - 1.0f);
    const float3 Pos = CalcViewPosition(In.Tex, NormalDepth.zw);
    const float2 RoughnessLightMap = tex2D(RoughnessLightMapSamp, In.Tex).rg;

    PS_Light_OUTPUT Out;

    {
        float4 ShadowTex;

        ShadowTex = mul(mul(float4(Pos, 1), g_V_inv_mat), g_Shadow_VP_mat);
        ShadowTex.xy = ShadowTex.xy / ShadowTex.w;
        ShadowTex.y = -ShadowTex.y;
        ShadowTex.xy = 0.5f * ShadowTex.xy + 0.5f;

        if (tex2D(ShadowDepthSamp, ShadowTex.xy).r < ShadowTex.z - 0.005f)
        {
            discard;
        }
    }

    //�X�y�L����
    float Sp = CalcSpecular(
        Pos,
        Normal,
        g_LightVec,
        RoughnessLightMap.r * 255
        );


    //�v�Z���ʂ���
    Out.Sp.rgb = Sp  * g_LightColor * RoughnessLightMap.g;

    Out.Def.rgb =
        g_LightColor *
        max(dot(Normal, -g_LightVec),0)
        * RoughnessLightMap.g;

    Out.Def.a = 1;
    Out.Sp.a = 1;

    return Out;
}


technique DirLightShadow
{
    pass P0
    {
        AlphaBlendEnable = true;
        BlendOp = Add;
        SrcBlend = SrcAlpha;
        DestBlend = One;
        CullMode = CCW;
        ZEnable = false;
        ZWriteEnable = false;

        PixelShader = compile ps_2_0 PS_DirLightShadow();
    }
}

//------------------------------------------------------
//		���s���i�\�t�g�V���h�E�j
//------------------------------------------------------

PS_Light_OUTPUT PS_DirLightShadowVariance(PS_DirLight_INPUT In)
{
    const float4 NormalDepth = tex2D(NormalDepthSamp, In.Tex);
    const float3 Normal = CreateNormal(NormalDepth.rg * 2.0f - 1.0f);
    const float3 Pos = CalcViewPosition(In.Tex, NormalDepth.zw);
    const float2 RoughnessLightMap = tex2D(RoughnessLightMapSamp, In.Tex).rg;

    PS_Light_OUTPUT Out;
    float ShadowValue = 0;
    float2 depth;
    float4 ShadowTex;

    {
        ShadowTex = mul(mul(float4(Pos, 1), g_V_inv_mat), g_Shadow_VP_mat);
        ShadowTex.xy = ShadowTex.xy / ShadowTex.w;
        ShadowTex.y = -ShadowTex.y;
        ShadowTex.xy = 0.5f * ShadowTex.xy + 0.5f;

        depth = tex2D(ShadowDepthSamp2, ShadowTex.xy).rg;
    }

    //float2�ɂ��Ȃ��Ă�������񂶂�ː�

    {
        //�u���[����
        float2 otherDepth = float2(0, 0);

        otherDepth += tex2D(ShadowDepthSamp2, ShadowTex.xy + float2(-g_TexOffsetX, g_TexOffsetY)).rg;
        otherDepth += tex2D(ShadowDepthSamp2, ShadowTex.xy + float2(0, g_TexOffsetY)).rg;
        otherDepth += tex2D(ShadowDepthSamp2, ShadowTex.xy + float2(g_TexOffsetX, g_TexOffsetY)).rg;

        otherDepth += tex2D(ShadowDepthSamp2, ShadowTex.xy + float2(-g_TexOffsetX, 0)).rg;
        otherDepth += tex2D(ShadowDepthSamp2, ShadowTex.xy + float2(g_TexOffsetX, 0)).rg;
  
        otherDepth += tex2D(ShadowDepthSamp2, ShadowTex.xy + float2(-g_TexOffsetX, -g_TexOffsetY)).rg;
        otherDepth += tex2D(ShadowDepthSamp2, ShadowTex.xy + float2(0, -g_TexOffsetY)).rg;
        otherDepth += tex2D(ShadowDepthSamp2, ShadowTex.xy + float2(g_TexOffsetX, -g_TexOffsetY)).rg;

        depth = depth*0.2f + otherDepth*0.1f;
    }

    {
        const float fragDepth = ShadowTex.z / ShadowTex.w;

        ShadowValue = 1;

        if (depth.r < fragDepth - 0.001f)
        {
            const float variance = depth.g - depth.r * depth.r;
            ShadowValue = variance / (variance + (fragDepth - depth.r) * (fragDepth - depth.r));
        }
    }

    //�X�y�L����
    float Sp = CalcSpecular(
        Pos,
        Normal,
        g_LightVec,
        RoughnessLightMap.r * 255
        );


    //�v�Z���ʂ���
    Out.Sp.rgb = Sp  * g_LightColor * RoughnessLightMap.g*ShadowValue;

    Out.Def.rgb =
        g_LightColor *
        max(dot(Normal, -g_LightVec), 0)
        * RoughnessLightMap.g
        *ShadowValue;

    Out.Def.a = 1;
    Out.Sp.a = 1;

    return Out;
}


technique DirLightShadowVariance
{
    pass P0
    {
        AlphaBlendEnable = true;
        BlendOp = Add;
        SrcBlend = SrcAlpha;
        DestBlend = One;
        CullMode = CCW;
        ZEnable = false;
        ZWriteEnable = false;

        PixelShader = compile ps_3_0 PS_DirLightShadowVariance();
    }
}

//------------------------------------------------------
//		�_����
//------------------------------------------------------

VS_PointLight_OUTPUT VS_PointLight(float4 pos : POSITION)
{
    VS_PointLight_OUTPUT Out = (VS_PointLight_OUTPUT)0;

    //���[���h�ϊ�
    Out.Pos = mul(pos, g_W_mat);

    //�ˉe��Ԃ܂�
    Out.Pos = mul(Out.Pos, g_VP_mat);

    //�v�Z�p
    Out.pXYW.xy = Out.Pos.xy;
    Out.pXYW.z = Out.Pos.w;

    return Out;
}

PS_Light_OUTPUT PS_PointLight(VS_PointLight_OUTPUT In)
{
    const float2 Tex = In.pXYW.xy / In.pXYW.z * float2(0.5, -0.5) + float2(0.5, 0.5);
    const float4 NormalDepth = tex2D(NormalDepthSamp, Tex);

    const float3 Normal = CreateNormal(NormalDepth.rg * 2.0f - 1.0f);
    const float3 Pos = CalcViewPosition(Tex, NormalDepth.zw);
    const float2 RoughnessLightMap = tex2D(RoughnessLightMapSamp, Tex).rg;

    PS_Light_OUTPUT Out;

    //�����ւ̃x�N�g��
    float3 PosToLight = g_LightPos - Pos;

    //�����ɂ�錸�����Z�o
    float L = length(PosToLight) / g_LightSize;
    L = min(L, 1);
    L = 1 - L;
    L *= L;

    //�@���v�Z
    PosToLight = normalize(PosToLight);
    L *= max(dot(PosToLight, Normal), 0);

    //�X�y�L����
    float Sp = CalcSpecular(
            Pos,
            Normal,
            -PosToLight,
            RoughnessLightMap.r * 255
        );

    //�v�Z���ʂ���
    Out.Sp.rgb = Sp * g_LightColor * L;
    Out.Def.rgb = g_LightColor * L;

    Out.Def.a = 1;
    Out.Sp.a = 1;

    return Out;
}

technique PointLight
{
    pass P0
    {
        AlphaBlendEnable = true;
        BlendOp = Add;
        SrcBlend = SrcAlpha;
        DestBlend = One;
        CullMode = CW;
        ZEnable = false;
        ZWriteEnable = false;

        VertexShader = compile vs_2_0 VS_PointLight();
        PixelShader = compile ps_2_0 PS_PointLight();
    }
}


//------------------------------------------------------
//		�X�|�b�g���C�g
//------------------------------------------------------

VS_SpotLight_OUTPUT VS_SpotLight(float4 pos : POSITION)
{
    VS_SpotLight_OUTPUT Out = (VS_SpotLight_OUTPUT)0;

    //���[���h�ϊ�
    Out.Pos = mul(pos, g_W_mat);

    //�ˉe��Ԃ܂�
    Out.Pos = mul(Out.Pos, g_VP_mat);

    //�v�Z�p
    Out.pXYW.xy = Out.Pos.xy;
    Out.pXYW.z = Out.Pos.w;

    return Out;
}


PS_Light_OUTPUT PS_SpotLight(VS_SpotLight_OUTPUT In)
{
    const float2 Tex = In.pXYW.xy / In.pXYW.z * float2(0.5, -0.5) + float2(0.5, 0.5);
    const float4 NormalDepth = tex2D(NormalDepthSamp, Tex);

    const float3 Normal = CreateNormal(NormalDepth.rg * 2.0f - 1.0f);
    const float3 Pos = CalcViewPosition(Tex, NormalDepth.zw);
    const float2 RoughnessLightMap = tex2D(RoughnessLightMapSamp, Tex).rg;

    PS_Light_OUTPUT Out;

    //�����ւ̃x�N�g��
    float3 PosToLight = g_LightPos - Pos;

    //�����ɂ�錸�����Z�o
    float L = 1;

    float an = 1 - dot(g_LightVec, normalize(-PosToLight));

    an *= g_LightMaxAngle;

    if (an > 1)
    {
        discard;
    }

    an = 1 - an;
    L *= an;


    //�@���v�Z
    PosToLight = normalize(PosToLight);
    L *= max(dot(PosToLight, Normal), 0);

    //�X�y�L����
    float Sp = CalcSpecular(
        Pos,
        Normal,
        -PosToLight,
        RoughnessLightMap.r * 255
        );

    //�v�Z���ʂ���
    Out.Sp.rgb = Sp * g_LightColor * L;
    Out.Def.rgb = g_LightColor * L;

    Out.Def.a = 1;
    Out.Sp.a = 1;

    return Out;
}

technique SpotLight
{
    pass P0
    {
        AlphaBlendEnable = true;
        BlendOp = Add;
        SrcBlend = SrcAlpha;
        DestBlend = One;
        CullMode = CW;
        ZEnable = false;
        ZWriteEnable = false;

        VertexShader = compile vs_2_0 VS_SpotLight();
        PixelShader = compile ps_2_0 PS_SpotLight();
    }
}

//------------------------------------------------------
//		�X�|�b�g���C�g(�e����)
//------------------------------------------------------


PS_Light_OUTPUT PS_SpotLightShadow(VS_SpotLight_OUTPUT In)
{
    

    const float2 Tex = In.pXYW.xy / In.pXYW.z * float2(0.5, -0.5) + float2(0.5, 0.5);
    const float4 NormalDepth = tex2D(NormalDepthSamp, Tex);

    const float3 Normal = CreateNormal(NormalDepth.rg * 2.0f - 1.0f);
    const float3 Pos = CalcViewPosition(Tex, NormalDepth.zw);
    const float2 RoughnessLightMap = tex2D(RoughnessLightMapSamp, Tex).rg;

    PS_Light_OUTPUT Out;

    //�e�v�Z
    {
        float4 ShadowTex;

        ShadowTex = mul(mul(float4(Pos, 1), g_V_inv_mat), g_Shadow_VP_mat);
        ShadowTex.xy = ShadowTex.xy / ShadowTex.w;
        ShadowTex.y = -ShadowTex.y;
        ShadowTex.xy = 0.5f * ShadowTex.xy + 0.5f;

        if (tex2D(ShadowDepthSamp, ShadowTex.xy).r < ShadowTex.z - 0.005f)
        {
            discard;
        }
    }


    //�����ւ̃x�N�g��
    float3 PosToLight = g_LightPos - Pos;

    //�����ɂ�錸�����Z�o
    float L = 1;

    float an = 1 - dot(g_LightVec, normalize(-PosToLight));

    an *= g_LightMaxAngle;

    if (an > 1)
    {
        discard;
    }

    an = 1 - an;
    L *= an;


    //�@���v�Z
    PosToLight = normalize(PosToLight);
    L *= max(dot(PosToLight, Normal), 0);

    //�X�y�L����
    float Sp = CalcSpecular(
        Pos,
        Normal,
        -PosToLight,
        RoughnessLightMap.r * 255
        );

    //�v�Z���ʂ���
    Out.Sp.rgb = Sp * g_LightColor * L;
    Out.Def.rgb = g_LightColor * L;

    Out.Def.a = 1;
    Out.Sp.a = 1;

    return Out;
}

technique SpotLightShadow
{
    pass P0
    {
        AlphaBlendEnable = true;
        BlendOp = Add;
        SrcBlend = SrcAlpha;
        DestBlend = One;
        CullMode = CW;
        ZEnable = false;
        ZWriteEnable = false;

        VertexShader = compile vs_2_0 VS_SpotLight();
        PixelShader = compile ps_3_0 PS_SpotLightShadow();
    }
}




//------------------------------------------------------
//		�������C�e�B���O
//------------------------------------------------------

PS_Light_OUTPUT PS_HemiLight(PS_DirLight_INPUT In)
{
    const float4 NormalDepth = tex2D(NormalDepthSamp, In.Tex);

    const float3 Normal = CreateNormal(NormalDepth.rg * 2.0f - 1.0f);
    const float3 Pos = CalcViewPosition(In.Tex, NormalDepth.zw);
    const float2 RoughnessLightMap = tex2D(RoughnessLightMapSamp, In.Tex).rg;

    PS_Light_OUTPUT Out;

    const float UpDot = dot(Normal, g_LightVec);


    if (UpDot > 0)
    {
        Out.Def.rgb = UpDot * g_LightColor;

        Out.Sp.rgb = 
            CalcSpecular(
                Pos,
                Normal,
                -g_LightVec,
                RoughnessLightMap.r * 255
            )  * g_LightColor;
    }
    else
    {
        Out.Def.rgb = -UpDot * g_LightGroundColor;

        Out.Sp.rgb =
            CalcSpecular(
                Pos,
                Normal,
                g_LightVec,
                RoughnessLightMap.r * 255
            )  * g_LightGroundColor;
    }

    Out.Def.a = 1;
    Out.Sp.a = 1;

    return Out;
}


technique HemiLight
{
    pass P0
    {
        AlphaBlendEnable = true;
        BlendOp = Add;
        SrcBlend = SrcAlpha;
        DestBlend = One;
        CullMode = CCW;
        ZEnable = false;
        ZWriteEnable = false;

        PixelShader = compile ps_2_0 PS_HemiLight();
    }
}


//------------------------------------------------------
//		�A���r�G���g���C�g
//------------------------------------------------------

PS_Light_OUTPUT PS_AmbientLight()
{
    PS_Light_OUTPUT Out;

    Out.Def.rgb = g_LightColor;
    Out.Def.a = 1;

    Out.Sp = 0;

    return Out;
}


technique AmbientLight
{
    pass P0
    {
        AlphaBlendEnable = true;
        BlendOp = Add;
        SrcBlend = SrcAlpha;
        DestBlend = One;
        CullMode = CCW;
        ZEnable = false;
        ZWriteEnable = false;

        PixelShader = compile ps_2_0 PS_AmbientLight();
    }
}


//------------------------------------------------------
//		�A���r�G���g���C�g�i�I�N�����[�W�������j
//------------------------------------------------------

//PS_Light_OUTPUT PS_AmbientOcclusionLight(PS_AmbientLight_INPUT In)
//{
//    //�A�����A���G���W��ver
//    PS_Light_OUTPUT Out;
//
//    const float4 NormalDepth = tex2D(NormalDepthSamp, In.Tex);
//
//    float  Occlusion = 0;
//    float3 v1, v2, Pos, Normal;
//    float4 Proj, SampNormalDepth1, SampNormalDepth2;
//   
//    Normal = CreateNormal(NormalDepth.rg * 2.0f - 1.0f);
//    Pos = CalcViewPosition(In.Tex, NormalDepth.zw);
//
//    for (uint i = 0; i < g_OcclusionSampNum; ++i)
//    {
//        Proj.xyz = Pos + g_OcclusionSampVec[i] * g_OcclusionSampSize;
//        Proj.w = 1;
//        Proj = mul(Proj, g_P_mat);
//
//        v1.xy = Proj.xy / Proj.w * float2(0.5f, -0.5f) + float2(0.5f, 0.5f);
//        SampNormalDepth1 = tex2D(NormalDepthSamp, v1.xy);
//
//        v1 = CalcViewPosition(v1.xy, SampNormalDepth1.zw);
//        v1 -= Pos;
//        v1 = normalize(v1);
//
//        Proj.xyz = Pos - g_OcclusionSampVec[i] * g_OcclusionSampSize;
//        Proj.w = 1;
//        Proj = mul(Proj, g_P_mat);
//
//        v2.xy = Proj.xy / Proj.w * float2(0.5f, -0.5f) + float2(0.5f, 0.5f);
//        SampNormalDepth2 = tex2D(NormalDepthSamp, v2.xy);
//
//        v2 = CalcViewPosition(v2.xy, SampNormalDepth2.zw);
//        v2 -= Pos;
//        v2 = normalize(v2);
//
//        if (dot(Normal, v1) > 0 &&
//            dot(Normal, v2) > 0 &&
//            abs(SampNormalDepth1.z - NormalDepth.z) < g_OcclusionCalcDiffZ &&
//            abs(SampNormalDepth2.z - NormalDepth.z) < g_OcclusionCalcDiffZ 
//            )
//        {
//            Occlusion += dot(v1, v2) * 0.5f + 0.5f;
//        }
//    }
//
//    Occlusion *= g_OcclusionRatio;
//
//    Out.Def.rgb = g_LightColor * pow(1 - Occlusion, 3);
//    Out.Def.a = 1;
//    Out.Sp = 0;
//
//    return Out;
//}

//PS_Light_OUTPUT PS_AmbientOcclusionLight(PS_AmbientLight_INPUT In)
//{
//    //�A�����A���G���W��ver2
//    PS_Light_OUTPUT Out;
//    
//    const float4 NormalDepth = tex2D(NormalDepthSamp, In.Tex);
//    const float3 
//        Normal = CreateNormal(NormalDepth.rg * 2.0f - 1.0f),
//        Pos = CalcViewPosition(In.Tex, NormalDepth.zw);
//
//    float4 SampParam;
//    float3 v[2];
//    float  Occlusion = 0;
//    float  Signed;
//    int    j;
//    
//    for (uint i = 0; i < g_OcclusionSampNum; ++i)
//    {
//        Signed = -1;
//        for (j = 0; j < 2; ++j)
//        {
//            //��͕`��s�N�Z���ʒu
//            SampParam.xyz = Pos;
//            SampParam.w = 1;
//
//            //�r���[��ԏ�ŏ������炷
//            SampParam.xy += g_OcclusionSampVec[i] * g_OcclusionSampSize * Signed;
//
//            //�e�N�X�`����Ԃ܂ŕϊ�
//            SampParam = mul(SampParam, g_P_mat);
//            SampParam.xy = SampParam.xy / SampParam.w * float2(0.5f, -0.5f) + float2(0.5f, 0.5f);
//
//            //ZW�l�𓾂�
//            SampParam.zw = tex2D(NormalDepthSamp, SampParam.xy).zw;
//
//            //�T���v�����O�_�ւ̃x�N�g�����Z�o
//            v[j] = CalcViewPosition(SampParam.xy, SampParam.zw) - Pos;
//
//            //�����𐳂�
//            Signed = 1;
//        }
//
//        //(�x�N�g�����@���ƂX�O�x�ȏ�)or(�T���v�����O�_����O�߂���)�ꍇ�͌v�Z���Ȃ�
//        /*
//
//        if (   (v[0].z > 0  || v[1].z > 0)
//            || (v[0].z < -1 || v[1].z < -1)
//            )
//        {
//            continue;
//        }
//
//        */
//
//        if (dot(v[0], Normal) < 0 || dot(v[1], Normal) < 0
//            || (v[0].z < -1 || v[1].z < -1)
//            )
//        {
//            continue;
//        }
//
//        //�x�N�g���Ԃ̊p�x����Օ��x���v�Z
//        Occlusion += dot(v[0], v[1]) / (length(v[0])*length(v[1]))*0.5f + 0.5f;
//    }
//    
//    Occlusion *= g_OcclusionRatio;
//    
//    Out.Def.rgb = g_LightColor * 1 - Occlusion;
//    Out.Def.a = 1;
//    Out.Sp = 0;
//    
//    return Out;
//}


PS_Light_OUTPUT PS_AmbientOcclusionLight(PS_AmbientLight_INPUT In)
{
    //�Օ��p�x����ver

    PS_Light_OUTPUT Out;

    const float4 NormalDepth = tex2D(NormalDepthSamp, In.Tex);
    const float3 
        Normal = CreateNormal(NormalDepth.rg * 2.0f - 1.0f),
        Pos = CalcViewPosition(In.Tex, NormalDepth.zw);

    float4 SampParam;
    float3 Vec;
    float  Occlusion = 0;
    float  Dot, SampLen;

    for (uint i = 0; i < g_OcclusionSampNum; ++i)
    {
        //��͕`��s�N�Z���ʒu
        SampParam.xyz = Pos;
        SampParam.w = 1;

        //�r���[��ԏ�ŏ������炷
        SampParam.xy += g_OcclusionSampVec[i] * g_OcclusionSampSize;

        //�e�N�X�`����Ԃ܂ŕϊ�
        SampParam = mul(SampParam, g_P_mat);
        SampParam.xy = SampParam.xy / SampParam.w * float2(0.5f, -0.5f) + float2(0.5f, 0.5f);

        //ZW�l�𓾂�
        SampParam.zw = tex2D(NormalDepthSamp, SampParam.xy).zw;

        //�r���[��ԏ�ł̃T���v�����O�_�̍��W�����߂�
        Vec = CalcViewPosition(SampParam.xy, SampParam.zw); 

        //�T���v�����O�_�ւ̃x�N�g�����Z�o
        Vec -= Pos;

        //�T���v�����O�_�ւ̋����̏d�݂��Z�o
        SampLen = length(Vec);

        //�Օ��x�����Z
        Dot = dot(Vec, Normal) / SampLen * (1 - min(SampLen / (g_OcclusionSampSize * 2.0f), 1));
        
        if (Dot > 0)
        {
            Occlusion += Dot;
        }
    }

    Occlusion *= g_OcclusionRatio;

    Out.Def.rgb = g_LightColor * pow(1 - Occlusion, 5);
    Out.Def.a = 1;
    Out.Sp = 0;

    return Out;
}

//PS_Light_OUTPUT PS_AmbientOcclusionLight(PS_AmbientLight_INPUT In)
//{
//    //���̐ϐϕ�ver
//
//    PS_Light_OUTPUT Out;
//
//    const float4 NormalDepth = tex2D(NormalDepthSamp, In.Tex);
//    const float3
//        Normal = CreateNormal(NormalDepth.rg * 2.0f - 1.0f),
//        Pos = CalcViewPosition(In.Tex, NormalDepth.zw);
//
//    const float pi_div2 = 3.141592654f * 0.5f;
//
//    float4 SampParam;
//    float3 Vec;
//    float  Occlusion = 0;
//    float  Length;
//    float  SphereZ;
//
//    
//
//    for (uint i = 0; i < g_OcclusionSampNum; ++i)
//    {
//        //��͕`��s�N�Z���ʒu
//        SampParam.xyz = Pos;
//        SampParam.w = 1;
//
//        //�r���[��ԏ�ŏ������炷
//        SampParam.xy += g_OcclusionSampVec[i] * g_OcclusionSampSize;
//
//        //�e�N�X�`����Ԃ܂ŕϊ�
//        SampParam = mul(SampParam, g_P_mat);
//        SampParam.xy = SampParam.xy / SampParam.w * float2(0.5f, -0.5f) + float2(0.5f, 0.5f);
//
//        //ZW�l�𓾂�
//        SampParam.zw = tex2D(NormalDepthSamp, SampParam.xy).zw;
//
//        //���̌��݂��Z�o
//        {
//            float a = length(g_OcclusionSampVec[i])*g_OcclusionSampSize;
//            float b = a / g_OcclusionSampSize;
//            float c = 1 - b;
//
//            SphereZ = sin(c * pi_div2) * g_OcclusionSampSize;
//        }
//
//        //SphereZ = sin((1 - (length(g_OcclusionSampVec[i])*g_OcclusionSampSize / g_OcclusionSampSize)) * pi_div2) * g_OcclusionSampSize;
//        //SphereZ = 1;
//
//        //�T���v�����O�_����̋���
//        Length = Pos.z - SampParam.z;
//
//        //���̒��ɓ����Ă���Ȃ�Օ��x���v�Z
//        if (abs(Length) < SphereZ)
//        {
//            Occlusion += Length / SphereZ;
//        }
//    }
//    
//    Occlusion = max(0, Occlusion);
//    Occlusion *= g_OcclusionRatio;
//
//    Out.Def.rgb = g_LightColor * pow((1 - Occlusion), 1);
//    Out.Def.a = 1;
//    Out.Sp = 0;
//
//    return Out;
//}

//PS_Light_OUTPUT PS_AmbientOcclusionLight(PS_AmbientLight_INPUT In)
//{
//    //�I���W�i���m����ver�@(�ŏI�I�Ƀu���[����ׂ�)
//
//    PS_Light_OUTPUT Out;
//
//    const float4 NormalDepth = tex2D(NormalDepthSamp, In.Tex);
//    const float3
//        Normal = CreateNormal(NormalDepth.rg * 2.0f - 1.0f),
//        Pos = CalcViewPosition(In.Tex, NormalDepth.zw);
//
//    const float MyDepth = NormalDepth.z / NormalDepth.w;
//
//    float4 SampParam;
//    float  Occlusion = 0;
//    float2 OtherDepth;
//    float2 TotalDepth = float2(0, 0);
//
//    for (uint i = 0; i < g_OcclusionSampNum; ++i)
//    {
//        //��͕`��s�N�Z���ʒu
//        SampParam.xyz = Pos;
//        SampParam.w = 1;
//
//        //�r���[��ԏ�ŏ������炷
//        SampParam.xy += g_OcclusionSampVec[i] * g_OcclusionSampSize;
//
//        //�e�N�X�`����Ԃ܂ŕϊ�
//        SampParam = mul(SampParam, g_P_mat);
//        SampParam.xy = SampParam.xy / SampParam.w * float2(0.5f, -0.5f) + float2(0.5f, 0.5f);
//
//        //Z�l�𓾂�
//        OtherDepth = tex2D(NormalDepthSamp, SampParam.xy).zw;
//        OtherDepth.x /= OtherDepth.y;
//
//        TotalDepth.x += OtherDepth.x;
//        TotalDepth.y += OtherDepth.x*OtherDepth.x;
//    }
//
//    //���ω�
//    TotalDepth *= g_OcclusionRatio;
//    TotalDepth = (TotalDepth * 0.8f) + float2(MyDepth, MyDepth*MyDepth) * 0.2f;
//
//    const float variance = TotalDepth.g - TotalDepth.r * TotalDepth.r;
//    Occlusion = variance / (variance + (MyDepth - TotalDepth.r) * (MyDepth - TotalDepth.r));
//
//    Out.Def.rgb = g_LightColor * Occlusion;
//    Out.Def.a = 1;
//    Out.Sp = 0;
//
//    return Out;
//}

technique AmbientOcclusionLight
{
    pass P0
    {
        AlphaBlendEnable = true;
        BlendOp = Add;
        SrcBlend = SrcAlpha;
        DestBlend = One;
        CullMode = CCW;
        ZEnable = false;
        ZWriteEnable = false;

        PixelShader = compile ps_3_0 PS_AmbientOcclusionLight();
    }
}

//------------------------------------------------------
//		Z�l��������
//------------------------------------------------------

VS_WriteZ_OUTPUT VS_WriteZ(float4 pos : POSITION, float2 Tex : TEXCOORD0)
{
    VS_WriteZ_OUTPUT Out;

    //���[���h�ϊ�
    Out.Pos = mul(mul(pos, g_W_mat), g_Shadow_VP_mat);
    Out.ZW = Out.Pos.zw;
    Out.Tex = Tex;

    return Out;
}

float4 PS_WriteZ(VS_WriteZ_OUTPUT In) :COLOR0
{
    float4 Out = float4(0, 0, 0, 0);

    if (tex2D(DecaleSamp, In.Tex).a < 0.1f)
    {
        discard;
    }

    Out.r = In.ZW.r / In.ZW.g;

    return Out;
}

technique WriteZ
{
    pass P0
    {
        AlphaBlendEnable = false;
        BlendOp = Add;
        SrcBlend = SrcAlpha;
        DestBlend = One;
        CullMode = CCW;
        ZEnable = true;
        ZWriteEnable = true;

        VertexShader = compile vs_2_0 VS_WriteZ();
        PixelShader = compile ps_2_0 PS_WriteZ();
    }
}

float4 PS_WriteZf2(VS_WriteZ_OUTPUT In) :COLOR0
{
    float4 Out = float4(0, 0, 0, 0);

    if (tex2D(DecaleSamp, In.Tex).a < 0.1f)
    {
        discard;
    }

    Out.r = In.ZW.r / In.ZW.g;
    Out.g = Out.r*Out.r;

    return Out;
}

technique WriteZf2
{
    pass P0
    {
        AlphaBlendEnable = false;
        BlendOp = Add;
        SrcBlend = SrcAlpha;
        DestBlend = One;
        CullMode = CCW;
        ZEnable = true;
        ZWriteEnable = true;

        VertexShader = compile vs_2_0 VS_WriteZ();
        PixelShader = compile ps_2_0 PS_WriteZf2();
    }
}

//-----------------------------------------------------------------
//�@�@�@�N���A
//-----------------------------------------------------------------

PS_Light_OUTPUT PS_Clear()
{
    PS_Light_OUTPUT Out;

    Out.Def = float4(0, 0, 0, 0);
    Out.Sp = float4(0, 0, 0, 0);

    return Out;
}


technique Clear
{
    pass P0
    {
        AlphaBlendEnable = false;
        BlendOp = Add;
        SrcBlend = SrcAlpha;
        DestBlend = InvSrcAlpha;
        CullMode = none;
        ZEnable = false;
        ZWriteEnable = false;

        PixelShader = compile ps_2_0 PS_Clear();
    }
}


//-----------------------------------------------------------------
//�@�@�@�N���AZ
//-----------------------------------------------------------------

float4 PS_ClearZ() :COLOR0
{
    float4 Out;

    Out = 1;

    return Out;
}


technique ClearZ
{
    pass P0
    {
        AlphaBlendEnable = false;
        BlendOp = Add;
        SrcBlend = SrcAlpha;
        DestBlend = InvSrcAlpha;
        CullMode = none;
        ZEnable = false;
        ZWriteEnable = false;

        PixelShader = compile ps_2_0 PS_ClearZ();
    }
}
