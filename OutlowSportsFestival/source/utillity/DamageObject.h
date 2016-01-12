#pragma once

#include "../GameSystem/GameObject.h"
#include "../Damage/Damage.h"

//-------------------------------------------------//
//  �����I�ɏ�����_���[�W�I�u�W�F�N�g
//-------------------------------------------------//

class DamageObject :public GameObjectBase
{
public:

    DamageObject(
        DamageBase*     pDamage,   //new �Ő������ꂽDamage�ւ̃|�C���^
        UINT            Frame      //��������
        );
    ~DamageObject();

private:
    DamageBase*     m_pDamage;
    int             m_Frame;

    bool Update();
    bool Msg(MsgType mt);

};