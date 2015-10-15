



texture SourceTexture;
sampler SourceSamp = sampler_state
{
    Texture = <SourceTexture>;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    MipFilter = NONE;

    AddressU = CLAMP;
    AddressV = CLAMP;
};



//パラメタ

float4x4 g_VP_mat;
float4x4 g_W_mat;

float    g_center_pos_x;
float    g_center_pos_y;

float    g_blur_size;
float    g_blur_power;

float    g_offset_X;
float    g_offset_Y;


//コーンブラー用定数

float    g_vec_x;
float    g_vec_y;

float    g_angle;



struct VS_OUTPUT_SphereBlur
{
    float4 Pos : POSITION;
    float4 pPos: TEXCOORD0;
};


VS_OUTPUT_SphereBlur VS_Radiation_Blur(float4 Pos : POSITION)
{
    VS_OUTPUT_SphereBlur Out;

    Out.Pos = mul(mul(Pos, g_W_mat), g_VP_mat);
    Out.pPos = Out.Pos;

    return Out;
}

float4 PS_Radiation_Blur(VS_OUTPUT_SphereBlur In) : COLOR
{
    float4 Color;

    const float2 Tex = In.pPos.xy / In.pPos.w * float2(0.5, -0.5) + float2(0.5, 0.5);

    float2 vec = float2(g_center_pos_x, g_center_pos_y) - Tex;
    float dist = length(vec);

    vec = normalize(vec) * float2(g_offset_X, g_offset_Y);
    vec *= dist * g_blur_power;

    Color = tex2D(SourceSamp, Tex) * 0.19f;
    Color += tex2D(SourceSamp, Tex + vec) * 0.17f;
    Color += tex2D(SourceSamp, Tex + vec * 2.0f) * 0.15f;
    Color += tex2D(SourceSamp, Tex + vec * 3.0f) * 0.13f;
    Color += tex2D(SourceSamp, Tex + vec * 4.0f) * 0.11f;
    Color += tex2D(SourceSamp, Tex + vec * 5.0f) * 0.09f;
    Color += tex2D(SourceSamp, Tex + vec * 6.0f) * 0.07f;
    Color += tex2D(SourceSamp, Tex + vec * 7.0f) * 0.05f;
    Color += tex2D(SourceSamp, Tex + vec * 8.0f) * 0.03f;
    Color += tex2D(SourceSamp, Tex + vec * 9.0f) * 0.01f;

    Color.a = min(dist / g_blur_size, 1);
    Color.a = 1 - Color.a;

    return Color;
}

technique radiation_blur
{
    pass P0
    {
        AlphaBlendEnable = true;
        BlendOp = Add;
        SrcBlend = SrcAlpha;
        DestBlend = InvSrcAlpha;
        CullMode = CW;
        ZEnable = false;
        ZWriteEnable = false;

        VertexShader = compile vs_2_0 VS_Radiation_Blur();
        PixelShader = compile ps_2_0 PS_Radiation_Blur();
    }
}

//--------------------------------------------------//


float4 PS_Radiation_BlurCone(VS_OUTPUT_SphereBlur In) : COLOR
{
    float4 Color;

    const float2 Tex = In.pPos.xy / In.pPos.w * float2(0.5, -0.5) + float2(0.5, 0.5);

    float2 vec = float2(g_center_pos_x, g_center_pos_y) - Tex;
    float dist = length(vec);

    vec = normalize(vec);

    float2 texvec = vec*float2(g_offset_X, g_offset_Y);

    texvec *= dist * g_blur_power;

    Color  = tex2D(SourceSamp, Tex) * 0.19f;
    Color += tex2D(SourceSamp, Tex + texvec) * 0.17f;
    Color += tex2D(SourceSamp, Tex + texvec * 2.0f) * 0.15f;
    Color += tex2D(SourceSamp, Tex + texvec * 3.0f) * 0.13f;
    Color += tex2D(SourceSamp, Tex + texvec * 4.0f) * 0.11f;
    Color += tex2D(SourceSamp, Tex + texvec * 5.0f) * 0.09f;
    Color += tex2D(SourceSamp, Tex + texvec * 6.0f) * 0.07f;
    Color += tex2D(SourceSamp, Tex + texvec * 7.0f) * 0.05f;
    Color += tex2D(SourceSamp, Tex + texvec * 8.0f) * 0.03f;
    Color += tex2D(SourceSamp, Tex + texvec * 9.0f) * 0.01f;

    Color.a = min(dist / g_blur_size, 1);
    Color.a = 1 - Color.a;

    //{
    //    float p = acos(dot(vec, float2(g_vec_x, g_vec_y)));
    //    p /= g_angle;

    //    Color.a *= 1 - min(p, 1);
    //}

    return Color;
}

technique radiation_blur_cone
{
    pass P0
    {
        AlphaBlendEnable = true;
        BlendOp = Add;
        SrcBlend = SrcAlpha;
        DestBlend = InvSrcAlpha;
        CullMode = CW;
        ZEnable = false;
        ZWriteEnable = false;

        VertexShader = compile vs_2_0 VS_Radiation_Blur();
        PixelShader = compile ps_2_0 PS_Radiation_BlurCone();
    }
}


//------------------------------------------------------
//		リニア空間変換
//------------------------------------------------------
float4 PS_Linear(float2 Tex:TEXCOORD0) : COLOR
{
    float4 Out = tex2D(SourceSamp, Tex);

    //Out.r = pow(Out.r, 0.4545f);
    //Out.g = pow(Out.g, 0.4545f);
    //Out.b = pow(Out.b, 0.4545f);

    return Out;
}


technique LinearSp
{
    pass P0
    {
        AlphaBlendEnable = true;
        BlendOp = Add;
        SrcBlend = SrcAlpha;
        DestBlend = InvSrcAlpha;
        CullMode = None;
        ZEnable = false;
        ZWriteEnable = false;

        PixelShader = compile ps_2_0 PS_Linear();
    }
}

