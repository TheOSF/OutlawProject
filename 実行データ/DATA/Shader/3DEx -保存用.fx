//********************************************************************
//																									
//		３Ｄ用シェーダー		
//
//********************************************************************
//------------------------------------------------------
//		環境関連
//------------------------------------------------------

//---ビュー関連---//
float4x4 g_VP_mat;	
float4x4 g_V_mat;
float4x4 g_P_mat;

float3   g_CameraPos;
float3   g_CameraVec;

//---スクリーン関連---//
float2   g_ScreenPixelSize;

//---モデル関連---//
float4x4 g_W_mat;
float4 g_Color = float4(1, 1, 1, 1);

//---平行光関連---//
float3 g_DirLightVec;
float3 g_DirLightColor;

//---点光源関連---//
#define MAX_POINTLIGHT 3

int    g_PointLightUseNum;
float3 g_PointLightPos  [MAX_POINTLIGHT];
float3 g_PointLightColor[MAX_POINTLIGHT];
float  g_PointLightSize [MAX_POINTLIGHT];

//---輪郭線---//
float3 g_OutLineColor = float3(0, 0, 0);
float  g_OutLineSize = 0.002f;

//------------------------------------------------------
//		テクスチャサンプラー	
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
//		頂点フォーマット
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
//		基本３Ｄシェーダー		
//
//********************************************************************
//------------------------------------------------------
//		頂点シェーダー	
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
//		ピクセルシェーダー	
//------------------------------------------------------
float4 PS_Basic( VS_OUTPUT In) : COLOR
{   
	float4	OUT;
	//	ピクセル色決定
	OUT = In.Color * tex2D( DecaleSamp, In.Tex );

	return OUT;
}

//------------------------------------------------------
//		通常描画テクニック
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
//		関数
//
//********************************************************************

//ハーフランバート
float CalcHalfLambert(float3 LightVec, float3 Normal)
{
    return dot(Normal, -LightVec) * 0.5f + 0.5f;
}

//ランバート
float CalcLambert(float3 LightVec, float3 Normal)
{
    return max(dot(Normal, -LightVec), 0);
}

//スペキュラ
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

//点光源拡散反射項計算
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

//点光源鏡面反射項計算
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
//		輪郭線
//
//********************************************************************

VP_HDR_OUTPUT VS_OutLine(float4 Pos : POSITION, float3 Normal : NORMAL)
{
    VP_HDR_OUTPUT Out = (VP_HDR_OUTPUT)0;
    float4 Normalx4  = float4(Normal, 0);

    //ビュー空間でZ値を調整
    Out.Pos = mul(Pos, g_W_mat);
    Out.Pos = mul(Out.Pos ,g_V_mat);

    Out.Pos += normalize(Out.Pos) * 0.5f;

    //プロジェクション空間へ
    Out.Pos = mul(Out.Pos, g_P_mat);
    Normalx4 = mul(Normalx4, g_W_mat);
    Normalx4 = mul(Normalx4, g_VP_mat);

    //頂点位置を法線方向に拡大
    Normalx4.xy = normalize(Normalx4.xy) * g_OutLineSize*Out.Pos.z;

    Out.Pos.xy += Normalx4.xy;
    

    return Out;
}

//------------------------------------------------------
//		ピクセルシェーダー	
//------------------------------------------------------
PS_HDR_OUTPUT PS_OutLine(VS_OUTPUT In)
{
    PS_HDR_OUTPUT	OUT;

    //	ピクセル色決定
    OUT.Color.rgb = g_OutLineColor;
    OUT.Color.a = 1;

    OUT.HdrDepth.rgba = 0;
    OUT.HdrDepth.rgba = 0;

    return OUT;
}


//********************************************************************
//
//		プレイヤーキャラクタ用シェーダー
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
//		頂点シェーダー	
//------------------------------------------------------
VS_Player_OUTPUT VS_Toon(VS_INPUT In)
{
    VS_Player_OUTPUT Out = (VS_Player_OUTPUT)0;

    //-----------------------------------------------------
    //    ワールド変換
    //-----------------------------------------------------
    Out.Pos = mul(In.Pos, g_W_mat);
    Out.wPos = Out.Pos.xyz;

    //-----------------------------------------------------
    //  ビュープロジェクション変換
    //-----------------------------------------------------
    Out.Pos = mul(Out.Pos, g_VP_mat);

    //-----------------------------------------------------
    //  テクスチャ座標
    //-----------------------------------------------------
    Out.Tex = In.Tex;

    //-----------------------------------------------------
    //  色
    //-----------------------------------------------------
    Out.Color = 1;

    //-----------------------------------------------------
    //  ワールド空間の法線
    //-----------------------------------------------------
    Out.Normal = mul(float4(In.Normal, 0), g_W_mat).xyz;

    return Out;
}

//------------------------------------------------------
//		ピクセルシェーダー	
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
    //  事前処理
    //-----------------------------------------------------
    {
        In.Normal = normalize(In.Normal);
    }

    //-----------------------------------------------------
    //  ライティング処理
    //-----------------------------------------------------
    {
        //平行光
        Diffuse += CalcLambert(normalize(In.Normal), g_DirLightVec) * g_DirLightColor;
        Specular += CalcSpecular(In.wPos, In.Normal, g_DirLightVec, 30) * g_DirLightColor;
    }

    {
        //点光源
        for (int i = 0; i < g_PointLightUseNum; ++i)
        {
            Diffuse += CalcPointLightPower(In.wPos, In.Normal, i);
            Specular += CalcPointLightSpecular(In.wPos, In.Normal, i, 20);
        }
    }

    //-----------------------------------------------------
    //  あふれ出しをスペキュラとして計算
    //-----------------------------------------------------
    Specular += max(Diffuse - 1, 0);

    //-----------------------------------------------------
    //  トゥーン処理
    //-----------------------------------------------------

    {
        //拡散反射トゥーン処理
        Diffuse.r = tex2D(ToonSamp, float2(Diffuse.r, 0));
        Diffuse.g = tex2D(ToonSamp, float2(Diffuse.g, 0));
        Diffuse.b = tex2D(ToonSamp, float2(Diffuse.b, 0));
    
        //鏡面反射トゥーン処理
        Specular.r = tex2D(ToonSpSamp, float2(Specular.r, 0));
        Specular.g = tex2D(ToonSpSamp, float2(Specular.g, 0));
        Specular.b = tex2D(ToonSpSamp, float2(Specular.b, 0));
    }


    //-----------------------------------------------------
    //  最終計算
    //-----------------------------------------------------
    {
        //拡散反射色
        Out = In.Color * DiffuseTex;
        Out.rgb *= Diffuse;

        //鏡面反射色
        Out.rgb += Specular*SpecularTex;
    }

    return Out;
}

//------------------------------------------------------
//		トゥーン描画テクニック
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
