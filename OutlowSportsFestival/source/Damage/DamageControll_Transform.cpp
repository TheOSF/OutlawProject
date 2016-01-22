#include "DamageControll_Transform.h"
#include "../character/CharacterFunction.h"

DamageControll_Transform::DamageControll_Transform() :
m_Destroy(false),
m_AddSkillGaugeValue(2),
m_pParent(nullptr)
{
    m_ChrControllArray.fill(nullptr);
    m_ChrArray.fill(nullptr);
    D3DXMatrixIdentity(&m_Transform);
}

DamageControll_Transform::~DamageControll_Transform()
{
    
}

//付いているキャラクタをすべて放す(デストラクタで自動的に呼ばれる
void DamageControll_Transform::AllFree()
{
    for (auto& it : m_ChrControllArray)
    {
        if (it != nullptr)
        {
            it->ToFree();
        }
    }

    m_ChrControllArray.fill(nullptr);
}

void DamageControll_Transform::AllFree(CrVector3 Vec)
{
    for (auto& it : m_ChrControllArray)
    {
        if (it != nullptr)
        {
            it->ToFree(Vec);
        }
    }
    m_ChrControllArray.fill(nullptr);
}

//操作するキャラクタを追加する
void DamageControll_Transform::AddControllClass(CharacterDamageControll* pControllClass)
{
    MyAssert(pControllClass != nullptr, "nullptrが送られました！");

    //空きを探す
    for (int i = 0; i < (int)m_ChrControllArray.size(); ++i)
    {
        if (m_ChrControllArray.at(i) == nullptr)
        {
            m_ChrControllArray.at(i) = pControllClass;
            m_ChrArray.at(i) = pControllClass->m_pEventClass->pOwner;
            return;
        }
    }

    MyAssert(false, "空き検索失敗!");
    pControllClass->ToFree();

    
}

//付いているすべてのキャラクタにダメージを与える(引数：１　ダメージ量　：２　体力を１残すかどうか)
void DamageControll_Transform::AddDamage(float value, bool dontDie)
{
    for (auto& it : m_ChrControllArray)
    {
        if (it != nullptr)
        {
            it->AddDamage(value, dontDie);

            if (m_pParent != nullptr)
            {
                chr_func::AddSkillGauge(m_pParent, value*0.01f);
            }
        }
    }

    
}

bool DamageControll_Transform::Update()
{
    //姿勢行列をセット
    for (auto& it : m_ChrControllArray)
    {
        if (it != nullptr)
        {
            it->SetTransform(m_Transform);
        }
    }

    //消去フレームなら
    if (m_Destroy)
    {
        //付いているキャラクタをすべて放す
        AllFree();
    }

    return !m_Destroy;
}

bool DamageControll_Transform::Msg(MsgType mt)
{

    return false;
}

