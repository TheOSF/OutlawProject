//********************************************************************
//																									
//		�R�c�p�V�F�[�_�[		
//
//********************************************************************
//------------------------------------------------------
//		���֘A
//------------------------------------------------------

//---�r���[�֘A---//
float4x4 g_VP_mat;	
float4x4 g_V_mat;
float4x4 g_P_mat;

float3   g_CameraPos;
float3   g_CameraVec;

//---�X�N���[���֘A---//
float2   g_ScreenPixelSize;

//---���f���֘A---//
float4x4 g_W_mat;
float4 g_Color = float4(1, 1, 1, 1);

//---���s���֘A---//
float3 g_DirLightVec;
float3 g_DirLightColor;

//---�_�����֘A---//
#define MAX_POINTLIGHT 3

int    g_PointLightUseNum;
float3 g_PointLightPos  [MAX_POINTLIGHT];
float3 g_PointLightColor[MAX_POINTLIGHT];
float  g_PointLightSize [MAX_POINTLIGHT];

//---�֊s��---//
float3 g_OutLineColor = float3(0, 0, 0);
float  g_OutLineSize = 0.002f;

//------------------------------------------------------
//		�e�N�X�`���T���v���[	
//------------------------------------------------------
texture Texture;
sampler DecaleSamp = sampler_state
{
    Texture = <Texture>;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    MipFilter = NONE;

    AddressU = Wrap;
    AddressV = Wrap;
};

texture SpecularMap;
sampler SpecularSamp = sampler_state
{
    Texture = <SpecularMap>;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    MipFilter = NONE;

    AddressU = Wrap;
    AddressV = Wrap;
};

texture ToonMap;
sampler ToonSamp = sampler_state
{
    Texture = <ToonMap>;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    MipFilter = NONE;

    AddressU = CLAMP;
    AddressV = CLAMP;
};

texture ToonSpMap;
sampler ToonSpSamp = sampler_state
{
    Texture = <ToonSpMap>;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    MipFilter = NONE;

    AddressU = CLAMP;
    AddressV = CLAMP;
};


//------------------------------------------------------
//		���_�t�H�[�}�b�g
//------------------------------------------------------
struct VS_OUTPUT
{
    float4 Pos		: POSITION;
    float4 Color	: COLOR0;
    float2 Tex		: TEXCOORD0;
};

struct VS_INPUT
{
    float4 Pos    : POSITION;
    float3 Normal : NORMAL;
    float2 Tex	  : TEXCOORD0;
};

struct VP_HDR_OUTPUT
{
    float4 Pos		: POSITION;
    float4 Color	: COLOR0;
    float2 Tex		: TEXCOORD0;
    float2 pPosZW   : TEXCOORD1;
};


struct PS_HDR_OUTPUT
{
    float4 Color      : COLOR0;
    float4 HdrDepth   : COLOR1;
};

//********************************************************************
//
//		��{�R�c�V�F�[�_�[		
//
//********************************************************************
//------------------------------------------------------
//		���_�V�F�[�_�[	
//------------------------------------------------------
VS_OUTPUT VS_Basic( VS_INPUT In )
{
    VS_OUTPUT Out = (VS_OUTPUT)0;

    Out.Pos = mul(In.Pos, g_W_mat);
    Out.Pos = mul(Out.Pos, g_VP_mat);
	Out.Tex = In.Tex;
	Out.Color = 1.0f;

    return Out;
}

//------------------------------------------------------
//		�s�N�Z���V�F�[�_�[	
//------------------------------------------------------
float4 PS_Basic( VS_OUTPUT In) : COLOR
{   
	float4	OUT;
	//	�s�N�Z���F����
	OUT = In.Color * tex2D( DecaleSamp, In.Tex );

	return OUT;
}

//------------------------------------------------------
//		�ʏ�`��e�N�j�b�N
//------------------------------------------------------
technique copy
{
    pass P0
    {
		AlphaBlendEnable = true;
		BlendOp          = Add;
		SrcBlend         = SrcAlpha;
		DestBlend        = InvSrcAlpha;
		CullMode         = CCW;
		ZEnable          = true;

		VertexShader = compile vs_2_0 VS_Basic();
		PixelShader  = compile ps_2_0 PS_Basic();
    }
}



//********************************************************************
//
//		�֐�
//
//********************************************************************

//�n�[�t�����o�[�g
float CalcHalfLambert(float3 LightVec, float3 Normal)
{
    return dot(Normal, -LightVec) * 0.5f + 0.5f;
}

//�����o�[�g
float CalcLambert(float3 LightVec, float3 Normal)
{
    return max(dot(Normal, -LightVec), 0);
}

//�X�y�L����
float CalcSpecular(
    float3 Pos,
    float3 Normal,
    float3 LightVec,
    float  Power
    )
{
    float3 H = normalize(g_CameraPos - Pos) - LightVec;
    float Sp;
    H = normalize(H);;

    Sp = max(dot(Normal, H), 0);
    Sp = pow(Sp, Power);

    return Sp;
}

//�_�����g�U���ˍ��v�Z
float3 CalcPointLightPower(
    float3 pos,
    float3 normal,
    uint   light_num
    )
{
    float3 power = g_PointLightPos[light_num];
    float  Len;

    power -= pos;
    Len = length(power);
    power.y = max(dot(normal, power) / Len, 0);

    power.x = min(Len / g_PointLightSize[light_num], 1);
    power.x = 1 - power.x;
    power.x *= power.x;
    power.x *= power.y;
    
    return g_PointLightColor[light_num] * power.x;
}

//�_�������ʔ��ˍ��v�Z
float3 CalcPointLightSpecular(
    float3 Pos,
    float3 Normal,
    int    i,
    float  Power
    )
{
    float3 LightVec = Pos - g_PointLightPos[i];
    float L = 1 - min(length(LightVec) / g_PointLightSize[i], 1);
    L *= L;

    return CalcSpecular(Pos, Normal, normalize(LightVec), Power) *
        L*
        g_PointLightColor[i];
}

//********************************************************************
//
//		�֊s��
//
//********************************************************************

VP_HDR_OUTPUT VS_OutLine(float4 Pos : POSITION, float3 Normal : NORMAL)
{
    VP_HDR_OUTPUT Out = (VP_HDR_OUTPUT)0;
    float4 Normalx4  = float4(Normal, 0);

    //�r���[��Ԃ�Z�l�𒲐�
    Out.Pos = mul(Pos, g_W_mat);
    Out.Pos = mul(Out.Pos ,g_V_mat);

    Out.Pos += normalize(Out.Pos) * 0.5f;

    //�v���W�F�N�V������Ԃ�
    Out.Pos = mul(Out.Pos, g_P_mat);
    Normalx4 = mul(Normalx4, g_W_mat);
    Normalx4 = mul(Normalx4, g_VP_mat);

    //���_�ʒu��@�������Ɋg��
    Normalx4.xy = normalize(Normalx4.xy) * g_OutLineSize*Out.Pos.z;

    Out.Pos.xy += Normalx4.xy;
    

    return Out;
}

//------------------------------------------------------
//		�s�N�Z���V�F�[�_�[	
//------------------------------------------------------
PS_HDR_OUTPUT PS_OutLine(VS_OUTPUT In)
{
    PS_HDR_OUTPUT	OUT;

    //	�s�N�Z���F����
    OUT.Color.rgb = g_OutLineColor;
    OUT.Color.a = 1;

    OUT.HdrDepth.rgba = 0;
    OUT.HdrDepth.rgba = 0;

    return OUT;
}


//********************************************************************
//
//		�v���C���[�L�����N�^�p�V�F�[�_�[
//
//********************************************************************

struct VS_Player_OUTPUT
{
    float4 Pos		: POSITION;
    float4 Color	: COLOR;

    float3 Normal   : TEXCOORD0;
    float2 Tex		: TEXCOORD1;
    float3 wPos     : TEXCOORD2;
};



//------------------------------------------------------
//		���_�V�F�[�_�[	
//------------------------------------------------------
VS_Player_OUTPUT VS_Toon(VS_INPUT In)
{
    VS_Player_OUTPUT Out = (VS_Player_OUTPUT)0;

    //-----------------------------------------------------
    //    ���[���h�ϊ�
    //-----------------------------------------------------
    Out.Pos = mul(In.Pos, g_W_mat);
    Out.wPos = Out.Pos.xyz;

    //-----------------------------------------------------
    //  �r���[�v���W�F�N�V�����ϊ�
    //-----------------------------------------------------
    Out.Pos = mul(Out.Pos, g_VP_mat);

    //-----------------------------------------------------
    //  �e�N�X�`�����W
    //-----------------------------------------------------
    Out.Tex = In.Tex;

    //-----------------------------------------------------
    //  �F
    //-----------------------------------------------------
    Out.Color = 1;

    //-----------------------------------------------------
    //  ���[���h��Ԃ̖@��
    //-----------------------------------------------------
    Out.Normal = mul(float4(In.Normal, 0), g_W_mat).xyz;

    return Out;
}

//------------------------------------------------------
//		�s�N�Z���V�F�[�_�[	
//------------------------------------------------------
float4 PS_Toon(VS_Player_OUTPUT In) : COLOR
{
    const float4 DiffuseTex = tex2D(DecaleSamp, In.Tex);
    //const float4 SpecularTex = tex2D(SpecularSamp, In.Tex);
    const float4 SpecularTex = float4(0.5f, 0.5f, 0.5f, 1);


    float4	Out;
    float3  Diffuse  = float3(0, 0, 0);
    float3  Specular = float3(0, 0, 0);
    

    //-----------------------------------------------------
    //  ���O����
    //-----------------------------------------------------
    {
        In.Normal = normalize(In.Normal);
    }

    //-----------------------------------------------------
    //  ���C�e�B���O����
    //-----------------------------------------------------
    {
        //���s��
        Diffuse += CalcLambert(normalize(In.Normal), g_DirLightVec) * g_DirLightColor;
        Specular += CalcSpecular(In.wPos, In.Normal, g_DirLightVec, 30) * g_DirLightColor;
    }

    {
        //�_����
        for (int i = 0; i < g_PointLightUseNum; ++i)
        {
            Diffuse += CalcPointLightPower(In.wPos, In.Normal, i);
            Specular += CalcPointLightSpecular(In.wPos, In.Normal, i, 20);
        }
    }

    //-----------------------------------------------------
    //  ���ӂ�o�����X�y�L�����Ƃ��Čv�Z
    //-----------------------------------------------------
    Specular += max(Diffuse - 1, 0);

    //-----------------------------------------------------
    //  �g�D�[������
    //-----------------------------------------------------

    {
        //�g�U���˃g�D�[������
        Diffuse.r = tex2D(ToonSamp, float2(Diffuse.r, 0));
        Diffuse.g = tex2D(ToonSamp, float2(Diffuse.g, 0));
        Diffuse.b = tex2D(ToonSamp, float2(Diffuse.b, 0));
    
        //���ʔ��˃g�D�[������
        Specular.r = tex2D(ToonSpSamp, float2(Specular.r, 0));
        Specular.g = tex2D(ToonSpSamp, float2(Specular.g, 0));
        Specular.b = tex2D(ToonSpSamp, float2(Specular.b, 0));
    }


    //-----------------------------------------------------
    //  �ŏI�v�Z
    //-----------------------------------------------------
    {
        //�g�U���ːF
        Out = In.Color * DiffuseTex;
        Out.rgb *= Diffuse;

        //���ʔ��ːF
        Out.rgb += Specular*SpecularTex;
    }

    return Out;
}

//------------------------------------------------------
//		�g�D�[���`��e�N�j�b�N
//------------------------------------------------------
technique Toon
{
    pass P0
    {
        AlphaBlendEnable = true;
        BlendOp = Add;
        SrcBlend = SrcAlpha;
        DestBlend = InvSrcAlpha;
        CullMode = CW;
        ZEnable = true;
        ZWriteEnable = true;

        VertexShader = compile vs_3_0 VS_OutLine();
        PixelShader = compile ps_3_0 PS_OutLine();
    }
    

    pass P1
    {
        AlphaBlendEnable = true;
        BlendOp = Add;
        SrcBlend = SrcAlpha;
        DestBlend = InvSrcAlpha;
        CullMode = CCW;
        ZEnable = true;
        ZWriteEnable = true;

        VertexShader = compile vs_3_0 VS_Toon();
        PixelShader = compile ps_3_0 PS_Toon();
    }


    
}
