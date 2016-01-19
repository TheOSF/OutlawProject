#pragma once

#include "../GameSystem/GameObject.h"
#include "../Damage/Damage.h"
#include "../Ball/BallEffect.h"
#include "../Render/LightObject.h"
#include "../Library/Bullet/BulletSystem.h"
#include "../utillity/LocusHDR.h"
#include "Ball.h"


class AmefootPlayer;

// �A���t�g�̃o�E���h����{�[���N���X
class AmefootBoundBall :public GameObjectBase
{
public:
    //�R���X�g���N�^
    AmefootBoundBall(
        CrVector3  pos,         //�������W
        CrVector3  first_move,  //�P��ڂ̈ړ��̈ړ���
        AmefootPlayer* const pOwner   //���ˎ�ւ̃|�C���^
        );

    ~AmefootBoundBall();

    bool Update();
    bool Msg(MsgType mt);

private:
    typedef void(AmefootBoundBall::*StateFunc)();

    StateFunc           m_pStateFunc;    //���݂̍X�V���s���֐�
    int                 m_Timer;         //��Ɨp�^�C�}�[
    Vector3             m_Move;
    Vector3             m_Pos;           
    RADIAN              m_ZRotate;

    AmefootPlayer*const m_pOwner;  //�e�L�����N�^
    MeshRenderer*       m_pBallRenderer; //�{�[���`��N���X
    LocusHDR            m_Locus;         //�O�ՃN���X

    void StateFly();         //�������^�x���̃X�e�[�g
    void StateGroundTouch(); //�n�ʂɒ��e�����u�Ԃ̃X�e�[�g
    void StateFinish();      //�t�B�j�b�V��(���ݎ��̂�������)�X�e�[�g

    void AddLocusPoint();    //�O�Ղ̃|�C���g�����݂̃p�����[�^�ň�_�ǉ�����
    void UpdateMesh();       //���b�V���X�V
    void UpdateLocusColor(); //�O�ՐF�X�V

                             
    void SetState(StateFunc pNextState); //�X�e�[�g�֐��̐؂�ւ�
};


