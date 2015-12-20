#include "SelectMenuUI_ControllerPlayer.h"

SelectMenuUI_ControllerPlayer::SelectMenuUI_ControllerPlayer()
{
    m_ContollerNumArray.fill(controller::__ERROR_CONTROLLER_NUM);
}


SelectMenuUI_ControllerPlayer::~SelectMenuUI_ControllerPlayer()
{

}

//操作可能なコントローラの番号をセットする
void SelectMenuUI_ControllerPlayer::SetCanControll_ControllerNum(controller::CONTROLLER_NUM Num)
{
    m_ContollerNumArray.at(Num) = Num;
}

SelectMenuUI::MoveVec SelectMenuUI_ControllerPlayer::GetVec()
{
    SelectMenuUI::MoveVec ret = SelectMenuUI::MoveVec::_NoMove;

    for (auto& it : m_ContollerNumArray)
    {
        //番号が有効かつ、倒した瞬間なら
        if (it != controller::__ERROR_CONTROLLER_NUM &&
            controller::GetStickJustMove(controller::stick::left, it))
        {
            Vector2 Stick = controller::GetStickValue(controller::stick::left, it);

            //誤差内なら処理しない
            if (controller::isStickAdjustValue(Stick.y))
            {
                continue;
            }

            //上下決定
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
        //番号が有効かつ、丸を押していたなら
        if (it != controller::__ERROR_CONTROLLER_NUM &&
            controller::GetTRG(controller::button::maru, it))
        {
            return true;
        }
    }

    return false;
}