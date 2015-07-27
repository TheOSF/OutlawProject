#include "Locus.h"
#include "../Camera/Camera.h"

Locus::Locus(size_t SavePointNum) :
m_NumPoint(SavePointNum),
m_UseCount(0)
{
	//ポイントデータ生成
	m_pPointData = new Point[SavePointNum];

	//パラメータ初期化
	Initialize();
}

Locus::~Locus()
{
	//解法
	delete[] m_pPointData;
}

//帯の点を追加する
void Locus::AddPoint(
	CrVector3 pos,  //中心位置
	CrVector3 vec   //帯の伸びる方向(正規化していなければならない)
	)
{

	//最大の場合ひとつ前にずらす
	if (m_UseCount == m_NumPoint)
	{
		for (size_t i = 0; i < m_UseCount - 1; ++i)
		{
			m_pPointData[i] = m_pPointData[i + 1];
		}

		//データの最後に追加
		m_pPointData[m_UseCount - 1].pos = pos;
		m_pPointData[m_UseCount - 1].vec = vec;
	}
	else
	{
		//最後尾に追加
		m_pPointData[m_UseCount].pos = pos;
		m_pPointData[m_UseCount].vec = vec;

		++m_UseCount;
	}
}

//帯の長さを０に
void Locus::Clear()
{
	//使用カウントを０に
	m_UseCount = 0;
}

//軌跡の点を後ろから消去する
void Locus::SubPoint(
	size_t num      //消去する点の数
	)
{
	//使用している点より引く数が多い場合は全消去
	if (m_UseCount <= num)
	{
		Clear();
		return;
	}

	//使用カウントを引く
	m_UseCount -= num;
}

//初期化
void Locus::Initialize()
{
	m_Visible = true;
	m_pTexture = nullptr;

	m_StartParam.Width = 1;
	m_StartParam.Color = D3DXVECTOR4(1, 1, 1, 1);

	m_EndParam.Width = 1;
	m_EndParam.Color = D3DXVECTOR4(1, 1, 1, 1);

	m_Division = 4;
}

//カメラからのZ距離を計算する
void Locus::CalcZ()
{
	m_SortZ = 1; //仮
}


void Locus::Render()
{
	//描画しない場合
	if (!m_Visible || m_UseCount <= 1)
	{
		return;
	}

	iexSystem::Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	if (m_UseCount > 4 && m_Division >= 2)
	{
		RenderDivision();
	}
	else
	{
		RenderUsual();
	}

	iexSystem::Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void Locus::RenderDivision()
{
	

	const float div = 1.0f / (float)m_Division;          //１ / セルの分割数
	const float PolygonT = 1.0f / ((float)m_NumPoint*(float)m_Division);  // 1 / 四角形の描画数

	Point point[2]; // ０から１に描画
	Param param[2]; // ０から１に描画

	float d, t = 0, pret = 0;;

	param[0] = m_StartParam;

	for (size_t i = 0; i < m_UseCount - 4; ++i)
	{
		point[0] = m_pPointData[i + 1];

		for (d = 0; d <= 1 + FLT_EPSILON; d += div)
		{
			pret = t;
			t += PolygonT;

			//パラメータを補間し、param[1]に格納
			LeapParam(param[1], m_StartParam, m_EndParam, 1 - t);

			//補間
			D3DXVec3CatmullRom(
				(D3DXVECTOR3*)&point[1].pos, //out

				(D3DXVECTOR3*)&m_pPointData[i    ].pos,
				(D3DXVECTOR3*)&m_pPointData[i + 1].pos,
				(D3DXVECTOR3*)&m_pPointData[i + 2].pos,
				(D3DXVECTOR3*)&m_pPointData[i + 3].pos, 

				d
				);

			//補間
			D3DXVec3CatmullRom(
				(D3DXVECTOR3*)&point[1].vec, //out

				(D3DXVECTOR3*)&m_pPointData[i    ].vec,
				(D3DXVECTOR3*)&m_pPointData[i + 1].vec,
				(D3DXVECTOR3*)&m_pPointData[i + 2].vec,
				(D3DXVECTOR3*)&m_pPointData[i + 3].vec,

				d
				);

			point[1].vec.Normalize();

			Render(
				param[0],
				param[1],
				point[0],
				point[1],
				t,
				pret
				);

			point[0] = point[1];
			param[0] = param[1];
		}
	}
}


void Locus::RenderUsual()
{
	
	const float tAdd = 1 / (float)m_NumPoint;

	Param param[2];
	float t = 0, pret = 0;;

	param[0] = m_StartParam;


	for (size_t i = 0; i < m_UseCount - 1; ++i)
	{
		pret = t;
		t += tAdd;

		LeapParam(param[1], m_StartParam, m_EndParam, 1 - t);

		Render(
			param[0],
			param[1],
			m_pPointData[i],
			m_pPointData[i+1],
			t,
			pret
			);

		param[0] = param[1];
	}
}

void Locus::Render(
	const Param& param1,
	const Param& param2,
	const Point& point1,
	const Point& point2,
	float t,
	float t2
	)
{
	
	LVERTEX v[4];

	v[0].pos = point1.pos - point1.vec*param1.Width;
	v[1].pos = point1.pos + point1.vec*param1.Width;

	v[2].pos = point2.pos - point2.vec*param2.Width;
	v[3].pos = point2.pos + point2.vec*param2.Width;

	v[0].tu = t;
	v[0].tv = 1;

	v[1].tu = t;
	v[1].tv = 0;

	v[2].tu = t2;
	v[2].tv = 1;

	v[3].tu = t2;
	v[3].tv = 0;
	

	v[0].color = v[1].color = 
		D3DCOLOR_COLORVALUE(
		   param1.Color.x,
		   param1.Color.y,
		   param1.Color.z,
		   param1.Color.w
		   );

	v[2].color = v[3].color =
		D3DCOLOR_COLORVALUE(
		param2.Color.x,
		param2.Color.y,
		param2.Color.z,
		param2.Color.w
		);

	iexPolygon::Render3D(
		v, 2, m_pTexture, RS_COPY
		);

}

void Locus::LeapParam(
	Param& out,
	const Param& in1,
	const Param& in2,
	float t
	)
{
	out.Color = in1.Color*(1 - t) + in2.Color*t;
	out.Width = in1.Width*(1 - t) + in2.Width*t;
}