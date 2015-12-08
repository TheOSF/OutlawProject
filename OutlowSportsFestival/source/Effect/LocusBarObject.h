#pragma once 

#include "../GameSystem/GameObject.h"
#include "../utillity/LocusHDR.h"

//----------------------------------------------------//
// 軌跡を円状に並べるオブジェクト
//----------------------------------------------------//

class LocusBarObject :public GameObjectBase
{
public:
    struct InitParams
    {
        Vector3 origin;    //原点
        Vector3 start_vec; //始まった時点でのベクトル
        Vector3 end_vec;   //終了時点でのベクトル

        RATIO   speed;     //回転スピード

        float   LocusPosLen;//原点から軌跡までの距離
    };

    LocusHDR  m_Locus;    

    LocusBarObject(
        const InitParams& Param,
        UINT              LocusLength
        );

    ~LocusBarObject();


    bool Update();
    bool Msg(MsgType mt);

private:
    const RATIO     m_Speed;
    const float     m_LocusPosLen;
    const Vector3   m_Vec;
    const Vector3   m_Origin;

    D3DXQUATERNION  m_Rot;
    RATIO           m_Count;
    UINT            m_DeleteTimer;
  
};