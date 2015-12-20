#pragma once

#include "SelectMenuUI.h"
#include "../GameSystem/GameController.h"
#include <array>

//----------------------------------------------------------//
// �u�ȈՃ��j���[�N���X�v���v���C���[�����삷�邽�߂̃N���X
//----------------------------------------------------------//


class SelectMenuUI_ControllerPlayer :public SelectMenuUI::IController
{
public:
    SelectMenuUI_ControllerPlayer();
    ~SelectMenuUI_ControllerPlayer();

    //����\�ȃR���g���[���̔ԍ����Z�b�g����
    void SetCanControll_ControllerNum(controller::CONTROLLER_NUM Num);

    
    SelectMenuUI::MoveVec GetVec()override;
    bool isDecide()override;

private:
    typedef std::array<controller::CONTROLLER_NUM, 4> ContollerNumArray;

    ContollerNumArray m_ContollerNumArray;
};