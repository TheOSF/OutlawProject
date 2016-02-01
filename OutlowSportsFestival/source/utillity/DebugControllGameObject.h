#pragma once

#include "../GameSystem/GameObject.h"
#include "../GameSystem/GameController.h"
#include "IEX_Input.h"

//-------------------------------------------------------------
//  デバッグ用　値を変更するクラス
//-------------------------------------------------------------


class DebugControllGameObject :public GameObjectBase
{
private:
    LPVECTOR3 pV;
    float*    pF;
    float     Speed;
    char      Flg;
    char      name[256];
public:
    DebugControllGameObject(LPVECTOR3 pV, float* pF, float  Speed, const char* name = nullptr, char Flg = 0) :
        pV(pV),
        pF(pF),
        Speed(Speed),
        Flg(Flg)
    {
        if (name)
        {
            strcpy_s<256>(this->name, name);
        }
        else
        {
            strcpy_s<256>(this->name, "DeContGameObj");
        }
    }

    bool Update()
    {
        
        float Key = controller::GetStickValue(controller::stick::right,0).x*Speed;

        if (Flg != 0 && !GetKeyState(Flg))return true;

        if (pV)
        {
            if (GetKeyState('X') & 0x80)pV->x += Key;
            if (GetKeyState('Y') & 0x80)pV->y += Key;
            if (GetKeyState('Z') & 0x80)pV->z += Key;
        }
        else
        {
            *pF += Key;
        }

        if (controller::GetTRG(controller::button::_L2, 0))
        {
            if (pV)
            {
                MyDebugString("%s x=%f y=%f z=%f \n",
                    name,
                    pV->x,
                    pV->y,
                    pV->z
                    );
            }
            else
            {
                MyDebugString("%s val=%f \n",
                    name,
                    *pF
                    );
            }

            
        }

        return true;
    }

    bool Msg(MsgType mt)
    {
        return false;
    }
};