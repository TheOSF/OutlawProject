


//********************************************************************
//
//		ブラーシェーダー		
//
//********************************************************************


texture Texture;
sampler TextureSamp = sampler_state
{
    Texture = <Texture>;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    MipFilter = NONE;

    AddressU = CLAMP;
    AddressV = CLAMP;
};

float g_offset_X;
float g_offset_Y;

float g_Alpha;

float4 PS_gaussX(float2 Tex :TEXCOORD0) : COLOR
{
    float4 Out = tex2D(TextureSamp, Tex)*0.20f;

    Out += tex2D(TextureSamp, Tex + float2(-g_offset_X * 1, 0.0f))*0.12f;
    Out += tex2D(TextureSamp, Tex + float2(-g_offset_X * 2, 0.0f))*0.10f;
    Out += tex2D(TextureSamp, Tex + float2(-g_offset_X * 3, 0.0f))*0.08f;
    Out += tex2D(TextureSamp, Tex + float2(-g_offset_X * 4, 0.0f))*0.06f;
    Out += tex2D(TextureSamp, Tex + float2(-g_offset_X * 5, 0.0f))*0.04f;

    Out += tex2D(TextureSamp, Tex + float2(g_offset_X * 1, 0.0f))*0.12f;
    Out += tex2D(TextureSamp, Tex + float2(g_offset_X * 2, 0.0f))*0.10f;
    Out += tex2D(TextureSamp, Tex + float2(g_offset_X * 3, 0.0f))*0.08f;
    Out += tex2D(TextureSamp, Tex + float2(g_offset_X * 4, 0.0f))*0.06f;
    Out += tex2D(TextureSamp, Tex + float2(g_offset_X * 5, 0.0f))*0.04f;

    return Out;
}

float4 PS_gaussY(float2 Tex :TEXCOORD0) : COLOR
{
    float4 Out = tex2D(TextureSamp, Tex)*0.20f;

    Out += tex2D(TextureSamp, Tex + float2(0.0f, -g_offset_Y * 1))*0.12f;
    Out += tex2D(TextureSamp, Tex + float2(0.0f, -g_offset_Y * 2))*0.10f;
    Out += tex2D(TextureSamp, Tex + float2(0.0f, -g_offset_Y * 3))*0.08f;
    Out += tex2D(TextureSamp, Tex + float2(0.0f, -g_offset_Y * 4))*0.06f;
    Out += tex2D(TextureSamp, Tex + float2(0.0f, -g_offset_Y * 5))*0.04f;

    Out += tex2D(TextureSamp, Tex + float2(0.0f, g_offset_Y * 1))*0.12f;
    Out += tex2D(TextureSamp, Tex + float2(0.0f, g_offset_Y * 2))*0.10f;
    Out += tex2D(TextureSamp, Tex + float2(0.0f, g_offset_Y * 3))*0.08f;
    Out += tex2D(TextureSamp, Tex + float2(0.0f, g_offset_Y * 4))*0.06f;
    Out += tex2D(TextureSamp, Tex + float2(0.0f, g_offset_Y * 5))*0.04f;

    return Out;
}




float4 PS_gauss(float2 Tex :TEXCOORD0) : COLOR
{
    float4 Out = tex2D(TextureSamp, Tex)*0.20f;

    Out += tex2D(TextureSamp, Tex + float2(-g_offset_X, -g_offset_Y))*0.1f;
    Out += tex2D(TextureSamp, Tex + float2(0, -g_offset_Y))*0.1f;
    Out += tex2D(TextureSamp, Tex + float2(g_offset_X, -g_offset_Y))*0.1f;

    Out += tex2D(TextureSamp, Tex + float2(-g_offset_X, 0))*0.1f;
    Out += tex2D(TextureSamp, Tex + float2(g_offset_X, 0))*0.1f;

    Out += tex2D(TextureSamp, Tex + float2(-g_offset_X, g_offset_Y))*0.1f;
    Out += tex2D(TextureSamp, Tex + float2(0, g_offset_Y))*0.1f;
    Out += tex2D(TextureSamp, Tex + float2(g_offset_X, g_offset_Y))*0.1f;

    return Out;
}



technique gaussX
{
    pass P0
    {
        AlphaBlendEnable = true;
        BlendOp = Add;
        SrcBlend = SrcAlpha;
        DestBlend = InvSrcAlpha;
        CullMode = None;
        ZEnable = false;

        PixelShader = compile ps_2_0 PS_gaussX();
    }
}

technique gaussY
{
    pass P0
    {
        AlphaBlendEnable = true;
        BlendOp = Add;
        SrcBlend = SrcAlpha;
        DestBlend = InvSrcAlpha;
        CullMode = None;
        ZEnable = false;

        PixelShader = compile ps_2_0 PS_gaussY();
    }
}


float4 Add(float2  Tex : TEXCOORD) :COLOR0
{
    float4 Out;

    Out = tex2D(TextureSamp,Tex);
    Out.a *= g_Alpha;

    return Out;
}

technique add
{
    pass P0
    {
        AlphaBlendEnable = true;
        BlendOp = Add;
        SrcBlend = SrcAlpha;
        DestBlend = One;
        CullMode = None;
        ZEnable = false;
        ZWriteEnable = false;

        PixelShader = compile ps_2_0 Add();
    }

}

technique gauss
{


    pass P0
    {
        AlphaBlendEnable = false;
        BlendOp = Add;
        SrcBlend = SrcAlpha;
        DestBlend = InvSrcAlpha;
        CullMode = None;
        ZEnable = false;
        ZWriteEnable = false;

        PixelShader = compile ps_2_0 PS_gauss();
    }

}

technique gaussXY
{

    
    pass P0
    {
        AlphaBlendEnable = false;
        BlendOp = Add;
        SrcBlend = SrcAlpha;
        DestBlend = InvSrcAlpha;
        CullMode = None;
        ZEnable = false;
        ZWriteEnable = false;

        PixelShader = compile ps_2_0 PS_gaussX();
    }

    pass P1
    {
        AlphaBlendEnable = false;
        BlendOp = Add;
        SrcBlend = SrcAlpha;
        DestBlend = InvSrcAlpha;
        CullMode = None;
        ZEnable = false;

        PixelShader = compile ps_2_0 PS_gaussY();
    }

}