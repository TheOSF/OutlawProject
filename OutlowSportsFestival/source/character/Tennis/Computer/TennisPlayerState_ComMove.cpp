#include "TennisPlayerState_ComMove.h"
#include "../TennisState_Shot.h"
#include "../TennisPlayerState_Counter.h"
#include "../../../GameSystem/GameController.h"
#include "../../CharacterFunction.h"
#include "../../../Ball/UsualBall.h"
#include "../../../Damage/Damage.h"
#include "../../CharacterCounterClass.h"
#include "../../CharacterMoveClass.h"
#include "../../CharacterShotAttackClass.h"
#include "../../CharacterManager.h"
#include "../Tennis_HitEvent.h"
#include "../TennisPlayerState_Attack.h"


//�X�e�[�g�J�n
void TennisState_ComputerControll_Move::Enter(TennisPlayer* t)
{
    //�ړ��C�x���g�N���X
    class TennisMoveEvent :public CharacterUsualMove::MoveEvent
    {
        TennisPlayer* m_pTennis;
    public:
        TennisMoveEvent(TennisPlayer* pTennis) :
            m_pTennis(pTennis){}

        //�A�j���[�V�����̍X�V
        void Update(bool isRun, RATIO speed_ratio)
        {
            m_pTennis->m_Renderer.Update(1);
        }
        //����n�߂Ƀ��[�V�������Z�b�g
        void RunStart()
        {
            m_pTennis->m_Renderer.SetMotion(TennisPlayer::_mt_Run);
        }
        //�����͂��߂Ƀ��[�V�������Z�b�g
        void StandStart()
        {
            m_pTennis->m_Renderer.SetMotion(TennisPlayer::_mt_Stand);
        }
    };

    //�ړ��p�����[�^����
    CharacterUsualMove::Params p;

    p.Acceleration = 0.2f;
    p.MaxSpeed = 0.2f;
    p.TurnSpeed = 0.3f;
    p.DownSpeed = 0.2f;

    //�ړ��N���X�̍쐬
    m_pMoveClass = new CharacterUsualMove(
        t,
        p,
        new TennisMoveEvent(t),
        new TennisHitEvent(t)
        );


    //�ړ��R���g���[���N���X�̍쐬
    if (t->m_PlayerInfo.strong_type == StrongType::__ErrorType)
    {
        m_pMoveControllClass = new SandBagMoveControllClass();
    }
    else
    {
        m_pMoveControllClass = new ComMoveControllClass(t);
    }
    
}


void TennisState_ComputerControll_Move::Execute(TennisPlayer* t)
{
    //�X�e�B�b�N�l���Z�b�g
    m_pMoveClass->SetStickValue(m_pMoveControllClass->Move(t));

    //�X�V
    m_pMoveClass->Update();

    //���f���̃��[���h�ϊ��s����X�V
    chr_func::CreateTransMatrix(t, t->m_ModelSize, &t->m_Renderer.m_TransMatrix);

}

void TennisState_ComputerControll_Move::Exit(TennisPlayer* t)
{
    delete m_pMoveClass;
    delete m_pMoveControllClass;
}

//---------------------------------------------------------------------
//                      SandBagMoveControllClass
//---------------------------------------------------------------------

Vector2 SandBagMoveControllClass::Move(TennisPlayer* t)
{
    //�X�e�B�b�N�̒l�Ƃ��ăR���s���[�^����̈ړ����v�Z����
    const Vector3 Target = DefCharacterMgr.GetRoundStartPos(t->m_PlayerInfo.number);
    Vector3 v = Target - t->m_Params.pos;

    if (v.Length() < 1.0f)
    {
        v = Vector3Zero;
    }

    return Vector2Normalize(Vector2(v.x, v.z));
}

//---------------------------------------------------------------------
//                      ComMoveControllClass
//---------------------------------------------------------------------

ComMoveControllClass::ComMoveControllClass(TennisPlayer* t) :
m_pTennis(t)
{
    GetParams(m_Param, t->m_PlayerInfo.strong_type);
    m_pStateFunc = &ComMoveControllClass::StateStop;
    m_Count = rand() % 20;
}

ComMoveControllClass::~ComMoveControllClass()
{

}

Vector2 ComMoveControllClass::Move(TennisPlayer* t)
{
    //�X�e�B�b�N�̒l�Ƃ��ăR���s���[�^����̈ړ����v�Z����
    const Vector3 Target = DefCharacterMgr.GetRoundStartPos(t->m_PlayerInfo.number);
    Vector3 v = Target - t->m_Params.pos;

    if (v.Length() < 1.0f)
    {
        v = Vector3Zero;
    }

    return Vector2Normalize(Vector2(v.x, v.z));
}


void ComMoveControllClass::GetParams(Param& out, StrongType::Value st)
{
    switch (st)
    {
        //�ア
    case StrongType::_Weak:
        out.BallCounter = 0.3f;
        out.BallCounterSpeed = 0.5f;
        out.BallCounterTec = 0.1f;
        out.RunStop = 0.8f;
        out.RunPlaceTec = 0.1f;
        out.DangerEscape = 0.2f;
        out.ActionFrequence = 0.3f;
        out.NamePlay = 1.0f;
        break;

        //����
    case StrongType::_Usual:
        out.BallCounter = 0.6f;
        out.BallCounterSpeed = 0.7f;
        out.BallCounterTec = 0.7f;
        out.RunStop = 0.4f;
        out.RunPlaceTec = 0.5f;
        out.DangerEscape = 0.5f;
        out.ActionFrequence = 0.6f;
        out.NamePlay = 0.5f;
        break;

        //����
    case StrongType::_Strong:
        out.BallCounter = 1.0f;
        out.BallCounterSpeed = 1.0f;
        out.BallCounterTec = 1.0f;
        out.RunStop = 0.2f;
        out.RunPlaceTec = 1.0f;
        out.DangerEscape = 1.0f;
        out.ActionFrequence = 1.0f;
        out.NamePlay = 0.0f;
        break;

    default:
        MyAssert(false, "�F���ł��Ȃ�strongType��ComMoveControllClass::GetParams�ɑ����܂���");
        break;
    }
}


void ComMoveControllClass::StateMove(Vector2& out)
{
    const float GoalOKlen = 2.0f; //�S�[���Ƃ݂Ȃ�����(�덷�̑Ώ�)

    //�ڕW�ɓ��B���Ă�����Ƃ܂�
    if (Vector3Distance(m_MoveTargetPos, m_pTennis->m_Params.pos) < GoalOKlen)
    {
        m_Count = (int)(m_Param.RunStop * 100.0f);
        m_pStateFunc = &ComMoveControllClass::StateStop;
    }
    

    //�ڕW�ɓ��B�ł��Ȃ� or �V�ڕW������΂����ɕύX����


    //�ڕW�Ɍ������Ĉړ�
}



void ComMoveControllClass::StateStop(Vector2& out)
{

}


Vector3 ComMoveControllClass::GetMoveTargetPos()
{
    Vector3 ret;

    //�e�j�X�@���@���������L���Ȃ̂ŁA
    //�S�L�����N�^��������Ƃ������ꏊ���Z�o����̂�Best

    //�ア�ꍇ�A�߂����������܂ɏo�����Ⴄ
    
    struct TargetInfo
    {
        bool      ok;
        Vector3   pos;
    };

    TargetInfo targets[8];

    const CharacterManager::CharacterMap& ChrMap = DefCharacterMgr.GetCharacterMap();

    for (int i = 0; i < (int)ARRAYSIZE(targets); ++i)
    {
        //�����_���ɍ��W���쐬
        targets[i].pos = Vector3(frand() - 0.5f, 0, frand() - 0.5f) * 50.f;
        targets[i].pos += Vector3Normalize(targets[i].pos) * 5.0f;


        //���̓_���ǂ����ǂ���
        for (auto& it : ChrMap)
        {
            if (it.first == m_pTennis||
                chr_func::isDie(it.first) )
            {
                continue;
            }


        }
    }

    return Vector3Zero;
}