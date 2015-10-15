
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
//		���o�͍\����
//------------------------------------------------------
struct VS_Gbuf_OUTPUT
{
    float4 Pos		: POSITION;

    float2 Tex		: TEXCOORD0;
    float2 DepthZW	: TEXCOORD1;

    float3 Normal	: TEXCOORD2;
};

struct VS_Gbuf2_OUTPUT
{
    float4 Pos		: POSITION;

    float2 Tex		: TEXCOORD0;
    float4 pPos		: TEXCOORD2;
    float3 Normal	: TEXCOORD3;
    float3 Tangent	: TEXCOORD4;
    float3 Binormal	: TEXCOORD5;
    float3 Eye		: TEXCOORD6;
    float2 DepthZW	: TEXCOORD7;
};


struct VS_Gbuf_INPUT
{
    float4 Pos    : POSITION;
    float4 Color  : COLOR;
    float3 Normal : NORMAL;
    float2 Tex	  : TEXCOORD0;
};

struct PS_Gbuf_OUTPUT
{
    float4 NormalDepth          : COLOR0;
    float4 RoughnessLightMap    : COLOR1;
};


float4x4 g_W_mat;
float4x4 g_VP_mat;
float4x4 g_V_mat;

//--------------------------------------------------------------
//�@ �e�N�X�`�����g�p���Ȃ����_�V�F�[�_
//--------------------------------------------------------------

VS_Gbuf_OUTPUT VS_NoTexture(VS_Gbuf_INPUT In)
{
    VS_Gbuf_OUTPUT Out = (VS_Gbuf_OUTPUT)0;

    //���_���W
    Out.Pos = In.Pos;
  
    Out.Pos = mul(Out.Pos, g_W_mat);
    Out.Pos = mul(Out.Pos, g_VP_mat);

    //Z���W��ۑ�
    Out.DepthZW = Out.Pos.zw;

    //�e�N�X�`��
    Out.Tex = In.Tex;

    //�@�����r���[��Ԃɕϊ�
    Out.Normal = mul(float4(In.Normal, 0), g_W_mat).xyz;
    Out.Normal = mul(float4(Out.Normal, 0), g_V_mat).xyz;

    return Out;
}

PS_Gbuf_OUTPUT PS_NoTexture(VS_Gbuf_OUTPUT In)
{
    PS_Gbuf_OUTPUT	OUT = (PS_Gbuf_OUTPUT)0;

    //�@��
    OUT.NormalDepth.rg = normalize(In.Normal).rg * 0.5f + 0.5f;
    
    //�[�x
    OUT.NormalDepth.ba = In.DepthZW;


    //���ʔ��ˋ��x
    OUT.RoughnessLightMap.r = 0.1f;

    //���C�g�}�b�v�l
    OUT.RoughnessLightMap.g = 1.0f;

    //���܂�
    OUT.RoughnessLightMap.ba = 1.0f;

    return OUT;
}


technique NoTexture
{
    pass P0
    {
        AlphaBlendEnable = false;
        BlendOp = Add;
        SrcBlend = SrcAlpha;
        DestBlend = InvSrcAlpha;
        CullMode = CCW;
        ZEnable = true;
        ZWriteEnable = true;

        VertexShader = compile vs_2_0 VS_NoTexture();
        PixelShader = compile ps_2_0 PS_NoTexture();
    }
}

//--------------------------------------------------------------
//�@ �@���@�}�b�v���g�p
//--------------------------------------------------------------

VS_Gbuf2_OUTPUT VS_UseNormal(VS_Gbuf_INPUT In)
{
    VS_Gbuf2_OUTPUT Out = (VS_Gbuf2_OUTPUT)0;

    //���_���W
    Out.Pos = In.Pos;

    Out.Pos = mul(Out.Pos, g_W_mat);
    Out.Pos = mul(Out.Pos, g_VP_mat);

    //Z���W��ۑ�
    Out.DepthZW = Out.Pos.zw;

    //�e�N�X�`��
    Out.Tex = In.Tex;

    //�@�������[���h��Ԃɕϊ�
    Out.Normal = mul(float4(In.Normal, 0), g_W_mat).xyz;
    Out.Normal = mul(float4(Out.Normal, 0), g_V_mat).xyz;

    //IEX���ǂݍ���ł��Ȃ��̂ŁA����Binormal��Tangent���쐬
    float3 temp = { 0, 1, 0.001f };
    Out.Tangent = cross(temp, Out.Normal);
    Out.Binormal = cross(Out.Normal, Out.Tangent);

    return Out;
}


PS_Gbuf_OUTPUT PS_UseNormal(VS_Gbuf2_OUTPUT In)
{
    PS_Gbuf_OUTPUT	OUT = (PS_Gbuf_OUTPUT)0;
    

    {
        //�@���}�b�v

        float3x3 ToView;	//��]�s����쐬
        ToView[0] = normalize(In.Tangent);
        ToView[1] = normalize(In.Binormal);
        ToView[2] = normalize(In.Normal);

        //�@���}�b�v����擾
        OUT.NormalDepth.rgb = tex2D(NormalSamp, In.Tex).rgb;		//�e�N�X�`������擾
        OUT.NormalDepth.rgb = normalize(OUT.NormalDepth.rgb);			//���K������
        OUT.NormalDepth.rgb = (OUT.NormalDepth.rgb - 0.5f) * 2.0f;	//�x�N�g���ɕϊ�
        OUT.NormalDepth.rgb = mul(OUT.NormalDepth.rgb, ToView);		//��]
        OUT.NormalDepth.rgb = normalize(OUT.NormalDepth.rgb);			//���K������
        OUT.NormalDepth.rg = OUT.NormalDepth.rg*0.5f + 0.5f;		//�F�ɕϊ�

    }

    //�[�x
    OUT.NormalDepth.ba = In.DepthZW;

    //���ʔ��ˋ��x
    OUT.RoughnessLightMap.r = 0.1f;

    //���C�g�}�b�v�l
    OUT.RoughnessLightMap.g = 1.0f;

    //���܂�
    OUT.RoughnessLightMap.ba = 1.0f;

    return OUT;
}


technique UseNormal
{
    pass P0
    {
        AlphaBlendEnable = false;
        BlendOp = Add;
        SrcBlend = SrcAlpha;
        DestBlend = InvSrcAlpha;
        CullMode = CCW;
        ZEnable = true;
        ZWriteEnable = true;

        VertexShader = compile vs_2_0 VS_UseNormal();
        PixelShader = compile ps_2_0 PS_UseNormal();
    }
}


//--------------------------------------------------------------
//�@ �@���@�����@�}�b�v���g�p
//--------------------------------------------------------------

VS_Gbuf2_OUTPUT VS_UseNormalHeight(VS_Gbuf_INPUT In)
{
    VS_Gbuf2_OUTPUT Out = (VS_Gbuf2_OUTPUT)0;

    //���_���W
    Out.Pos = In.Pos;

    Out.Pos = mul(Out.Pos, g_W_mat);
    Out.Pos = mul(Out.Pos, g_VP_mat);

    //Z���W��ۑ�
    Out.DepthZW = Out.Pos.zw;

    //�e�N�X�`��
    Out.Tex = In.Tex;

    //�@�������[���h��Ԃɕϊ�
    Out.Normal = mul(float4(In.Normal, 0), g_W_mat).xyz;
    Out.Normal = mul(float4(Out.Normal, 0), g_V_mat).xyz;

    //IEX���ǂݍ���ł��Ȃ��̂ŁA����Binormal��Tangent���쐬
    float3 temp = { 0, 1, 0.001f };
    Out.Tangent = cross(temp, Out.Normal);
    Out.Binormal = cross(Out.Normal, Out.Tangent);

    return Out;
}


PS_Gbuf_OUTPUT PS_UseNormalHeight(VS_Gbuf2_OUTPUT In)
{
    PS_Gbuf_OUTPUT	OUT = (PS_Gbuf_OUTPUT)0;


    {
        //�@���}�b�v

        float3x3 ToView;	//��]�s����쐬
        ToView[0] = normalize(In.Tangent);
        ToView[1] = normalize(In.Binormal);
        ToView[2] = normalize(In.Normal);

        //�@���}�b�v����擾
        OUT.NormalDepth.rgb = tex2D(NormalSamp, In.Tex).rgb;		//�e�N�X�`������擾
        OUT.NormalDepth.rgb = normalize(OUT.NormalDepth.rgb);			//���K������
        OUT.NormalDepth.rgb = (OUT.NormalDepth.rgb - 0.5f) * 2.0f;	//�x�N�g���ɕϊ�
        OUT.NormalDepth.rgb = mul(OUT.NormalDepth.rgb, ToView);		//��]
        OUT.NormalDepth.rgb = normalize(OUT.NormalDepth.rgb);			//���K������
        OUT.NormalDepth.rg = OUT.NormalDepth.rg*0.5f + 0.5f;		//�F�ɕϊ�

    }

    //�[�x
    OUT.NormalDepth.ba = In.DepthZW;


    //���ʔ��ˋ��x
    OUT.RoughnessLightMap.r = 0.1f;

    //���C�g�}�b�v�l
    OUT.RoughnessLightMap.g = 1.0f;

    //���܂�
    OUT.RoughnessLightMap.ba = 1.0f;

    return OUT;
}


technique UseNormalHeight
{
    pass P0
    {
        AlphaBlendEnable = false;
        BlendOp = Add;
        SrcBlend = SrcAlpha;
        DestBlend = InvSrcAlpha;
        CullMode = CCW;
        ZEnable = true;
        ZWriteEnable = true;

        VertexShader = compile vs_2_0 VS_UseNormalHeight();
        PixelShader = compile ps_2_0 PS_UseNormalHeight();
    }
}

//----------------------------------------------------------------------
// �N���A
//----------------------------------------------------------------------

PS_Gbuf_OUTPUT PS_Clear()
{
    PS_Gbuf_OUTPUT Out;

    Out.NormalDepth = float4(0, 0, 0, 0);
    Out.RoughnessLightMap = float4(0, 0, 0, 0);
        
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
