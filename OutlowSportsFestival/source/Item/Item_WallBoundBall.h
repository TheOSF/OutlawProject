#pragma once


#include "../Ball/Ball.h"
#include "../Damage/Damage.h"
#include "../utillity/LocusHDR.h"
#include "../Render/MeshRenderer.h"
#include "../GameSystem/GameObject.h"

//***************************************************
//		�x���炵�p�{�[���N���X
//***************************************************

class Item_WallBoundBall :public GameObjectBase, public BallBase
{
public:
    //�R���X�g���N
    Item_WallBoundBall(CrVector3 pos, CrVector3 move, CrVector3 target);
    ~Item_WallBoundBall();

    bool Update();
    bool Msg(MsgType mt);

private:
    MeshRenderer		m_MeshRenderer;
    DamageShpere		m_Damage;
    LocusHDR            m_Locus;
    bool                m_LiveFlg;
    const Vector3       m_Target;
    const float         m_Glavity;
    float               m_ColorTimer;
    int                 m_Timer;

    void(Item_WallBoundBall::*m_pStateFunc)();

    Vector3 GetToMoveValue()const;   //���ւ̈ړ��x�N�g�����Z�o����
    Vector4 GetColor(RATIO t)const;  //t�ɉ��������F�J���[���Z�o
    void UpdateLocusColor();         //�F�X�V

    void ApperEffect();              //�G�t�F�N�g
    void UpdateDamageClass();        //�����蔻��X�V
    void UpdateMesh();               //���b�V���X�V
    bool WallCheck(Vector3* pNormal);//�ǂƂ̔�������,true��Ԃ��ꍇ�ɂ�pNormal�ɕǖ@�������Ă���

    void State_ToFloorMove();
    void State_RefrectMove();
    void State_ToBellMove();
    void State_Finish();

    void Counter(CharacterBase* pCounterCharacter)override;
};