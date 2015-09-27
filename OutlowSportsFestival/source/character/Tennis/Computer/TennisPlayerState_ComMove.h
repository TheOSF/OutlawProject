#pragma once

#include "../TennisPlayer.h"
#include "../../../GameSystem/ForwardDecl.h"

//****************************************************
//	�e�j�X�v���C���[�̑���N���X�w�b�_�[
//****************************************************


//�v���C���[����̈ړ��N���X
class TennisState_ComputerControll_Move :public TennisState
{
public:

    class MoveControllClass
    {
    public:
        virtual ~MoveControllClass(){}
        virtual Vector2 Move(TennisPlayer* t) = 0;
    };

    void Enter(TennisPlayer* t)override;
    void Execute(TennisPlayer* t)override;
    void Exit(TennisPlayer* t)override;

private:
    CharacterUsualMove*		m_pMoveClass;
    MoveControllClass*      m_pMoveControllClass;
};


//�T���h�o�b�O�R���g���[���N���X
class SandBagMoveControllClass :public TennisState_ComputerControll_Move::MoveControllClass
{
public:
    Vector2 Move(TennisPlayer* t)override;
};


class ComMoveControllClass :public TennisState_ComputerControll_Move::MoveControllClass
{
private:

    struct Param
    {
        RATIO   BallCounter;     //�{�[���Ώ���
        RATIO   BallCounterSpeed;//�{�[���������x
        RATIO   BallCounterTec;  //�Ώ��̓I�m��
        RATIO   RunStop;         //�����~�܂闦
        RATIO   RunPlaceTec;     //�ړ��ڕW�̐��m��
        RATIO   DangerEscape;    //�댯�@�m�̐��m��
        RATIO   ActionFrequence; //�s���̕p�x
        RATIO   NamePlay;        //�Ȃ߃v���C�x
    };

    Param m_Param;
    void(ComMoveControllClass::*m_pStateFunc)(Vector2&);

    int   m_Count;
    TennisPlayer* const m_pTennis;
public:

    ComMoveControllClass(TennisPlayer* t);
    ~ComMoveControllClass();

    Vector2 Move(TennisPlayer* t)override;

    void GetParams(Param& out, StrongType::Value st);

    void StateMove(Vector2& out);
    void StateStop(Vector2& out);

    void SwitchAction();
};
