

//------------------------------------------------------
//		環境関連
//------------------------------------------------------
float4x4 g_VP_mat;	//	投影変換行列
float4x4 g_W_mat;	    //ワールド変換

float4   g_Color;
float3   g_Diffuse = float3(1, 1, 1);
float3   g_Specular = float3(1, 1, 1);//スペキュラー

float3   g_HDR_Color; //HDRカラー

//float    g_Outline_Size = 0.1f;
//float4   g_Outline_Color = float4(2, 1.5f, 0, 0);


float    g_Outline_Size = 0.04f;
float4   g_Outline_Color = float4(0, 0, 0, 1);


//------------------------------------------------------
//		環境関連 (フォーワード描画用)
//------------------------------------------------------

//平行光
float3 FR_DirLightVec;
float3 FR_DirLightColor;

//環境光
float3 FR_AmbientColor;
float3 FR_SkyColor;
float3 FR_GroundColor;


//------------------------------------------------------
//		ソフトパーティクル描画用
//------------------------------------------------------

float g_SoftDepth = 0.08f; //透明にし始める距離

//------------------------------------------------------
//		テクスチャサンプラー	
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
//		ライトバッファサンプラー	
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
//		法線・深度バッファサンプラー	
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
//		頂点Input
//------------------------------------------------------

struct VS_INPUT
{
    float4 Pos		: POSITION;
    float4 Color	: COLOR0;

    float2 Tex		: TEXCOORD0;
};


//------------------------------------------------------
//		頂点Output
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
//      ピクセルOutput
//------------------------------------------------------
struct PS_OUT_DeffLight
{
    float4 Color    : COLOR0;
    float4 High     : COLOR1;
};

//********************************************************************
//
//		アウトライン
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
//		ディファードライティングレンダリング
//
//********************************************************************
//------------------------------------------------------
//		頂点シェーダー	
//------------------------------------------------------
VS_OUT_DeffLight VS_DeffLightRender(VS_INPUT In)
{
    VS_OUT_DeffLight Out = (VS_OUT_DeffLight)0;

    //ワールド→射影変換
    Out.Pos = mul(mul(In.Pos, g_W_mat), g_VP_mat);

    //テクスチャ座標
    Out.Tex = In.Tex;

    //頂点色
    Out.Color = 1.0f;

    //射影変換済み座標を保存
    Out.pPos = Out.Pos;

    return Out;
}

//------------------------------------------------------
//		ピクセルシェーダー	
//------------------------------------------------------
PS_OUT_DeffLight PS_DeffLightRender(VS_OUT_DeffLight In)
{
    //出力変数
    PS_OUT_DeffLight Out;

    //スクリーン空間をテクスチャ座標に
    const float2 ScreenTex = In.pPos.xy / In.pPos.w * float2(0.5, -0.5) + float2(0.5, 0.5);

    //  拡散反射
    {
        Out.Color = In.Color * tex2D(DecaleSamp, In.Tex);
        Out.Color.rgb *= tex2D(DiffuseLightSamp, ScreenTex).rgb * 2;
    }

    //  鏡面反射
    {
        Out.Color.rgb += tex2D(SpecularSamp, In.Tex).rgb * 2 * tex2D(SpecuarLightSamp, ScreenTex).rgb;
    }

    //高輝度部分を抽出
    Out.High.rgb = max(Out.Color.rgb - 1.0f, 0);
    Out.High.rgb += g_HDR_Color;
    
    Out.High.a = 1;



    return Out;
}


//------------------------------------------------------
//		ピクセルシェーダー	
//------------------------------------------------------
PS_OUT_DeffLight PS_DeffLightNoSpecularRender(VS_OUT_DeffLight In)
{
    //出力変数
    PS_OUT_DeffLight Out;

    //スクリーン空間をテクスチャ座標に
    const float2 ScreenTex = In.pPos.xy / In.pPos.w * float2(0.5, -0.5) + float2(0.5, 0.5);

    //  拡散反射
    {
        Out.Color = In.Color * tex2D(DecaleSamp, In.Tex);
        Out.Color.rgb *= tex2D(DiffuseLightSamp, ScreenTex).rgb * 2.0f;
    }

    //  鏡面反射
    {
        Out.Color.rgb += tex2D(SpecuarLightSamp, ScreenTex).rgb * g_Specular * 2.0f;
    }

    //高輝度部分を抽出
    Out.High.rgb = max(Out.Color.rgb - 1.0f, 0);
    Out.High.rgb += g_HDR_Color;

    Out.High.a = 1;

    return Out;
}


//------------------------------------------------------
//		ピクセルシェーダー	
//------------------------------------------------------
PS_OUT_DeffLight PS_DeffLightNoTextureRender(VS_OUT_DeffLight In)
{
    //出力変数
    PS_OUT_DeffLight Out;

    //スクリーン空間をテクスチャ座標に
    const float2 ScreenTex = In.pPos.xy / In.pPos.w * float2(0.5, -0.5) + float2(0.5, 0.5);

    //  拡散反射
    {
        Out.Color = In.Color;
        Out.Color.rgb *= tex2D(DiffuseLightSamp, ScreenTex).rgb * 2.0f;
    }

    //  鏡面反射
    {
        Out.Color.rgb += tex2D(SpecuarLightSamp, ScreenTex).rgb * g_Specular * 2.0f;
    }

    //高輝度部分を抽出
    Out.High.rgb = max(Out.Color.rgb - 1.0f, 0);
    Out.High.rgb += g_HDR_Color;

    Out.High.a = 1;

    return Out;
}


//------------------------------------------------------
//		通常描画テクニック
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
//		HDRフォワードレンダリング
//------------------------------------------------------


PS_OUT_DeffLight PS_HDR_Forward(VS_OUT_DeffLight In)
{
    //出力変数
    PS_OUT_DeffLight Out;

    //  拡散反射
    {
        Out.Color = In.Color * tex2D(DecaleSamp, In.Tex) * g_Color;
    }

    //高輝度
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
//		HDRフォワードポリゴンレンダリング
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
    //出力変数
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
//		ソフトパーティクルレンダリング
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
    //出力変数
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
//		デバッグ描画用
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
//		フォワード描画用
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
        //ハーフランバート
        Out.Color.rgb += FR_DirLightColor * max(dot(FR_DirLightVec, WorldNormal), 0);
    }

    {
        //半球
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
        //アンビエント
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