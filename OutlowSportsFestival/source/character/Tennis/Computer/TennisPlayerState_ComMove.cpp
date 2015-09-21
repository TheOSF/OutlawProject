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
}


void TennisState_ComputerControll_Move::Execute(TennisPlayer* t)
{
    {
        //�X�e�B�b�N�̒l�Ƃ��ăR���s���[�^����̈ړ����v�Z����
        const Vector3 Target = DefCharacterMgr.GetRoundStartPos(t->m_PlayerInfo.number);
        Vector3 v = Target - t->m_Params.pos;

        if (v.Length() < 1.0f)
        {
            v = Vector3Zero;
        }

        //�X�e�B�b�N�̒l�Z�b�g
        m_pMoveClass->SetStickValue(Vector2Normalize(Vector2(v.x, v.z)));

    }
    //�X�V
    m_pMoveClass->Update();

    //���f���̃��[���h�ϊ��s����X�V
    chr_func::CreateTransMatrix(t, t->m_ModelSize, &t->m_Renderer.m_TransMatrix);

}

void TennisState_ComputerControll_Move::Exit(TennisPlayer* t)
{
    delete m_pMoveClass;
}


