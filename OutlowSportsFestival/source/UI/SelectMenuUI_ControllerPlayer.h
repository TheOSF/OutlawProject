#pragma once

#include "SelectMenuUI.h"
#include "../GameSystem/GameController.h"
#include <array>

//----------------------------------------------------------//
// 「簡易メニュークラス」をプレイヤーが操作するためのクラス
//----------------------------------------------------------//


class SelectMenuUI_ControllerPlayer :public SelectMenuUI::IController
{
public:
    SelectMenuUI_ControllerPlayer();
    ~SelectMenuUI_ControllerPlayer();

    //操作可能なコントローラの番号をセットする
    void SetCanControll_ControllerNum(controller::CONTROLLER_NUM Num);

    
    SelectMenuUI::MoveVec GetVec()override;
    bool isDecide()override;

private:
    typedef std::array<controller::CONTROLLER_NUM, 4> ContollerNumArray;

    ContollerNumArray m_ContollerNumArray;
};