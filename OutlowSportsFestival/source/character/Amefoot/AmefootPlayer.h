#pragma once

#include "../CharacterBase.h"
#include "../CharacterStateTemplate.h"
#include "../CharacterRenderer.h"

class AmefootPlayer;
typedef ChrStateMachine<AmefootPlayer, CharacterBase::MsgType> AmefootStateMachine;
typedef ChrState<AmefootPlayer, CharacterBase::MsgType> AmefootState;


//-----------------------------------------------------------------------------------------
// AmefootPlayer
// [ �A���t�g ] �L�����N�^�[�N���X
//-----------------------------------------------------------------------------------------
class AmefootPlayer : public CharacterBase
{
public:
    AmefootPlayer(const PlayerInfo& info);

    ~AmefootPlayer();

    bool Update()override;

    bool CharacterMsg(MsgType msg)override;

    bool SetState(AmefootState* state, int important = 0);

    void SetDontThrowBallTimer(int Frame);

    bool isCanThrowBall()const;

public:
    enum MotionType
    {
        Motion_Zero = 0,
        Motion_Stand = 3, // �ҋ@
        Motion_Run = 4, // ����
        Motion_Run_End = 6, // ����I���
        Motion_Evasion_Start = 7, // ������Ⴊ��
        Motion_Evasion_ViewAround = 8,//���������낫���
        Motion_Evasion_Standup = 9, // ����N���オ��
        Motion_Throw_Ball = 10, // �{�[������
        Motion_Tackle_Save_Power = 12, // �^�b�N������
        Motion_Tackle_Charge = 13, // �^�b�N���ːi
        Motion_Tackle_Drive_Away = 14, // �^�b�N���ł��グ
        Motion_Tackle_Touchdown = 16, // �^�b�N���^�b�`�_�E��
        Motion_Tackle_Touchdown_andStand = 17, // �^�b�N���^�b�`�_�E��
        //  Motion_Tackle_Standup, // �^�b�N���N���オ��
        Motion_Counter_Pose = 19, // �J�E���^�[�\��
        Motion_Counter_Catch_Right = 21, // �J�E���^�[�E�L���b�`
        Motion_Counter_Catch_Left = 23, // �J�E���^�[���L���b�`
        Motion_Counter_Throw_Ball = 25, // �J�E���^�[����
        Motion_Counter_Failed = 38 , // �J�E���^�[���s  ------------------------
        Motion_Damage_Weak = 40, // ��U���_���[�W ------------------------
        Motion_Damage_Vanish_Start = 28, // ������у_���[�W�J�n
        Motion_Damage_Vanish_Landing = 29, // ������у_���[�W���n
        Motion_Damage_Vanish_Standup = 30, // ������у_���[�W�N���オ��
        Motion_Damage_Vanish_Hit_Wall_Fall = 32, // ������у_���[�W�Ǘ���
        Motion_Damage_Vanish_Hit_Wall_Landing = 33, // ������у_���[�W�ǒn�ʂł͂˂�
        Motion_Damage_Vanish_Hit_Wall_Standup = 34, // ������у_���[�W�ǋN���オ��
        Motion_Touchdown_Start = 36, // �ŗL�Z�J�n
        Motion_Touchdown_Finish = 37, // �ŗL�Z���݂͂���t�B�j�b�V���܂�
        Motion_Touchdown_Failed = 38, // �ŗL�Z���s
        Motion_Touchdown_Failed_Bounced = 40, // �ŗL�Z���s �͂����Ԃ����
        Motion_Special_Attack_Start = 42, // �K�E�Z�J�n
        Motion_Special_Attack_Running = 43, // �K�E�Z����
        Motion_Special_Attack_Finish = 44 , // �K�E�Z�t�B�j�b�V��

        Motion_Win_Pose = 0, // �����|�[�Y
        Motion_Lose_Pose = 0, // �����|�[�Y
    };

private:

    AmefootStateMachine*    m_pStateMachine;
    int                     m_DontThrowBallTimer;

};
//-----------------------------------------------------------------------------------------

