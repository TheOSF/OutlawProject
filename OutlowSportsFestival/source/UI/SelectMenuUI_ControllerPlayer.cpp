#include "SelectMenuUI_ControllerPlayer.h"

SelectMenuUI_ControllerPlayer::SelectMenuUI_ControllerPlayer()
{
    m_ContollerNumArray.fill(controller::__ERROR_CONTROLLER_NUM);
}


SelectMenuUI_ControllerPlayer::~SelectMenuUI_ControllerPlayer()
{

}

//����\�ȃR���g���[���̔ԍ����Z�b�g����
void SelectMenuUI_ControllerPlayer::SetCanControll_ControllerNum(controller::CONTROLLER_NUM Num)
{
    m_ContollerNumArray.at(Num) = Num;
}

SelectMenuUI::MoveVec SelectMenuUI_ControllerPlayer::GetVec()
{
    SelectMenuUI::MoveVec ret = SelectMenuUI::MoveVec::_NoMove;

    for (auto& it : m_ContollerNumArray)
    {
        //�ԍ����L�����A�|�����u�ԂȂ�
        if (it != controller::__ERROR_CONTROLLER_NUM &&
            controller::GetStickJustMove(controller::stick::left, it))
        {
            Vector2 Stick = controller::GetStickValue(controller::stick::left, it);

            //�덷���Ȃ珈�����Ȃ�
            if (controller::isStickAdjustValue(Stick.y))
            {
                continue;
            }

            //�㉺����
            if (Stick.y > 0)
            {
                ret = SelectMenuUI::MoveVec::Up;
            }
            else
            {
                ret = SelectMenuUI::MoveVec::Down;
            }

            break;
        }
    }

    return ret;
}

bool SelectMenuUI_ControllerPlayer::isDecide()
{
    for (auto& it : m_ContollerNumArray)
    {
        //�ԍ����L�����A�ۂ������Ă����Ȃ�
        if (it != controller::__ERROR_CONTROLLER_NUM &&
            controller::GetTRG(controller::button::maru, it))
        {
            return true;
        }
    }

    return false;
}