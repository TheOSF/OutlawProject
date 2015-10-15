
//------------------------------------------------------
//		テクスチャサンプラー	
//------------------------------------------------------
texture ZTexture;
sampler ZTextureSamp = sampler_state
{
    Texture = <ZTexture>;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    MipFilter = NONE;

    AddressU = CLAMP;
    AddressV = CLAMP;
};


float g_offset_x;
float g_offset_y;

float  g_Outline_Start;
float  g_Outline_End;
float3 g_Outline_Color;

#define SampleNum 8

float2 SampleUV[SampleNum] =
{
    float2(1, 0),
    float2(-1, 0),
    float2(0, 1),
    float2(0, -1),

    float2(1, 1),
    float2(-1, 1),
    float2(1, -1),
    float2(-1, -1)
};

float4 PS_OutLine(float2 Tex : TEXCOORD0) :COLOR
{
    
    float4 Out = float4(g_Outline_Color, 0);

    //現在のピクセルのZ座標
    float MyZ = tex2D(ZTextureSamp, Tex).r;

    //Z値の差分
    float Diff = 0;

    //サンプリング点のZ
    float OtherZ;

    //ピクセルサイズ
    float2 pixel = float2(g_offset_x, g_offset_y);

    //差分を算出
    for (uint i = 0; i < SampleNum; ++i)
    {
        OtherZ = tex2D(ZTextureSamp, Tex + SampleUV[i] * pixel).r;
        //Diff = abs(MyZ - OtherZ);
        Diff = max(Diff, (OtherZ - MyZ));
    }

  //  Diff *= 0.125f;

    if (Diff > g_Outline_Start)
    {
        Diff -= g_Outline_Start;
        Out.a = Diff / (g_Outline_End - g_Outline_Start);
    }

    return Out;
}


technique Outline
{
    pass P0
    {
        AlphaBlendEnable = true;
        BlendOp = Add;
        SrcBlend = SrcAlpha;
        DestBlend = InvSrcAlpha;
        CullMode = CCW;
        ZEnable = false;
        ZWriteEnable = false;

        PixelShader = compile ps_2_0 PS_OutLine();
    }
}

