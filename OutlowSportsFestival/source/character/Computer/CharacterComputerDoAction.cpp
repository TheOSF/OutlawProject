#include "CharacterComputerDoAction.h"
#include "../CharacterManager.h"
#include "../CharacterFunction.h"
#include"../../Damage/Damage.h"

CharacterComputerDoAction::CharacterComputerDoAction(
    CharacterBase*			pParent,	            //操るキャラクタのポインタ
    CharacterComputerMove*  pCharacterComputerMove, //AI移動クラスへのポインタ
    ActionEvent*			pActionEvent	            //移動イベントに反応するクラス
    ) :
    m_pCharacterComputerMove(pCharacterComputerMove),
    m_pChr(pParent),
    m_pActionEvent(pActionEvent),
    m_DoAttack(false)
{


}

CharacterComputerDoAction::~CharacterComputerDoAction()
{
    delete m_pActionEvent;
}

void CharacterComputerDoAction::Update()
{
    //攻撃フラグを更新
    if (m_DoAttack == false && isDoAttack())
    {
        m_DoAttack = true;
    }

    //攻撃を実行
    if (m_DoAttack)
    {
        CharacterBase* pTarget = nullptr;

        if (m_pCharacterComputerMove->GetTargetCharacter(&pTarget) && 
            m_pActionEvent->Action(pTarget, Vector3Distance(pTarget->m_Params.pos, m_pChr->m_Params.pos)))
        {
            m_DoAttack = false;
        }
    }
}



bool CharacterComputerDoAction::isDoAttack()
{
    RATIO val = 0.0f;

    switch (m_pChr->m_PlayerInfo.strong_type)
    {
    case StrongType::_Weak:
        val = 0.01f;
        break;

    case StrongType::_Usual:
        val = 0.03f;
        break;

    case StrongType::_Strong:
        val = 0.05f;
        break;

    default:
        MyAssert(false, "対応していない強さタイプ");
        break;
    }

    return frand() < val;
}
