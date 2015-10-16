#pragma once


#include "../../Ball/Ball.h"
#include "../../utillity/Locus.h"
#include "../../Library/Bullet/BulletSystem.h"
#include "../../GameSystem/GameObject.h"
#include "../../Damage/Damage.h"

//*****************************************************
//		�����グ��{�[���N���X(�{�[�������ł͂Ȃ�)
//*****************************************************

class TennisUpBall :public GameObjectBase
{
public:

    //�R���X�g���N�^
    TennisUpBall(
        CrVector3  pos
        );

    ~TennisUpBall();

    bool Update();
    bool Msg(MsgType mt);

    Vector3 GetPos()const;

    bool m_DeleteFlag;
    bool m_DownFlag;

private:

    MeshRenderer*  m_pMeshRenderer;
    Vector3        m_Pos;
    Vector3        m_Move;
    int            m_Timer;

    void MeshUpdate();
};




#include "TennisPlayer.h"
#include "../../Library/Bullet/BulletSystem.h"
#include "../../Damage/Damage.h"

//****************************************************
//	�e�j�X_�{�[������ɏグ��N���X
//****************************************************

class TennisState_SlowUpBall :public TennisState
{
public:
    class ControllClass
    {
    public:
        virtual ~ControllClass(){}
        virtual void AngleControll(TennisPlayer* p) = 0;
        virtual bool SwitchState(TennisState_SlowUpBall* pState) = 0;
    };

    TennisState_SlowUpBall(
        ControllClass* pControllClass   //����R���g���[������N���X(�I������delete����)
        );

    ~TennisState_SlowUpBall();

    void Enter(TennisPlayer* t)override;
    void Execute(TennisPlayer* t)override;
    void Exit(TennisPlayer* t)override;

    RATIO           GetChargePower()const;
    TennisUpBall*   GetSlowUpBall();
    void            DeleteSlowBall();

private:
    ControllClass* const m_pControllClass;
    TennisUpBall*        m_pUpBall;
    int                  m_Timer;
    const int            m_EndFrame;
};

