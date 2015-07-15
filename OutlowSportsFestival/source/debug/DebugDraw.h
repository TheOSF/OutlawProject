#ifndef __DEBUG_DRAW_H__
#define __DEBUG_DRAW_H__

#include "iextreme.h"
#include "../utillity/ColorUtility.h"
#include "../GameSystem/GameObject.h"
#include "../Render/Renderer.h"

//--------------------------------------------------------
//        �f�o�b�O�`��p�@��
//--------------------------------------------------------

class DebugDrawSphere :
    public GameObjectBase, public ForwardRenderer
{
public:
    static const UINT LIVETIME_LIMITLESS;

    DebugDrawSphere(
        CrVector3   pos,
        float       size,
        COLORf      color = COLORf(1, 1, 1, 1), 
        UINT        live_time = 1,     //�o������(LIVETIME_LIMITLESS ���w�肷��Ə����Ȃ�)
        CrVector3   move = Vector3Zero //�ړ��x�N�g��
        );

    ~DebugDrawSphere();

private:
    LPIEXMESH           m_pMesh;
    Matrix              m_TransMat;
    const Vector3       m_Move;
    Vector4             m_Color;
    UINT                m_Livetime;
    bool                m_FirstDraw;

    bool Update();
    bool Msg(MsgType mt);

    void CalcZ();
    void Render();
};

#endif