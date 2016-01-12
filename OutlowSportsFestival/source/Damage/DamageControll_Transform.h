#pragma once

//-----------------------------------------------------------------------//
// 　キャラクタをくっつけるオブジェクト
//-----------------------------------------------------------------------//

#include "../GameSystem/GameObject.h"
#include "../character/CharacterDamageControll.h"
#include <array>

class DamageControll_Transform :public GameObjectBase
{
public:

    bool    m_Destroy;       //削除するかどうか(このクラスを)(このフラグをtrueにする以外でこのクラスは削除されない)
    Matrix  m_Transform;     //姿勢行列(キャラクタ操作)

    DamageControll_Transform();
    ~DamageControll_Transform();

    //付いているキャラクタをすべて放す(デストラクタで自動的に呼ばれる
    void AllFree();

    //操作するキャラクタを追加する
    void AddControllClass(CharacterDamageControll* pControllClass);

    //付いているすべてのキャラクタにダメージを与える(引数：１　ダメージ量　：２　体力を１残すかどうか)
    void AddDamage(float value, bool dontDie = false);

private:
    std::array<CharacterDamageControll*, 4> m_ChrControllArray;

    std::array<CharacterBase*, 4> m_ChrArray;  //

    bool Update();
    bool Msg(MsgType mt);
};

