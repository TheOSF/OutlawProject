

//------------------------------------------------------
//		ÉTÉìÉvÉâÅ[
//------------------------------------------------------


texture LightTex;
sampler LightTexSamp = sampler_state
{
    Texture = <LightTex>;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    MipFilter = NONE;

    AddressU = BORDER;
    AddressV = BORDER;
};

texture LightBloomTex;
sampler LightBloomTexSamp = sampler_state
{
    Texture = <LightBloomTex>;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    MipFilter = NONE;

    AddressU = BORDER;
    AddressV = BORDER;
};


//------------------------------------------------------
//	íËêî
//------------------------------------------------------



float4 PS_HDRRender(float2 Tex: TEXCOORD0) :COLOR0
{
    float4 Out;

    Out = tex2D(LightTexSamp, Tex) + tex2D(LightBloomTexSamp, Tex);
    Out.a = 1;

    Out.rgb -= 1.0f;
    Out.rgb *= 2.0f;

    return Out;
}


technique HDRRender
{
    pass P0
    {
        AlphaBlendEnable = true;
        BlendOp = Add;
        SrcBlend = SrcAlpha;
        DestBlend = InvSrcAlpha;
        CullMode = none;
        ZEnable = false;
        ZWriteEnable = false;

        PixelShader = compile ps_2_0 PS_HDRRender();
    }
}
