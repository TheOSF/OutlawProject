

//------------------------------------------------------
//		���֘A
//------------------------------------------------------
float4x4 g_VP_mat;	//	���e�ϊ��s��
float4x4 g_W_mat;	    //���[���h�ϊ�

float4   g_Color;
float3   g_Diffuse = float3(1, 1, 1);
float3   g_Specular = float3(1, 1, 1);//�X�y�L�����[

float3   g_HDR_Color; //HDR�J���[

//float    g_Outline_Size = 0.1f;
//float4   g_Outline_Color = float4(2, 1.5f, 0, 0);


float    g_Outline_Size = 0.04f;
float4   g_Outline_Color = float4(0, 0, 0, 1);


//------------------------------------------------------
//		���֘A (�t�H�[���[�h�`��p)
//------------------------------------------------------

//���s��
float3 FR_DirLightVec;
float3 FR_DirLightColor;

//����
float3 FR_AmbientColor;
float3 FR_SkyColor;
float3 FR_GroundColor;


//------------------------------------------------------
//		�\�t�g�p�[�e�B�N���`��p
//------------------------------------------------------

float g_SoftDepth = 0.08f; //�����ɂ��n�߂鋗��

//------------------------------------------------------
//		�e�N�X�`���T���v���[	
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

texture NormalMap;
sampler NormalSamp = sampler_state
{
    Texture = <NormalMap>;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    MipFilter = LINEAR;

    AddressU = Wrap;
    AddressV = Wrap;
};

texture SpecularMap;
sampler SpecularSamp = sampler_state
{
    Texture = <SpecularMap>;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    MipFilter = LINEAR;

    AddressU = Wrap;
    AddressV = Wrap;
};

texture HeightMap;
sampler HeightSamp = sampler_state
{
    Texture = <HeightMap>;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    MipFilter = LINEAR;

    AddressU = Wrap;
    AddressV = Wrap;
};


//------------------------------------------------------
//		���C�g�o�b�t�@�T���v���[	
//------------------------------------------------------

texture DiffuseLightMap;
sampler DiffuseLightSamp = sampler_state
{
    Texture = <DiffuseLightMap>;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    MipFilter = LINEAR;

    AddressU = Wrap;
    AddressV = Wrap;
};

texture SpecuarLightMap;
sampler SpecuarLightSamp = sampler_state
{
    Texture = <SpecuarLightMap>;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    MipFilter = LINEAR;

    AddressU = Wrap;
    AddressV = Wrap;
};

//------------------------------------------------------
//		�@���E�[�x�o�b�t�@�T���v���[	
//------------------------------------------------------

texture NormalDepthMap;
sampler NormalDepthSamp = sampler_state
{
    Texture = <NormalDepthMap>;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    MipFilter = LINEAR;

    AddressU = Clamp;
    AddressV = Clamp;
};

//------------------------------------------------------
//		���_Input
//------------------------------------------------------

struct VS_INPUT
{
    float4 Pos		: POSITION;
    float4 Color	: COLOR0;

    float2 Tex		: TEXCOORD0;
};


//------------------------------------------------------
//		���_Output
//------------------------------------------------------

struct VS_OUT_DeffLight
{
    float4 Pos		: POSITION;
    float4 Color	: COLOR0;

    float2 Tex		: TEXCOORD0;
    float4 pPos		: TEXCOORD2;
};

struct VS_OUT_Forward
{
    float4 Pos		: POSITION;
    float4 Color    : COLOR0;
    float2 Tex		: TEXCOORD0;
};

struct VS_OUT_SoftParticle
{
    float4 Pos		: POSITION;
    float4 Color    : COLOR0;
    float2 Tex		: TEXCOORD0;
    float4 pPos     : TEXCOORD1;
};

struct VS_OUT_ForwardHDRPolygon
{
    float4 Pos		: POSITION;
    float4 Color    : COLOR0;
    float4 HDR      : COLOR1;
    float2 Tex		: TEXCOORD0;
};

//------------------------------------------------------
//      �s�N�Z��Output
//------------------------------------------------------
struct PS_OUT_DeffLight
{
    float4 Color    : COLOR0;
    float4 High     : COLOR1;
};

//********************************************************************
//
//		�A�E�g���C��
//
//********************************************************************

// ctl kc ku

//float4 VS_OutLine(
//    float4 Pos    :POSITION,
//    float3 Normal : NORMAL) :POSITION
//{
//    float4 Out = (float4)0;
//
//    float3 WorldNormal = mul(float4(Normal, 0), g_W_mat).xyz;
//
//    Out = mul(Pos, g_W_mat);
//    Out.xyz += normalize(WorldNormal) * g_Outline_Size;
//
//    Out = mul(Out, g_VP_mat);
//
//    return Out;
//}
//
//PS_OUT_DeffLight PS_OutLine()
//{
//    PS_OUT_DeffLight Out = (PS_OUT_DeffLight)0;
//
//    Out.Color = g_Outline_Color;
//
//    Out.High = g_Outline_Color - 1;
//    Out.High.a = 1;
//
//    return Out;
//}


float4 VS_OutLine(
    float4 Pos    : POSITION,
    float3 Normal : NORMAL) :POSITION
{
    float4 Out = (float4)0;

    float3 WorldNormal = mul(float4(Normal, 0), g_W_mat).xyz;

    Out = mul(Pos, g_W_mat);
    Out.xyz += normalize(WorldNormal) * g_Outline_Size;

    Out = mul(Out, g_VP_mat);

    return Out;
}

PS_OUT_DeffLight PS_OutLine()
{
    PS_OUT_DeffLight Out = (PS_OUT_DeffLight)0;

    Out.Color = g_Outline_Color;

    Out.High = g_Outline_Color - 1;
    Out.High.a = 1;

    return Out;
}

//********************************************************************
//
//		�f�B�t�@�[�h���C�e�B���O�����_�����O
//
//********************************************************************
//------------------------------------------------------
//		���_�V�F�[�_�[	
//------------------------------------------------------
VS_OUT_DeffLight VS_DeffLightRender(VS_INPUT In)
{
    VS_OUT_DeffLight Out = (VS_OUT_DeffLight)0;

    //���[���h���ˉe�ϊ�
    Out.Pos = mul(mul(In.Pos, g_W_mat), g_VP_mat);

    //�e�N�X�`�����W
    Out.Tex = In.Tex;

    //���_�F
    Out.Color = 1.0f;

    //�ˉe�ϊ��ςݍ��W��ۑ�
    Out.pPos = Out.Pos;

    return Out;
}

//------------------------------------------------------
//		�s�N�Z���V�F�[�_�[	
//------------------------------------------------------
PS_OUT_DeffLight PS_DeffLightRender(VS_OUT_DeffLight In)
{
    //�o�͕ϐ�
    PS_OUT_DeffLight Out;

    //�X�N���[����Ԃ��e�N�X�`�����W��
    const float2 ScreenTex = In.pPos.xy / In.pPos.w * float2(0.5, -0.5) + float2(0.5, 0.5);

    //  �g�U����
    {
        Out.Color = In.Color * tex2D(DecaleSamp, In.Tex);
        Out.Color.rgb *= tex2D(DiffuseLightSamp, ScreenTex).rgb * 2;
    }

    //  ���ʔ���
    {
        Out.Color.rgb += tex2D(SpecularSamp, In.Tex).rgb * 2 * tex2D(SpecuarLightSamp, ScreenTex).rgb;
    }

    //���P�x�����𒊏o
    Out.High.rgb = max(Out.Color.rgb - 1.0f, 0);
    Out.High.rgb += g_HDR_Color;
    
    Out.High.a = 1;



    return Out;
}


//------------------------------------------------------
//		�s�N�Z���V�F�[�_�[	
//------------------------------------------------------
PS_OUT_DeffLight PS_DeffLightNoSpecularRender(VS_OUT_DeffLight In)
{
    //�o�͕ϐ�
    PS_OUT_DeffLight Out;

    //�X�N���[����Ԃ��e�N�X�`�����W��
    const float2 ScreenTex = In.pPos.xy / In.pPos.w * float2(0.5, -0.5) + float2(0.5, 0.5);

    //  �g�U����
    {
        Out.Color = In.Color * tex2D(DecaleSamp, In.Tex);
        Out.Color.rgb *= tex2D(DiffuseLightSamp, ScreenTex).rgb * 2.0f;
    }

    //  ���ʔ���
    {
        Out.Color.rgb += tex2D(SpecuarLightSamp, ScreenTex).rgb * g_Specular * 2.0f;
    }

    //���P�x�����𒊏o
    Out.High.rgb = max(Out.Color.rgb - 1.0f, 0);
    Out.High.rgb += g_HDR_Color;

    Out.High.a = 1;

    return Out;
}


//------------------------------------------------------
//		�s�N�Z���V�F�[�_�[	
//------------------------------------------------------
PS_OUT_DeffLight PS_DeffLightNoTextureRender(VS_OUT_DeffLight In)
{
    //�o�͕ϐ�
    PS_OUT_DeffLight Out;

    //�X�N���[����Ԃ��e�N�X�`�����W��
    const float2 ScreenTex = In.pPos.xy / In.pPos.w * float2(0.5, -0.5) + float2(0.5, 0.5);

    //  �g�U����
    {
        Out.Color = In.Color;
        Out.Color.rgb *= tex2D(DiffuseLightSamp, ScreenTex).rgb * 2.0f;
    }

    //  ���ʔ���
    {
        Out.Color.rgb += tex2D(SpecuarLightSamp, ScreenTex).rgb * g_Specular * 2.0f;
    }

    //���P�x�����𒊏o
    Out.High.rgb = max(Out.Color.rgb - 1.0f, 0);
    Out.High.rgb += g_HDR_Color;

    Out.High.a = 1;

    return Out;
}


//------------------------------------------------------
//		�ʏ�`��e�N�j�b�N
//------------------------------------------------------
technique DeffLight
{
    pass P0
    {
        AlphaBlendEnable = true;
        BlendOp = Add;
        SrcBlend = SrcAlpha;
        DestBlend = InvSrcAlpha;
        CullMode = CCW;
        ZEnable = true;

        VertexShader = compile vs_2_0 VS_DeffLightRender();
        PixelShader = compile ps_2_0 PS_DeffLightRender();
    }
}


technique DeffLightNoSp
{
    /*
    pass P0
    {
        AlphaBlendEnable = true;
        BlendOp = Add;
        SrcBlend = SrcAlpha;
        DestBlend = InvSrcAlpha;
        CullMode = CW;
        ZEnable = true;

        VertexShader = compile vs_2_0 VS_OutLine();
        PixelShader = compile ps_2_0 PS_OutLine();
    }
    */

    pass P0
    {
        AlphaBlendEnable = true;
        BlendOp = Add;
        SrcBlend = SrcAlpha;
        DestBlend = InvSrcAlpha;
        CullMode = CCW;
        ZEnable = true;

        VertexShader = compile vs_2_0 VS_DeffLightRender();
        PixelShader = compile ps_2_0 PS_DeffLightNoSpecularRender();
    }
}


technique DeffLightNoTex
{
    pass P0
    {
        AlphaBlendEnable = true;
        BlendOp = Add;
        SrcBlend = SrcAlpha;
        DestBlend = InvSrcAlpha;
        CullMode = CCW;
        ZEnable = true;

        VertexShader = compile vs_2_0 VS_DeffLightRender();
        PixelShader = compile ps_2_0 PS_DeffLightNoTextureRender();
    }
}


//------------------------------------------------------
//		HDR�t�H���[�h�����_�����O
//------------------------------------------------------


PS_OUT_DeffLight PS_HDR_Forward(VS_OUT_DeffLight In)
{
    //�o�͕ϐ�
    PS_OUT_DeffLight Out;

    //  �g�U����
    {
        Out.Color = In.Color * tex2D(DecaleSamp, In.Tex) * g_Color;
    }

    //���P�x
    Out.High = In.Color * float4(g_HDR_Color, 1);

    return Out;
}


technique HDR_Forward
{
    pass P0
    {
        AlphaBlendEnable = true;
        BlendOp = Add;
        SrcBlend = SrcAlpha;
        DestBlend = InvSrcAlpha;
        CullMode = CCW;
        ZEnable = true;

        VertexShader = compile vs_2_0 VS_DeffLightRender();
        PixelShader = compile ps_2_0 PS_HDR_Forward();
    }
}

//------------------------------------------------------
//		HDR�t�H���[�h�|���S�������_�����O
//------------------------------------------------------

VS_OUT_ForwardHDRPolygon VS_HDR_ForwardPolygon(
    float4 Pos		: POSITION,
    float4 Color : COLOR0,
    float4 HDR : COLOR1,
    float2 Tex : TEXCOORD0
    )
{
    VS_OUT_ForwardHDRPolygon Out = (VS_OUT_ForwardHDRPolygon)0;

    Out.Pos = mul(Pos, g_VP_mat);
    Out.Color = Color;
    Out.HDR = HDR;
    Out.Tex = Tex;

    return Out;
}


PS_OUT_DeffLight PS_HDR_ForwardPolygon(VS_OUT_ForwardHDRPolygon In)
{
    //�o�͕ϐ�
    PS_OUT_DeffLight Out;

    const float4 TexColor = tex2D(DecaleSamp, In.Tex);

    Out.Color = In.Color * TexColor;
    Out.High  = In.HDR   * TexColor;

    return Out;
}


technique HDR_ForwardPolygon
{
    pass P0
    {
        AlphaBlendEnable = true;
        BlendOp = Add;
        SrcBlend = SrcAlpha;
        DestBlend = InvSrcAlpha;
        CullMode = CCW;
        

        VertexShader = compile vs_2_0 VS_HDR_ForwardPolygon();
        PixelShader = compile ps_2_0 PS_HDR_ForwardPolygon();
    }
}

//------------------------------------------------------
//		�\�t�g�p�[�e�B�N�������_�����O
//------------------------------------------------------

VS_OUT_SoftParticle VS_SoftParticle(
    float4 Pos : POSITION,
    float2 Tex : TEXCOORD0
    )
{
    VS_OUT_SoftParticle Out = (VS_OUT_SoftParticle)0;

    Out.Pos = mul(Pos, g_VP_mat);
    Out.pPos = Out.Pos;
    Out.Color = g_Color;
    Out.Tex = Tex;

    return Out;
}


float4 PS_SoftParticle(VS_OUT_SoftParticle In) :COLOR0
{
    //�o�͕ϐ�
    float4 Out;
    float2 Depth;

    Out = In.Color * tex2D(DecaleSamp, In.Tex);

    Depth = tex2D(NormalDepthSamp, In.pPos.xy / In.pPos.w * float2(0.5, -0.5) + float2(0.5, 0.5)).zw;
    Depth.x = (Depth.x / Depth.y) - (In.pPos.z / In.pPos.w);
 
    if (Depth.x < 0)
    {
        discard;
    }

    Out.a *= min(Depth.x / g_SoftDepth, 1);

    return Out;
}


technique SoftParticle
{
    pass P0
    {
        AlphaBlendEnable = true;
        BlendOp = Add;
        SrcBlend = SrcAlpha;
        DestBlend = InvSrcAlpha;
        CullMode = CCW;

        VertexShader = compile vs_2_0 VS_SoftParticle();
        PixelShader = compile ps_2_0 PS_SoftParticle();
    }
}


//------------------------------------------------------
//		�f�o�b�O�`��p
//------------------------------------------------------

float4 VS_NoWork(float4 Pos:POSITION) :POSITION
{
    return mul(mul(Pos, g_W_mat), g_VP_mat);
}


float4 PS_ColorOnly() :COLOR0
{
    return g_Color;
}


technique ColorOnly
{
    pass P0
    {
        AlphaBlendEnable = true;
        BlendOp = Add;
        SrcBlend = SrcAlpha;
        DestBlend = InvSrcAlpha;
        CullMode = CCW;
        ZEnable = true;
        ZWriteEnable = false;

        VertexShader = compile vs_2_0 VS_NoWork();
        PixelShader = compile ps_2_0 PS_ColorOnly();
    }
}



//------------------------------------------------------
//		�t�H���[�h�`��p
//------------------------------------------------------


VS_OUT_Forward VS_ForwardRender(
    float4 Pos : POSITION,
    float2 Tex : TEXCOORD0,
    float3 Normal : NORMAL
    ) 
{
    VS_OUT_Forward Out;

    const float3 WorldNormal = normalize(mul(float4(Normal, 0), g_W_mat));

    Out.Pos = mul(mul(Pos, g_W_mat), g_VP_mat);
    Out.Tex = Tex;
    Out.Color = float4(0, 0, 0, 0);

    {
        //�n�[�t�����o�[�g
        Out.Color.rgb += FR_DirLightColor * max(dot(FR_DirLightVec, WorldNormal), 0);
    }

    {
        //����
        if (WorldNormal.y > 0)
        {
            Out.Color.rgb += FR_SkyColor * WorldNormal.y;
        }
        else
        {
            Out.Color.rgb += FR_GroundColor * -WorldNormal.y;
        }

    }

    {
        //�A���r�G���g
        Out.Color.rgb += FR_AmbientColor;
    }

    Out.Color.rgb *= 2.0f;

    {
        Out.Color.rgb *= g_Color.rgb;
        Out.Color.a = g_Color.a;
    }

    return Out;
}


float4 PS_ForwardRender(VS_OUT_Forward In) : COLOR0
{
    return tex2D(DecaleSamp, In.Tex) * In.Color;
}


technique forward
{
    pass P0
    {
        AlphaBlendEnable = true;
        BlendOp = Add;
        SrcBlend = SrcAlpha;
        DestBlend = InvSrcAlpha;
        CullMode = CCW;
        ZEnable = true;
        ZWriteEnable = true;

        VertexShader = compile vs_2_0 VS_ForwardRender();
        PixelShader = compile ps_2_0 PS_ForwardRender();
    }
}