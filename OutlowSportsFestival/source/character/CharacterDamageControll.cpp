#include "CharacterDamageControll.h"
#include "CharacterFunction.h"

#include "../Sound/Sound.h"
#include "../Effect/EffectFactory.h"

#include "CharacterHitEventFunc.h"

CharacterDamageControll::EventClass::EventClass(CharacterBase*  pOwner, int VanishMotion, CharacterRenderer* pRenderer) :
pOwner(pOwner),
VanishMotion(VanishMotion),
pRenderer(pRenderer),
PosOffset(0, 0, 0)
{

}

CharacterDamageControll::EventClass::~EventClass()
{

}

//---------------------------------------------------------------//

CharacterDamageControll::CharacterDamageControll(EventClass* pEventClass) :
m_pEventClass(pEventClass),
m_Move(Vector3Zero),
m_Init(false),
m_NotControllCount(0),
m_DamageShockCount(0)
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

    Matrix M;
    D3DXMatrixTranslation(&M, m_pEventClass->PosOffset.x, m_pEventClass->PosOffset.y, m_pEventClass->PosOffset.z);

    m_Transform = M*m;
    m_NotControllCount = 0;
}

//操作解除(通常の吹き飛びステートに移行する)
void CharacterDamageControll::ToFree()
{
    const float MaxMove = 0.6f;

    if (m_Move.Length() > MaxMove)
    {
        m_Move.Normalize();
        m_Move *= MaxMove;
    }

    ToFree(m_Move);
}

void CharacterDamageControll::ToFree(CrVector3 Vec)
{
    m_pEventClass->SetVanishState(Vec);
}

//ダメージを与える(引数：１　ダメージ量　：２　体力を１残すかどうか)
void CharacterDamageControll::AddDamage(float value, bool dontDie)
{
    //ダメージ計算
    chr_func::CalcDamage(m_pEventClass->pOwner, value, dontDie);

    //受けたダメージ値によってゲージ増加
    chr_func::AddSkillGauge(m_pEventClass->pOwner, value * CharacterHitEventFunc::m_DamageAddSkillValue);


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
        m_pEventClass->pRenderer->SetMotion(m_pEventClass->VanishMotion);
    }
    
    const float ShockValue = 0.5f;  //ダメージのゆれ具合 

    CharacterBase* const pChr = m_pEventClass->pOwner;
    Vector3 PrePos = pChr->m_Params.pos;
    Vector3 DamageShockDiffPos = Vector3Zero;

    //アニメーション更新
    m_pEventClass->pRenderer->Update(1);


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

        m_pEventClass->pRenderer->m_TransMatrix = ScaleMat;
        m_pEventClass->pRenderer->m_TransMatrix *= m_Transform;

        {
            Matrix* p = &m_pEventClass->pRenderer->m_TransMatrix;

            pChr->m_Params.pos = Vector3(p->_41, p->_42, p->_43);
        }

        {
            Matrix* p = &m_pEventClass->pRenderer->m_TransMatrix;

            p->_41 += DamageShockDiffPos.x;
            p->_42 += DamageShockDiffPos.y;
            p->_43 += DamageShockDiffPos.z;
        }
    }

    //前回フレームの位置から移動値を算出
    {
        m_Move = m_pEventClass->pOwner->m_Params.move = (pChr->m_Params.pos - PrePos);
    }

    if (m_NotControllCount++ > 5)
    {
        ToFree();
    }
}

