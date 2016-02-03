#include "BaseballState_SPAttack_P.h"
#include "BaseballPlayerState.h"
#include "Baseball_HitEvent.h"
#include "../../Ball/Baseball_SpAtk_Ball.h"
#include "Computer/BaseballPlayerState_ComMove.h"
#include "../../Camera/Camera.h"
#include "../../Collision/Collision.h"
#include "../../GameSystem/GameController.h"
#include "../CharacterFunction.h"
#include "../CharacterManager.h"
#include "../CharacterEvasionClass.h"
#include "../../Sound/Sound.h"
#include "../../Effect/SpecialAttackEffect.h"
//�@�R���X�g���N�^
BaseballState_SPAttack_P::BaseballState_SPAttack_P() :
m_Timer(0)
{

}


//�@�X�e�[�g�J�n
void BaseballState_SPAttack_P::Enter(BaseballPlayer* b)
{
    b->SetMotion(baseball_player::_mb_SpAtk_P);

    {
        std::list<LpGameObjectBase> UpdateObjList;

        UpdateObjList.push_back(b);

        DefGameObjMgr.FreezeOtherObjectUpdate(UpdateObjList, 45, true);
    }

    //�G�t�F�N�g
    new SpecialAttackEffect(b, 50);

    //SE
    Sound::Play(Sound::Skill);

}


//�@�X�e�[�g���s
void BaseballState_SPAttack_P::Execute(BaseballPlayer* b)
{
    const int ShotFrame = 48;
    const int EndFrame = 65;
    const int MoveFrame = 33;

    ++m_Timer;

    //�����␳
    CharacterBase* pTarget = nullptr;

    if (chr_func::CalcAtkTarget(b, PI*0.5f, 150.0f, &pTarget))
    {
        chr_func::AngleControll(b, pTarget);
    }

    chr_func::XZMoveDown(b, 0.08f);

    if (m_Timer == MoveFrame)
    {
        chr_func::AddMoveFront(b, 0.8f, 1.0f);
    }

    if (m_Timer == ShotFrame)
    {
        Vector3 pos, move;

        chr_func::GetFront(b, &move);
        move *= 3.8f;
        pos = b->m_Params.pos;
        pos.y = BallBase::UsualBallShotY;

        //�@�З͂Ƃ�
        new Baseball_SpAtk_Ball(b, pos, move, 25);
    }

	//�@���[�V�����X�V�Ƃ�
    b->ModelUpdate(1);
    chr_func::CreateTransMatrix(b, &b->getNowModeModel()->m_TransMatrix);


    if (m_Timer == EndFrame)
    {
        b->SetState(BaseballState_PlayerControll_Move::GetPlayerControllMove(b));
    }

}

//�@�X�e�[�g�I��
void BaseballState_SPAttack_P::Exit(BaseballPlayer* b)
{
    //�@�X�L���Q�[�W���Z�b�g
    chr_func::ResetSkillGauge(b);
}

