#include "TennisState_BoundShot.h"
#include "TennisPlayerState_UsualMove.h"
#include "Tennis_HitEvent.h"
#include "../../GameSystem/GameController.h"
#include "../CharacterFunction.h"
#include "../../Ball/UsualBall.h"
#include "../../Damage/Damage.h"
#include "../CharacterMoveClass.h"
#include "../CharacterShotAttackClass.h"
#include "../CharacterManager.h"
#include "TennisPlayerState_UsualMove.h"

#include "../../Effect/EffectFactory.h"
#include "../../Effect/HitEffectObject.h"
#include "../../Sound/Sound.h"
#include "../../GameSystem/GameController.h"
#include "TennisState_BoundBall.h"
#include"TennisHeartBall.h"

TennisState_BoundShot::TennisState_BoundShot(
    ControllClass*       pControllClass //�I������delete����
    ):
    m_pControllClass(pControllClass)
{

}

TennisState_BoundShot::~TennisState_BoundShot()
{
    delete m_pControllClass;
}

void TennisState_BoundShot::Enter(TennisPlayer* t)
{
    //�J�E���g������
    m_Timer = 0;

    //���[�V�����Z�b�g
    t->m_Renderer.SetMotion(TennisPlayer::_mt_Shot);
}

void TennisState_BoundShot::Execute(TennisPlayer* t)
{
    const int EndFrame = 38; //�I���t���[��
    const int ShotFrame = 9;//�łt���[��
    const int AfterAction = 15;//�V���b�g��̃A�N�V�������\�ɂȂ�t���[��

    //�J�E���g�X�V
    ++m_Timer;

    //�����n�߂̕����␳
    if (m_Timer < 3)
    {
        Vector3 vec = m_pControllClass->GetVec();
        chr_func::AngleControll(t, t->m_Params.pos + vec);
    }

    //�ړ��l�_�E��
    {
        chr_func::XZMoveDown(t, 0.1f);
    }

    //�V���b�g��̃A�N�V����
    if (m_Timer > AfterAction && m_Timer < EndFrame)
    {
        m_pControllClass->DoShotAfterAction();
    }

    //�����␳
    if (m_Timer < ShotFrame)
    {
        const float AngleSpeed = D3DXToRadian(10);

        
            Vector3 vec = m_pControllClass->GetVec();
            chr_func::AngleControll(t, t->m_Params.pos + vec, AngleSpeed);
        
    }
    
    //�T�E���h
    if (m_Timer == ShotFrame - 3)
    {
        Sound::Play(Sound::Tennis_BallAtk);
    }
    

    //�łI
    if (m_Timer == ShotFrame)
    {
        Vector3 pos, move;

        //�L�����̏ꏊ��
        pos = t->m_Params.pos;
        //�������L�������ʃ{�[�����˂�Y��
        pos.y = BallBase::UsualBallShotY;


        //�ړ��͑O����
        chr_func::GetFront(t, &move);
        //�X�s�[�h�͓K��
        move *= 0.2f;

        new TennisHeartBall(pos, move, t);
         

        /*
        //�{�[������
        BallBase::Params param;

        //�ړ��͑O����
        chr_func::GetFront(t, &param.move);
        //�X�s�[�h�͓K��
        param.move *= 0.3f;

        //�L�����̏ꏊ��(�ŏI�I�ɘr�̈ʒu�ɁH)
        param.pos = t->m_Params.pos;
        //�������L�������ʃ{�[�����˂�Y��
        param.pos.y = BallBase::UsualBallShotY;

        //�e��������
        param.pParent = t;
        //�ʏ�^�C�v
        param.type = BallBase::Type::_Usual;

        //����
        new UsualBall(param, DamageBase::Type::_WeekDamage, 5, 2, 80);

        */
        
        /*


        

        //�{�[������
        Vector3 pos, move;

        pos = t->m_Params.pos + Vector3(0, BallBase::UsualBallShotY, 0) + chr_func::GetFront(t)*0.8f;

        chr_func::GetFront(t, &move);
        move *= 0.4f;
        move.y = 0.3f;

        new TennisBoundBall(
            pos,
            move,
            t
            );

        //�R���g���[����U��
        chr_func::SetControllerShock(
            t,
            0.5f,
            0.15f
            );

        //�G�t�F�N�g
        {
            COLORf EffectColor(CharacterBase::GetPlayerColor(t->m_PlayerInfo.number));

            //�G�t�F�N�g�̐ݒ�
            new HitEffectObject(
                pos,
                Vector3Normalize(move),
                0.08f,
                0.03f,
                Vector3(EffectColor.r, EffectColor.g, EffectColor.b),
                1,
                0
                );
        }

        //���Ɉړ�
        chr_func::AddMoveFront(t, 0.25f, 0.5f);

        */
    }

    //�X�e�[�g�I��
    if (m_Timer > EndFrame)
    {
        t->SetState(TennisState_PlayerControll_Move::GetPlayerControllMove(t));
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

void TennisState_BoundShot::Exit(TennisPlayer* t)
{
	
}