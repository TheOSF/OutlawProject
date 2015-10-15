
//-----------------------------------------------------
//	関数集
//-----------------------------------------------------

//XYベクトルの値からZの入ったベクトルを生成する
float3 CreateNormal(float2 xy)
{
	float3 normal;

	normal.xy = xy;
	// max() 入れないとなぜか正しく描画されないときがある
	normal.z = -sqrt(max(1 - normal.x * normal.x - normal.y * normal.y, 0));
	normal = normalize(normal);

	return normal;
}


////スペキュラ計算
//float CalcSpecular(
//	float3 pos,
//	float3 normal,
//	float3 ViewPos,
//	float3 LightDir,
//	float  power)
//{
//    float   sp;
//
//    float3	H = normalize(ViewPos - pos);
//    H = normalize(H - LightDir);
//    
//	sp = dot(normal, H);
//	sp = max( 0, sp );
//	sp = pow(sp, power);
//
//    return sp;
//}


//スペキュラ計算
float CalcSpecular(
	float3 pos,
	float3 normal,
	float3 LightDir,
	float  power)
{
    float   sp;

    float3	H = normalize(-pos);
    H = normalize(H - LightDir);
    
	sp = dot(normal, H);
	sp = max( 0, sp );
	sp = pow(sp, power);

    return sp;
}



////UVとZ値からワールド空間座標を算出
//float3 CalcWorldPosition(float2 texUV, float2 zw)
//{
//    float4 ret; 
//
//    ret.xy = (texUV * float2(2.0f, -2.0f) + float2(-1.0f, 1.0f)) * zw.y;
//	ret.zw = zw;
//	ret = mul(ret, g_VP_inv_mat);
//
//	return ret.rgb;
//}


//UVとZ値からワールド空間座標を算出
float3 CalcViewPosition(float2 texUV, float2 zw)
{
    float4 ret; 

    ret.xy = (texUV * float2(2.0f, -2.0f) + float2(-1.0f, 1.0f)) * zw.y;
	ret.zw = zw;
	ret = mul(ret, g_P_inv_mat);

	return ret.rgb;
}


////ライト値圧縮
//float4 ClacLightOutValue(float3 AddValue, float4 BufValue)
//{
//    float4 ret = BufValue;
//    
//    ret.rgb = (ret.rgb / ret.a)*0.25f;
//
//    ret.rgb += AddValue;
//
//    ret.a = max(max(ret.r, ret.g),max(ret.b, 0.25f));
//
//    ret.rgb /= ret.a;
//    ret.a = 0.25f/ret.a;
//
//    return ret;
//}

