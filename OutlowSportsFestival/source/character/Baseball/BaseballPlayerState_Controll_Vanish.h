#pragma once

#include "BaseBallPlayer.h"
#include "../CharacterDamageControll.h"

//***************************************************
//		�R���g���[��������уN���X
//***************************************************
class BaseballState_ControllVanish : public BaseballState
{
public:
    BaseballState_ControllVanish(BaseballPlayer*  pBaseBall);
    ~BaseballState_ControllVanish();

    //�Q�b�^
    inline CharacterDamageControll*    GetControllClass()
    {
        return m_pMoveClass;
    }

    // �X�e�[�g�J�n
    void Enter(BaseballPlayer* p)override;

    // �X�e�[�g���s
    void Execute(BaseballPlayer* p)override;

    // �X�e�[�g�I��
    void Exit(BaseballPlayer* p)override;

private:
    BaseballPlayer*  const        m_pBaseBall;
    CharacterDamageControll*      m_pMoveClass;
};
