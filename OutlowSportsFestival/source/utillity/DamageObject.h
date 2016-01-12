#pragma once

#include "../GameSystem/GameObject.h"
#include "../Damage/Damage.h"

//-------------------------------------------------//
//  自動的に消えるダメージオブジェクト
//-------------------------------------------------//

class DamageObject :public GameObjectBase
{
public:

    DamageObject(
        DamageBase*     pDamage,   //new で生成されたDamageへのポインタ
        UINT            Frame      //生存時間
        );
    ~DamageObject();

private:
    DamageBase*     m_pDamage;
    int             m_Frame;

    bool Update();
    bool Msg(MsgType mt);

};