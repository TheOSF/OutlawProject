#pragma once
#include "iextreme.h"
#include "../Render/Renderer.h"

//----------------------------------------------
//    軌跡描画クラス
//----------------------------------------------

class LocusHDR :protected ForwardHDRRenderer
{
public:
    struct Param
    {
        float   Width;   //帯の幅
        Vector4 Color;   //色(r,g,b,aの順番なので注意！)
        Vector4 HDRColor;   //色(r,g,b,aの順番なので注意！)
    };

    bool              m_Visible;         //描画するかどうか

    LPIEX2DOBJ        m_pTexture;        //帯に張るテクスチャ(nullで色のみ)

    Param             m_StartParam;      //開始地点の情報
    Param             m_EndParam;        //終了地点の情報

    UINT              m_Division;        //補間の分割数

    LocusHDR(
        size_t SavePointNum  //保存する点の数
        );
    ~LocusHDR();

    //帯の点を追加する
    void AddPoint(
        CrVector3 pos,   //中心位置
        CrVector3 vec    //帯の伸びる方向(正規化していなければならない)
        );

    //帯の長さを０に
    void Clear();

    //軌跡の点を後ろから消去する
    void SubPoint(
        size_t num       //消去する点の数
        );

    //点を得る(なければfalseを返す)
    bool GetPos(UINT n, Vector3& out)const;

private:

    struct Point
    {
        Vector3 pos;
        Vector3 vec;
    };

    Point*		    m_pPointData;
    const size_t    m_NumPoint;
    size_t		    m_UseCount;

    void Initialize();

    void CalcZ();
    void Render();

    void RenderDivision();
    void RenderUsual();

    void RenderHDR(
        const Param& param1,
        const Param& param2,
        const Point& point1,
        const Point& point2,
        float t,
        float t2
        );


    void LeapParam(
        Param& out,
        const Param& in1,
        const Param& in2,
        float t
        );
};
