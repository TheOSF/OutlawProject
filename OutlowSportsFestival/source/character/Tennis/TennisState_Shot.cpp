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

#include "../../Effect/EffectFactory.h"
#include "../../Effect/HitEffectObject.h"
#include "../../Sound/Sound.h"
#include "../../GameSystem/GameController.h"


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
}

void TennisState_Shot::Execute(TennisPlayer* t)
{
    const int EndFrame = 38; //�I���t���[��
    const int ShotFrame = 9;//�łt���[��
    const int AfterAction = 15;//�V���b�g��̃A�N�V�������\�ɂȂ�t���[��

    //�J�E���g�X�V
    ++m_Timer;

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
        const CharacterBase* const pTargetCharacter = GetFrontTarget(t);
        const float AngleSpeed = D3DXToRadian(3);

        if (pTargetCharacter != nullptr)
        {
            //������]
            chr_func::AngleControll(t, pTargetCharacter->m_Params.pos, AngleSpeed * 2.0f);
        }
        else
        {   
            Vector3 vec = m_pControllClass->GetVec();
            chr_func::AngleControll(t, t->m_Params.pos + vec, AngleSpeed);
        }
    }
    
    //�T�E���h
    if (m_Timer == ShotFrame - 3)
    {
        Sound::Play(Sound::Tennis_BallAtk);
    }
    

    //�łI
    if (m_Timer == ShotFrame)
    {

        //�{�[������
        BallBase::Params param;

        //�ړ��͑O����
        chr_func::GetFront(t, &param.move);
        //�X�s�[�h�͓K��
        param.move *= 0.55f;

        //�L�����̏ꏊ��(�ŏI�I�ɘr�̈ʒu�ɁH)
        param.pos = t->m_Params.pos;
        //�������L�������ʃ{�[�����˂�Y��
        param.pos.y = BallBase::UsualBallShotY;

        //�e��������
        param.pParent = t;
        //�ʏ�^�C�v
        param.type = BallBase::Type::_Usual;

        //����
        new UsualBall(param, DamageBase::Type::_WeekDamage, 5);


        //�R���g���[����U��
        controller::SetVibration(
            5000,
            0.15f,
            t->m_PlayerInfo.number
            );

        //�G�t�F�N�g
        {
            COLORf EffectColor(CharacterBase::GetPlayerColor(t->m_PlayerInfo.number));

            //�G�t�F�N�g�̐ݒ�
            new HitEffectObject(
                param.pos,
                Vector3Normalize(param.move),
                0.08f,
                0.03f,
                Vector3(EffectColor.r, EffectColor.g, EffectColor.b),
                1,
                0
                );


            //�G�t�F�N�g�̐ݒ�
            new HitEffectObject(
                param.pos,
                Vector3Normalize(param.move),
                0.08f,
                0.02f,
                Vector3(1, 1, 1),
                0,
                10.0f
                );
        }
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

void TennisState_Shot::Exit(TennisPlayer* t)
{
	
}


const CharacterBase* TennisState_Shot::GetFrontTarget(TennisPlayer* t)const
{
    CharacterManager::CharacterMap ChrMap = DefCharacterMgr.GetCharacterMap();

    const CharacterBase* pTargetEnemy = nullptr;    //�^�[�Q�b�g�ێ��̃|�C���^
    RADIAN MostMinAngle = D3DXToRadian(45);         //�����Ƃ������p�x
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