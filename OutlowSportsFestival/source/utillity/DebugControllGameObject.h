#pragma once

#include "../GameSystem/GameObject.h"

//-------------------------------------------------------------
//  �f�o�b�O�p�@�l��ύX����N���X
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
        float Key = KEY(KEY_AXISX2, 0)*0.001f*Speed;

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

        if (KEY(KEY_ENTER, 0) == 3)
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