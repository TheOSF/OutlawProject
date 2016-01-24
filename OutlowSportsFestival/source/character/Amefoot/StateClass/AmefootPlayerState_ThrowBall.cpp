#include "AmefootPlayerState_ThrowBall.h"
#include "AmefootPlayerState_UsualMove.h"
#include "../AmefootUsualHitEvent.h"
#include "../../../GameSystem/GameController.h"
#include "../../CharacterFunction.h"
#include "../../../Ball/UsualBall.h"
#include "../../../Damage/Damage.h"
#include "../../CharacterMoveClass.h"
#include "../../CharacterShotAttackClass.h"
#include "../../CharacterManager.h"
#include "../../../Effect/EffectFactory.h"
#include "../../../Effect/HitEffectObject.h"
#include "../../../Sound/Sound.h"
#include "../../../Ball/AmefootBoundBall.h"



//-----------------------------------------------------------------------------------------
// AmefootPlayerState_ThrowBall
// [ �A���t�g ] �������U��(�{�[������)������X�e�[�g�N���X
//-----------------------------------------------------------------------------------------
AmefootPlayerState_ThrowBall::AmefootPlayerState_ThrowBall() 
{}
//-----------------------------------------------------------------------------------------
AmefootPlayerState_ThrowBall::~AmefootPlayerState_ThrowBall()
{
    
}
//-----------------------------------------------------------------------------------------
void AmefootPlayerState_ThrowBall::Enter(AmefootPlayer* pCharacter)
{
    //�J�E���g������
    m_Timer = 0;

    //���[�V�����Z�b�g
    pCharacter->m_Renderer.SetMotion(AmefootPlayer::Motion_Throw_Ball);

    //���ĂȂ��^�C�}�[�Z�b�g
    pCharacter->SetDontThrowBallTimer(50);
}
//-----------------------------------------------------------------------------------------
void AmefootPlayerState_ThrowBall::Execute(AmefootPlayer* pCharacter)
{
    const int EndFrame = 21; //�I���t���[��
    const int ShotFrame = 10;//�łt���[��
    const int AfterAction = 15;//�V���b�g��̃A�N�V�������\�ɂȂ�t���[��
    
    //�J�E���g�X�V
    ++m_Timer;

    //�ړ��l�_�E��
    {
        chr_func::XZMoveDown(pCharacter, 0.1f);
    }

    //�T�E���h
    if ( m_Timer == ShotFrame - 3 )
    {
        //Sound::Play(Sound::Tennis_BallAtk);
    }

    //�łI
    if ( m_Timer == ShotFrame )
    {
        Vector3 pos, move;

        pos = pCharacter->m_Params.pos + Vector3(0, BallBase::UsualBallShotY, 0) + chr_func::GetFront(pCharacter)*0.8f;

        chr_func::GetFront(pCharacter, &move);
        move *= 0.35f;
        move.y = 0.7f;

        new AmefootBoundBall(
        pos,
        move,
        pCharacter
        );

        //�R���g���[����U��
        chr_func::SetControllerShock(
            pCharacter,
            0.5f,
            0.15f
            );

        //�G�t�F�N�g
        {
        COLORf EffectColor(CharacterBase::GetPlayerColor(pCharacter->m_PlayerInfo.number));

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
        //chr_func::AddMoveFront(pCharacter, 0.25f, 0.5f);
    }

    //�X�e�[�g�I��
    if ( m_Timer > EndFrame )
    {
        pCharacter->SetState(AmefootPlayerState_UsualMove::GetPlayerControllMove(pCharacter));
    }

    //��{�I�ȍX�V
    {
        AmefootUsualHitEvent HitEvent(pCharacter);
        chr_func::UpdateAll(pCharacter, &HitEvent);

        //���f���֘A�̍X�V
        pCharacter->m_Renderer.Update(2.0f);
        chr_func::CreateTransMatrix(pCharacter, &pCharacter->m_Renderer.m_TransMatrix);
    }
}
//-----------------------------------------------------------------------------------------
void AmefootPlayerState_ThrowBall::Exit(AmefootPlayer* pCharacter)
{}
//-----------------------------------------------------------------------------------------


