#include "CharacterDamageControll.h"
#include "CharacterFunction.h"

#include "../Sound/Sound.h"
#include "../Effect/EffectFactory.h"


CharacterDamageControll::EventClass::EventClass(CharacterBase*  pOwner, int VanishMotion):
pOwner(pOwner),
VanishMotion(VanishMotion)
{

}

CharacterDamageControll::EventClass:: ~EventClass()
{

}

//---------------------------------------------------------------//

CharacterDamageControll::CharacterDamageControll(EventClass* pEventClass) :
m_pEventClass(pEventClass),
m_Move(Vector3Zero),
m_Init(false),
m_NotControllCount(0)
{
    D3DXMatrixIdentity(&m_Transform);

    MyAssert(m_pEventClass != nullptr, "イベントクラスがnullptrになっています");

    m_pEventClass->pOwner->m_Params.DoCheckOtherChrSpace = false;
}

CharacterDamageControll::~CharacterDamageControll()
{
    m_pEventClass->pOwner->m_Params.DoCheckOtherChrSpace = true;
    delete m_pEventClass;
}

//姿勢行列をセット
void CharacterDamageControll::SetTransform(const Matrix& m)
{
    m_Transform = m;
    m_NotControllCount = 0;
}

//操作解除(通常の吹き飛びステートに移行する)
void CharacterDamageControll::ToFree()
{
    const float MaxMove = 2.0f;

    if (m_Move.Length() > MaxMove)
    {
        m_Move.Normalize();
        m_Move *= MaxMove;
    }

    m_pEventClass->SetVanishState(m_Move);
}

//ダメージを与える(引数：１　ダメージ量　：２　体力を１残すかどうか)
void CharacterDamageControll::AddDamage(float value, bool dontDie)
{
    //ダメージ計算
    chr_func::CalcDamage(m_pEventClass->pOwner, value, dontDie);


    EffectFactory::HitEffect(
        m_pEventClass->pOwner,
        Vector3Normalize(m_Move)
        );

    //ＳＥ
    Sound::Play(Sound::Damage3);

    //位置のズレカウントを　セット
    m_DamageShockCount = 1.0f;
}



//毎フレームの更新
void CharacterDamageControll::Update()
{
    //初期フレームでモーションセット
    if (m_Init == false)
    {
        m_Init = true;
        m_pEventClass->pOwner->m_Renderer.SetMotion(m_pEventClass->VanishMotion);
    }
    
    const float ShockValue = 0.5f;  //ダメージのゆれ具合 

    CharacterBase* const pChr = m_pEventClass->pOwner;
    Vector3 PrePos = pChr->m_Params.pos;
    Vector3 DamageShockDiffPos = Vector3Zero;

    //アニメーション更新
    pChr->m_Renderer.Update(1);


    //ダメージによる振動の位置を更新
    if (m_DamageShockCount > 0)
    {
        DamageShockDiffPos = Vector3Rand()*m_DamageShockCount*ShockValue;
        m_DamageShockCount -= 0.08f;
    }

    //ワールド変換行列を作成し代入
    {
        Matrix ScaleMat;

        D3DXMatrixScaling(&ScaleMat, pChr->m_ModelSize, pChr->m_ModelSize, pChr->m_ModelSize);

        pChr->m_Renderer.m_TransMatrix = ScaleMat;
        pChr->m_Renderer.m_TransMatrix *= m_Transform;

        {
            Matrix* p = &pChr->m_Renderer.m_TransMatrix;

            pChr->m_Params.pos = Vector3(p->_41, p->_42, p->_43);
        }

        {
            Matrix* p = &pChr->m_Renderer.m_TransMatrix;

            p->_41 += DamageShockDiffPos.x;
            p->_42 += DamageShockDiffPos.y;
            p->_43 += DamageShockDiffPos.z;
        }
    }

    //前回フレームの位置から移動値を算出
    {
        m_Move = m_pEventClass->pOwner->m_Params.move = (pChr->m_Params.pos - PrePos);
    }

    if (m_NotControllCount++ > 60)
    {
        ToFree();
    }
}

