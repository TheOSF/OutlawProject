#pragma once

#include "TennisPlayer.h"
#include "../CharacterDamageControll.h"

//***************************************************
//		�R���g���[��������уN���X
//***************************************************
class TennisState_ControllVanish : public TennisState
{
public:
    TennisState_ControllVanish(TennisPlayer*  pTennis);
    ~TennisState_ControllVanish();

    //�Q�b�^
    inline CharacterDamageControll*    GetControllClass()
    {
        return m_pMoveClass;
    }

    // �X�e�[�g�J�n
    void Enter(TennisPlayer* t)override;

    // �X�e�[�g���s
    void Execute(TennisPlayer* t)override;

    // �X�e�[�g�I��
    void Exit(TennisPlayer* t)override;

private:
    TennisPlayer*  const        m_pTennis;
    CharacterDamageControll*    m_pMoveClass;
};
