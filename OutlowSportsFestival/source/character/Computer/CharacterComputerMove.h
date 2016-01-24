#pragma once
#include "../CharacterBase.h"
#include "../CharacterRenderer.h"
#include "../CharacterStateTemplate.h"	

//------------------------------------------------------------//
//            コンピュータ用の移動クラス
//------------------------------------------------------------//

class CharacterComputerMove
{
public:

	struct InParam
	{
        struct BestLenInfo
        {
            float Len;   //目標とのベストな距離
            RATIO Ratio; //重みさん
        };

        std::list<BestLenInfo>  m_BestLenList;

        InParam(){}
        InParam(const InParam& p);
	};


    CharacterComputerMove(CharacterBase* p, const InParam& param);
	~CharacterComputerMove();

    void    Update();
    Vector2 GetMoveVec(); //移動ベクトルを得る

    //このクラスが現在ターゲットにしているキャラクタを得る
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
    Vector2                 m_MoveVec;     //移動ベクトル(XZ)
    int                     m_StateTimer;  //現在のステートが始まってから何フレームたったか

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


    void State_TargetSelect();  //ターゲット選考
    void State_Move();          //目標に向かって移動中
    void State_NearTargetPos(); //微妙な更新はするが現状を維持する

    void State_Stop();          //一定時間立ち止まる
};