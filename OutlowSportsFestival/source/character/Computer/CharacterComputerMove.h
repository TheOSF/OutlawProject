#pragma once
#include "../CharacterBase.h"
#include "../CharacterRenderer.h"
#include "../CharacterStateTemplate.h"	

//------------------------------------------------------------//
//            �R���s���[�^�p�̈ړ��N���X
//------------------------------------------------------------//

class CharacterComputerMove
{
public:

	struct InParam
	{
        struct BestLenInfo
        {
            float Len;   //�ڕW�Ƃ̃x�X�g�ȋ���
            RATIO Ratio; //�d�݂���
        };

        std::list<BestLenInfo>  m_BestLenList;

        InParam(){}
        InParam(const InParam& p);
	};


    CharacterComputerMove(CharacterBase* p, const InParam& param);
	~CharacterComputerMove();

    void    Update();
    Vector2 GetMoveVec(); //�ړ��x�N�g���𓾂�

    //���̃N���X�����݃^�[�Q�b�g�ɂ��Ă���L�����N�^�𓾂�
    bool GetTargetCharacter(CharacterBase** ppOutTargetChr)const;

private:

    struct StrongParam
    {
        RATIO UpdateTargetChrRatio;
        RATIO UpdateMovePosRatio;
        RATIO StopRatio;
        float RandBestLen;
    };

    typedef void(CharacterComputerMove::*StateFunc)();

    CharacterBase* const    m_pChr;
    const InParam           m_InParam;
    StateFunc               m_pStateFunc;
    Vector2                 m_MoveVec;     //�ړ��x�N�g��(XZ)
    int                     m_StateTimer;  //���݂̃X�e�[�g���n�܂��Ă��牽�t���[����������

    float                   m_SelectBestLen;
    RADIAN                  m_MovePosRotate;
    CharacterBase*          m_pTargetChr;
    Vector3                 m_MoveTargetPos;
    int                     m_StopFrame;
    bool                    m_ViewTargetChr;
    

    void SetNewState(StateFunc state);

    StrongParam GetStrongParam()const;

    bool isUpdateTargetFrame()const;
    bool isUpdatePositionFrame()const;
    bool isStopStartFrame(int MoveingFrame)const;
    bool isNearTargetPos(float OkLen)const;
    
    bool    GetAttackTargetChr(CharacterBase** ppOut);
    bool    GetMostHighHpChr(CharacterBase** ppOut);
    bool    GetTargetChr(CharacterBase** ppOut);
    float   GetBestLen();

    bool isViewTargetPos()const;


    Vector3 GetMovePosition();
    int     GetStopFrame()const;

    Vector2 GetMove_NearPos()const;
    Vector2 GetMove_ToTargetMove()const;


    void State_TargetSelect();  //�^�[�Q�b�g�I�l
    void State_Move();          //�ڕW�Ɍ������Ĉړ���
    void State_NearTargetPos(); //�����ȍX�V�͂��邪������ێ�����

    void State_Stop();          //��莞�ԗ����~�܂�
};