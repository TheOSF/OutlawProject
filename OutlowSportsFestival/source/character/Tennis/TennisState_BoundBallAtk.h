#pragma once

#include "TennisPlayer.h"

//****************************************************
//	�e�j�X_�{�[���o�E���h�U���N���X
//****************************************************

class TennisState_BoundBallAtk :public TennisState
{
public:
    class ControllClass
    {
    public:
        virtual ~ControllClass(){}
        virtual Vector3 GetBoundVec() = 0;
    };

    TennisState_BoundBallAtk(
        ControllClass* pControllClass   //�o�E���h�������R���g���[������N���X(�I������delete����)
        );

    ~TennisState_BoundBallAtk();

    void Enter(TennisPlayer* t)override;
    void Execute(TennisPlayer* t)override;
    void Exit(TennisPlayer* t)override;

private:
    ControllClass* const m_pControllClass;
    int                  m_Timer;
};


//****************************************************
//	�e�j�X_�o�E���h����{�[���N���X
//****************************************************
#include "../../Ball/Ball.h"
#include "../../GameSystem/GameObject.h"
#include "../../GameSystem/ForwardDecl.h"
#include "../../utillity/Locus.h"
#include "../../Render/LightObject.h"

class TennisBoundBall :public GameObjectBase, public BallBase
{
public:

    //�R���X�g���N�^
    TennisBoundBall(
        BallBase::Params	params,			//�{�[���p�����[�^
        float               glavity,        //�d��
        CrVector3           AtkVec          //�o�E���h�������
        );

    ~TennisBoundBall();

    bool Update();
    bool Msg(MsgType mt);

private:

    LpMeshRenderer		m_pMeshRenderer;
    Locus               m_Locus;
    D3DXQUATERNION      m_BallRot;
    const Vector3       m_MoveVec;
    const float         m_Glavity;
    int                 m_Timer;
    void(TennisBoundBall::*m_pStateFunc)();

    void UpdateTransMatrix(RATIO scaleYratio);
    void GetTransMatrix(Matrix& m, RATIO scaleYratio);
    void RotateY();

    void State_Flying();
    void State_Failed();
    void State_Rolling();
    void State_AtkStart();
    void State_Finish(){}
};
