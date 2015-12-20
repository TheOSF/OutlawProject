#pragma once

#include "TennisPlayer.h"

#include "../Computer/CharacterComputerDoAction.h"
#include "../Computer/CharacterComputerReaction.h"

#include <bitset>

//----------------------------------------------------------------//
//  �e�j�X�L�����N�^�̃L�����Z���A�N�V�����̃Z�b�g�N���X
//----------------------------------------------------------------//


//�x�[�X�N���X
class Tennis_DoCancelAction
{
public:
    enum Action
    {
        NearAtk       ,//�ߋ����U��
        FarAtk        ,//�������U��
        Step          ,//�X�e�b�v(���
        SkillAtk      ,//�K�E�Z
        BoundBallShot ,//����Z
        Counter       ,//�J�E���^�[

        _MaxAction
    };

    Tennis_DoCancelAction(TennisPlayer* pTennis);
    virtual ~Tennis_DoCancelAction(){}

    void SetDontActions(Action ac);    //���s�ł��Ȃ��A�N�V�����̃r�b�g���Z�b�g����
    bool isCanAction(Action ac)const;  //�����̃A�N�V���������s�ł��邩�ǂ���

    virtual bool DoAction() = 0;       //�A�N�V�������s(�߂�l�F���̃A�N�V���������s�������ǂ���)

protected:
    TennisPlayer* const m_pTennis;

private:
    std::bitset<_MaxAction> m_Flags;

};


//�v���C���[�p
class Tennis_DoCancelAction_Player :public Tennis_DoCancelAction
{
public:
    Tennis_DoCancelAction_Player(TennisPlayer* pTennis);
    bool DoAction()override;
};

//�R���s���[�^�[�p
class Tennis_DoCancelAction_Computer :public Tennis_DoCancelAction
{
public:

    Tennis_DoCancelAction_Computer(TennisPlayer* pTennis);
    ~Tennis_DoCancelAction_Computer();

    bool DoAction()override;

private:

    CharacterComputerReaction*    m_pReaction;
    CharacterComputerDoAction*    m_pDoAction;

};
