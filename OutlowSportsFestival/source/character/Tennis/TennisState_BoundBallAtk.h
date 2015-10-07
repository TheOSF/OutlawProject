#pragma once

#include "TennisPlayer.h"
#include "../../Library/Bullet/BulletSystem.h"
#include "../../Damage/Damage.h"

//****************************************************
//	�e�j�X_�{�[���o�E���h�U���N���X
//****************************************************

class TennisState_BoundBallAtk :public TennisState
{
public:
    class ControllClass
    {
    public:
        virtual ~ControllClass(){}
        virtual Vector3 GetBoundVec() = 0;
    };

    TennisState_BoundBallAtk(
        ControllClass* pControllClass   //�o�E���h�������R���g���[������N���X(�I������delete����)
        );

    ~TennisState_BoundBallAtk();

    void Enter(TennisPlayer* t)override;
    void Execute(TennisPlayer* t)override;
    void Exit(TennisPlayer* t)override;

private:
    ControllClass* const m_pControllClass;
    int                  m_Timer;
};


//****************************************************
//	�e�j�X_�o�E���h����{�[���N���X
//****************************************************
#include "../../Ball/Ball.h"
#include "../../GameSystem/GameObject.h"
#include "../../GameSystem/ForwardDecl.h"
#include "../../utillity/Locus.h"
#include "../../Render/LightObject.h"

class TennisBoundBall :public GameObjectBase, public BallBase
{
public:

    //�R���X�g���N�^
    TennisBoundBall(
        CrVector3  pos,         //�������W
        CrVector3  first_move,  //�P��ڂ̈ړ��̈ړ���
        TennisPlayer* const pOwner   //���ˎ�ւ̃|�C���^
        );

    ~TennisBoundBall();

    bool Update();
    bool Msg(MsgType mt);

private:
    typedef void(TennisBoundBall::*StateFunc)();
    
    StateFunc      m_pStateFunc;    //���݂̍X�V���s���֐�
    int            m_Timer;         //��Ɨp�^�C�}�[

    TennisPlayer*const  m_pOwnerTennis;  //�e�e�j�X
    MeshRenderer*       m_pBallRenderer; //�{�[���`��N���X
    Locus               m_Locus;         //�O�ՃN���X
    RigidBody*          m_pRigidBody;    //���̃{�[���𓮂������̃N���X
    Matrix              m_BaseMatrix;    //�X�P�[�����������ϊ��s��
    DamageShpere        m_Damage;        //�_���[�W����N���X

    void StateFly();         //�������^�x���̃X�e�[�g
    void StateGroundTouch(); //�n�ʂɒ��e�����u�Ԃ̃X�e�[�g
    void StateMove();        //���������^�����̃X�e�[�g
    void StateNoDamage();    //�U���I����̌�͏����邾���X�e�[�g
    void StateFinish();      //�t�B�j�b�V��(���ݎ��̂�������)�X�e�[�g

    void StateCreateFadeOutBall(); //�t�F�[�h�A�E�g���ď�����{�[�������

    void CreateRigidBody();  //���̐���
    void AddLocusPoint();    //�O�Ղ̃|�C���g�����݂̃p�����[�^�ň�_�ǉ�����
    void UpdateMesh();       //���b�V���X�V
    void UpdateLocusColor(); //�O�ՐF�X�V
    void UpdateDamage();     //�����蔻��̍X�V

    //�X�e�[�g�֐��̐؂�ւ�
    void SetState(StateFunc pNextState);

    //�J�E���^�[�������ۂ̏���
    void Counter(CharacterBase* pCounterCharacter)override;
};
