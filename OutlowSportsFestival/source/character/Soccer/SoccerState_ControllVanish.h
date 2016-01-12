#pragma once

#include "SoccerPlayer.h"
#include "../CharacterDamageControll.h"

//***************************************************
//		�R���g���[��������уN���X
//***************************************************
class SoccerState_ControllVanish : public SoccerState
{
public:
    SoccerState_ControllVanish(SoccerPlayer*  pSoccer);
    ~SoccerState_ControllVanish();

    //�Q�b�^
    inline CharacterDamageControll*    GetControllClass()
    {
        return m_pMoveClass;
    }

    // �X�e�[�g�J�n
    void Enter(SoccerPlayer* p)override;

    // �X�e�[�g���s
    void Execute(SoccerPlayer* p)override;

    // �X�e�[�g�I��
    void Exit(SoccerPlayer* p)override;

private:
    SoccerPlayer*  const        m_pSoccer;
    CharacterDamageControll*    m_pMoveClass;
};
