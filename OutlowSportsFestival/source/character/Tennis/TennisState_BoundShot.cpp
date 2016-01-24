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
    )
{

}

TennisState_BoundShot::~TennisState_BoundShot()
{

}

void TennisState_BoundShot::Enter(TennisPlayer* t)
{
    //�J�E���g������
    m_Timer = 0;

    //���[�V�����Z�b�g
    t->m_Renderer.SetMotion(TennisPlayer::_mt_HeartShot);
}

void TennisState_BoundShot::Execute(TennisPlayer* t)
{
    const int EndFrame = 38; //�I���t���[��
    const int ShotFrame = 25;//�łt���[��
    const int AfterAction = 15;//�V���b�g��̃A�N�V�������\�ɂȂ�t���[��

    //�J�E���g�X�V
    ++m_Timer;

    //�����n�߂̕����␳
    if (m_Timer < 3)
    {
        Vector3 vec = GetVec();
        chr_func::AngleControll(t, t->m_Params.pos + vec);
    }

    //�ړ��l�_�E��
    {
        chr_func::XZMoveDown(t, 0.1f);
    }

    //�����␳
    if (m_Timer < ShotFrame)
    {
        const float AngleSpeed = D3DXToRadian(10);

 
        Vector3 vec = GetVec();
        chr_func::AngleControll(t, t->m_Params.pos + vec, AngleSpeed);
        
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
        chr_func::CreateTransMatrix(t, &t->m_Renderer.m_TransMatrix);
    }
}

void TennisState_BoundShot::Exit(TennisPlayer* t)
{
	
}

Vector3 TennisState_BoundShot::GetVec()
{
    return Vector3Zero;
}