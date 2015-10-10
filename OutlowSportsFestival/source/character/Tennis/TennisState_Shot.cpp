#include "TennisState_Shot.h"
#include "TennisPlayerState_UsualMove.h"
#include "Tennis_HitEvent.h"
#include "../../GameSystem/GameController.h"
#include "../CharacterFunction.h"
#include "../../Ball/UsualBall.h"
#include "../../Damage/Damage.h"
#include "../CharacterCounterClass.h"
#include "../CharacterMoveClass.h"
#include "../CharacterShotAttackClass.h"
#include "../CharacterManager.h"
#include "TennisPlayerState_UsualMove.h"

#include "../../Effect/HitEffectObject.h"
#include "../../Sound/Sound.h"


TennisState_Shot::TennisState_Shot(
    ControllClass*       pControllClass //�I������delete����
    ):
    m_pControllClass(pControllClass)
{

}

TennisState_Shot::~TennisState_Shot()
{
    delete m_pControllClass;
}

void TennisState_Shot::Enter(TennisPlayer* t)
{
    //�J�E���g������
    m_Timer = 0;

    //���[�V�����Z�b�g
    t->m_Renderer.SetMotion(TennisPlayer::_mt_Shot);

    {
        //�ړ��ʐ���
        const float MaxMove = 0.06f;
        
        if (t->m_Params.move.Length() > MaxMove)
        {
            t->m_Params.move.Normalize();
            t->m_Params.move *= MaxMove;
        }
    }

    {
        //�ł��グ�{�[������
        BallBase::Params param;

        //�ړ��͏����
        param.move = t->m_Params.move;
        param.move.y = 0.5f;
        
        //�L�����̏ꏊ��(�ŏI�I�ɘr�̈ʒu�ɁH)
        param.pos = t->m_Params.pos + Vector3(0, 2.0f, 0);
        //�e��������
        param.pParent = t;
        //�J�E���^�[�ł��Ȃ��^�C�v
        param.type = BallBase::Type::_CantCounter;

        //����
        m_pUpBall = new PhysicallyMoveBall(param, DamageBase::Type::_WeekDamage, 1, -0.025f);
   
        //�ł��グ���͂����蔻��Ȃ�
        m_pUpBall->m_Damage.m_Enable = false;
    }
}

void TennisState_Shot::Execute(TennisPlayer* t)
{
    const int EndFrame = 48; //�I���t���[��
    const int ShotFrame = 30;//�łt���[��
    const int CancelStart = 10;//�L�����Z���s���\�ȃt���[��
    const int AfterAction = 33;//�V���b�g��̃A�N�V�������\�ɂȂ�t���[��

    //�J�E���g�X�V
    ++m_Timer;

    //�ł��L�����Z��
    if (m_Timer > CancelStart && m_Timer < ShotFrame - 3)
    {        
        if (m_pUpBall != nullptr && m_pControllClass->DoOtherAction())
        {
            m_pUpBall->m_Damage.m_Enable = true;
            m_pUpBall->m_Params.type = BallBase::Type::_CantCounter;
        }
    }

    //�V���b�g��̃A�N�V����
    if (m_Timer > AfterAction && m_Timer < EndFrame)
    {
        m_pControllClass->DoShotAfterAction();
    }

    //�����␳
    if (m_Timer < ShotFrame)
    {
        const CharacterBase* const pTargetCharacter = GetFrontTarget(t);
        const float AngleSpeed = D3DXToRadian(3);

        if (pTargetCharacter != nullptr)
        {
            //������]
            chr_func::AngleControll(t, pTargetCharacter->m_Params.pos, AngleSpeed*2.0f);
        }
        else
        {   
            Vector3 vec = m_pControllClass->GetVec();
            chr_func::AngleControll(t, t->m_Params.pos + vec, AngleSpeed);
        }
    }

    //�ł��������{�[�����������̏ꍇ�_���[�W�����L����
    if (m_pUpBall != nullptr&&
        m_pUpBall->m_Params.move.y < 0)
    {
        m_pUpBall->m_Damage.m_Enable = true;
        m_pUpBall->m_Params.type = BallBase::Type::_CantCounter;
    }
    
    //�T�E���h
    if (m_Timer == ShotFrame - 3)
    {
        Sound::Play(Sound::Tennis_BallAtk);
    }
    

    //�łI
    if (m_Timer == ShotFrame)
    {
        //�ł��������{�[���̏�ԃ`�F�b�N
        if (
            m_pUpBall != nullptr&&
            m_pUpBall->m_Params.pParent == t&&
            m_pUpBall->m_Params.type == BallBase::Type::_CantCounter
            )
        {
            
            //�{�[������
            BallBase::Params param;
            
            //�ړ��͑O����
            chr_func::GetFront(t, &param.move);
            //�X�s�[�h�͓K��
            param.move *= 0.6f;

            //�L�����̏ꏊ��(�ŏI�I�ɘr�̈ʒu�ɁH)
            param.pos = m_pUpBall->m_Params.pos;
            //�������L�������ʃ{�[�����˂�Y��
            param.pos.y = BallBase::UsualBallShotY;	

            //�e��������
            param.pParent = t;
            //�ʏ�^�C�v
            param.type = BallBase::Type::_Usual;
            
            //����
            new UsualBall(param, DamageBase::Type::_WeekDamage, 5);


            //�G�t�F�N�g
            {
                COLORf EffectColor(CharacterBase::GetPlayerColor(t->m_PlayerInfo.number));

                //�G�t�F�N�g�̐ݒ�
                new HitEffectObject(
                    param.pos,
                    Vector3Normalize(param.move),
                    0.1f,
                    0.1f,
                    Vector3(EffectColor.r, EffectColor.g, EffectColor.b)
                    );
            }

            //��ɏグ���{�[��������
            m_pUpBall->m_DeleteFlag = true;
            m_pUpBall = nullptr;
        }
    }

    //�X�e�[�g�I��
    if (m_Timer > EndFrame)
    {
        t->SetState(TennisState_PlayerControll_Move::GetPlayerControllMove(t));
    }

    //�ł��I����Ă����ꍇ
    if (m_Timer > ShotFrame)
    {
        chr_func::XZMoveDown(t, 0.1f);
    }

    //��{�I�ȍX�V
    {
        TennisHitEvent HitEvent(t);
        chr_func::UpdateAll(t, &HitEvent);

        //���f���֘A�̍X�V
        t->m_Renderer.Update(1);
        chr_func::CreateTransMatrix(t, t->m_ModelSize, &t->m_Renderer.m_TransMatrix);
    }
}

void TennisState_Shot::Exit(TennisPlayer* t)
{
	
}


const CharacterBase* TennisState_Shot::GetFrontTarget(TennisPlayer* t)const
{
    CharacterManager::CharacterMap ChrMap = DefCharacterMgr.GetCharacterMap();

    const CharacterBase* pTargetEnemy = nullptr;    //�^�[�Q�b�g�ێ��̃|�C���^
    RADIAN MostMinAngle = D3DXToRadian(33);         //�����Ƃ������p�x
    RADIAN TempAngle;

    Vector3 MyFront;      //���g�̑O���x�N�g��
    chr_func::GetFront(t, &MyFront);

    auto it = ChrMap.begin();

    while (it != ChrMap.end())
    {
        //���g�����O
        if (t->m_PlayerInfo.number == it->first->m_PlayerInfo.number ||
            chr_func::isDie(it->first))
        {
            ++it;
            continue;
        }

        //�O�x�N�g���ƓG�ւ̃x�N�g���̊p�x���v�Z����
        TempAngle = Vector3Radian(MyFront, (it->first->m_Params.pos - t->m_Params.pos));

        //�p�x����ԋ���������X�V
        if (TempAngle < MostMinAngle)
        {
            pTargetEnemy = it->first;
            MostMinAngle = TempAngle;
        }

        ++it;
    }
    return pTargetEnemy;
}