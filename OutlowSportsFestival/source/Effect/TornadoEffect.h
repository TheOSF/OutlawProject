#pragma once

#include "../utillity/LocusHDR.h"
#include "../GameSystem/GameObject.h"



//-------------------------------------------------------//
//            �g���l�[�h�G�t�F�N�g�N���X
//-------------------------------------------------------//

class TornadoEffect :public GameObjectBase
{
public:

    struct Param
    {
        Vector3    pos;
        Vector3    vec;
        Vector3    right;

        float      LocusWidthStart;
        float      LocusWidthEnd;

        float      Length;

        float      lowWidth;   //�L����̕�
        float      middleWidth;
        float      highWidth;

        UINT       middle_height;

        float      RotateSpeed;
    };

    TornadoEffect(
        const Param&   InitParam,
        UINT           NumLocus,
        UINT           NumLocusPoint
        );

    ~TornadoEffect();

    void Destroy();


    Param   m_Param;

private:

    const UINT    m_NumLocus;
    const UINT    m_LocusPoint;
    
    LocusHDR**    m_ppLocusPtrArray; 
    bool          m_Destroy;
    RADIAN        m_Angle;

    bool Update();
    bool Msg(MsgType msg);

};