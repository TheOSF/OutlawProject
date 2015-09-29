#pragma once

//------------------------------------------------------------//
//  �@�@�@�@�@���̍Đ��E�ǂݍ��݂�����֐�
//------------------------------------------------------------//

namespace Sound
{
    enum Type
    {
        Damage1, //�_���[�W�r�d�P
        Damage2, //�_���[�W�r�d�Q
        Damage3, //�_���[�W�r�d�R

        AtkHit1, //�L�[��
        AtkHit2, //�p�[��

        Swing1,  //���S���t�@�ߋ����U����
        Swing2,  //���T�b�J�[�ߋ����U����
        Swing3,  //���e�j�X�@�ߋ����U����

        Impact1, //�Ռ����P(�h��)
        Impact2, //�Ռ����Q(�h�J��)

        Sand1,   //���̉�
        Sand2,   //���̉�(�Y�U�[)

        Counter, //�J�E���^�[��
        Explode, //����(���T�b�J�[�K�E�Z�̔���)
        Skill,   //�X�L���Z����(�V���C�[��)
        Beam1,    //�r�[����(�O��̖싅�K�E�Z)
        Beam2,   //�r�[����(�O��̃T�b�J�[�K�E�Z)

        Tennis_BallAtk,  //�e�j�X�{�[���ł���
        BaseBall_kaki_n, //�싅(�J�L�[��)
        Golf_Swing,      //���S���t�������U��

        Cursor_move,  //�J�[�\���ړ���
        Cursor_enter, //�J�[�\������
        Cursor_cancel,//�J�[�\���L�����Z��

        Gong_Start, //�S���O���X�^�[�g
        Gong_End,   //�S���O���G���h

        Scene_Enter,//�V�[���ڍs��

        // BGM 
        BGM_Battle,
        BGM_LastRound,
    };

    //�ǂݍ���
    void Initialize();

    //�Đ�
    void Play(Type type, RATIO volume = 1, bool loop = false);
}