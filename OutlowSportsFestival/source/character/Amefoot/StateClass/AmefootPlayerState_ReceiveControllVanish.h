#pragma once

#include "../AmefootPlayer.h"
#include "../../CharacterDamageControll.h"

//***************************************************
//		�R���g���[��������уN���X
//***************************************************
class AmefootState_ControllVanish : public AmefootState
{
public:
    AmefootState_ControllVanish(AmefootPlayer*  pAmefoot);
    ~AmefootState_ControllVanish();

    //�Q�b�^
    inline CharacterDamageControll*    GetControllClass()
    {
        return m_pMoveClass;
    }

    // �X�e�[�g�J�n
    void Enter(AmefootPlayer* t)override;

    // �X�e�[�g���s
    void Execute(AmefootPlayer* t)override;

    // �X�e�[�g�I��
    void Exit(AmefootPlayer* t)override;

private:
    AmefootPlayer*  const        m_pAmefoot;
    CharacterDamageControll*    m_pMoveClass;
};
