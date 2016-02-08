#include "AmefootPlayerState_Die.h"
#include "../AmefootUsualHitEvent.h"
#include "../../CharacterFunction.h"
#include "../../../Effect/HitEffectObject.h"
#include "../../../Ball/Ball.h"
#include "../../../Camera/Camera.h"
#include "../../../Effect/GlavityLocus.h"
#include "../../../Effect/EffectFactory.h"
#include "../../../GameSystem/GameController.h"
#include "../../CharacterDamageVanish.h"


AmefootPlayerState_Die::AmefootPlayerState_Die(
    AmefootPlayer* pAmefootPlayer,
    const Vector3& Damage_vec
    ) :
    m_pAmefootPlayer(pAmefootPlayer),
    m_Damage_vec(Damage_vec)
{}

void AmefootPlayerState_Die::Enter(AmefootPlayer* pCharacter)
{
    //�L�����N�^���ʂЂ�݃N���X�̃e�j�X�ŗL�C�x���g�N���X
    class AmefootEvent :public CharacterDamageVanish::Event
    {
    public:
        AmefootEvent(AmefootPlayer* pAmefootPlayer) :m_pAmefootPlayer(pAmefootPlayer) {}

        void FlyStart()
        {
            //������у��[�V�������Z�b�g
            m_pAmefootPlayer->m_Renderer.SetMotion(AmefootPlayer::Motion_Damage_Vanish_Start);
        }

        void Flying(const Matrix& Rotate, RATIO t)
        {
            //���f���̃A�j���[�V�����X�V
            m_pAmefootPlayer->m_Renderer.Update(t);

            //�ʒu�ɂ��Ƃ����A���[���h�ϊ��s����v�Z
            chr_func::CreateTransMatrix(m_pAmefootPlayer, &m_pAmefootPlayer->m_Renderer.m_TransMatrix);

            //������т̉�]������
            m_pAmefootPlayer->m_Renderer.m_TransMatrix = Rotate*m_pAmefootPlayer->m_Renderer.m_TransMatrix;
        }

        void DownStart()
        {
            //�_�E�����[�V�������Z�b�g
            m_pAmefootPlayer->m_Renderer.SetMotion(AmefootPlayer::Motion_Damage_Vanish_Landing);
        }

        void Downing()
        {
            m_pAmefootPlayer->m_Renderer.Update(1);

            //���[���h�ϊ��s����v�Z
            chr_func::CreateTransMatrix(m_pAmefootPlayer, &m_pAmefootPlayer->m_Renderer.m_TransMatrix);

        }

        void StandUpStart()
        {
            //�������Ȃ�
        }

        void StandUping()
        {
            //���f���̃A�j���[�V�����X�V
            m_pAmefootPlayer->m_Renderer.Update(1);

            //���[���h�ϊ��s����v�Z
            chr_func::CreateTransMatrix(m_pAmefootPlayer, &m_pAmefootPlayer->m_Renderer.m_TransMatrix);
        }

        void End()
        {
            //�J�����ʂ��t���O��false��
            m_pAmefootPlayer->m_DrawObject.m_isDraw = false;
        }
        void HitWall()
        {
            //�ǂɓ����������[�V�������Z�b�g
            m_pAmefootPlayer->m_Renderer.SetMotion(AmefootPlayer::Motion_Damage_Vanish_Hit_Wall_Fall);
        }

        void HitFloor()
        {
            //���ɓ����������[�V�������Z�b�g
            m_pAmefootPlayer->m_Renderer.SetMotion(AmefootPlayer::Motion_Damage_Vanish_Hit_Wall_Landing);
        }

        void HitFloorAndStandUp()
        {
            //�������Ȃ�(�����オ��Ȃ�
        }

        void HitWallUpdate()
        {
            //���f���̃A�j���[�V�����X�V
            m_pAmefootPlayer->m_Renderer.Update(1);

            //���[���h�ϊ��s����v�Z
            chr_func::CreateTransMatrix(m_pAmefootPlayer, &m_pAmefootPlayer->m_Renderer.m_TransMatrix);
        }
    private:
        AmefootPlayer*  m_pAmefootPlayer;
    };

    //�_���[�W���[�V�����p�����[�^�[���쐬����
    CharacterDamageVanish::Param  Param;

    Param.rotate_speed = Vector3(0.0f, 0.0f, 0.0f);

    Param.move = m_Damage_vec;
    Param.down_frame = 10;
    Param.standup_frame = 50;

    //�Ђ�݃N���X���쐬
    m_pDamageVanishClass = new CharacterDamageVanish(
        pCharacter,
        Param,
        new AmefootEvent(pCharacter),
        new DamageManager::HitEventBase()
        );

    //���S�G�t�F�N�g
    EffectFactory::DieEffect(
        pCharacter,
        m_Damage_vec
        );

    //�R���g���[����U��
    chr_func::SetControllerShock(
        m_pAmefootPlayer,
        0.8f,
        0.5f
        );
}

void AmefootPlayerState_Die::Execute(AmefootPlayer* pCharacter)
{
    m_pDamageVanishClass->Update();
}

void AmefootPlayerState_Die::Exit(AmefootPlayer* pCharacter)
{
    delete m_pDamageVanishClass;
}


