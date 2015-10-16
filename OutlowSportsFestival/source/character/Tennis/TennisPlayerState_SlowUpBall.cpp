#include "TennisPlayerState_SlowUpBall.h"
#include "../CharacterFunction.h"
#include "Tennis_HitEvent.h"
#include "../../Render/MeshRenderer.h"
#include "../../Ball/UsualBall.h"


//*****************************************************
//		�����グ��{�[���N���X
//*****************************************************

TennisUpBall::TennisUpBall(
    CrVector3  pos
    ) :
    m_Pos(pos),
    m_Move(0, 1, 0),
    m_DeleteFlag(false),
    m_DownFlag(false),
    m_Timer(0)
{
    {
        iexMesh* pMesh = nullptr;

        UsualBall::GetBallMesh(CharacterType::_Tennis, &pMesh);

        m_pMeshRenderer = new MeshRenderer(
            pMesh,
            false,
            MeshRenderer::RenderType::UseColorSpecular
        );
    }

    //���b�V�������X�V
    MeshUpdate();
}


TennisUpBall::~TennisUpBall()
{
    delete m_pMeshRenderer;
}


bool TennisUpBall::Update()
{
    const int UpFrame = 5;
    const int DownFrame = 60;


    if (UpFrame < m_Timer&&
        m_Timer < DownFrame)
    {

    }

    //�d�͒���

    //���b�V���X�V
    MeshUpdate();

    return !m_DeleteFlag;
}

bool TennisUpBall::Msg(MsgType mt)
{
    return false;
}

void TennisUpBall::MeshUpdate()
{

}

//****************************************************
//	�e�j�X_�{�[������ɏグ��N���X
//****************************************************


TennisState_SlowUpBall::TennisState_SlowUpBall(
    ControllClass* pControllClass   //����R���g���[������N���X(�I������delete����)
    ) :
    m_pControllClass(pControllClass),
    m_Timer(0),
    m_EndFrame(80)
{

}


TennisState_SlowUpBall::~TennisState_SlowUpBall()
{

}


void TennisState_SlowUpBall::Enter(TennisPlayer* t)
{
    t->m_Renderer.SetMotion(TennisPlayer::_mt_Smash);
}

void TennisState_SlowUpBall::Execute(TennisPlayer* t)
{
    const int SlowFrame = 3;
    const int CanSwitchFrame = SlowFrame + 2;


    if (m_Timer > CanSwitchFrame)
    {
        m_pControllClass->SwitchState(this);
    }

    if (m_Timer == SlowFrame)
    {
        //new TennisUpBall(

        //    );
    }


    TennisHitEvent HitEvent(t);

    chr_func::UpdateAll(t, &HitEvent);
    chr_func::CreateTransMatrix(t, t->m_ModelSize, &t->m_Renderer.m_TransMatrix);
    
    t->m_Renderer.Update(1);
}

void TennisState_SlowUpBall::Exit(TennisPlayer* t)
{

}





RATIO TennisState_SlowUpBall::GetChargePower()const
{
    return (float)m_Timer / (float)m_EndFrame;
}

TennisUpBall* TennisState_SlowUpBall::GetSlowUpBall()
{
    return m_pUpBall;
}


void TennisState_SlowUpBall::DeleteSlowBall()
{
    delete m_pUpBall;
    m_pUpBall = nullptr;
}