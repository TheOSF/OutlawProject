#pragma once

#include "../GameSystem/GameObject.h"
#include "../Render/Renderer.h"
#include "../Library/Bullet/BulletSystem.h"

//--------------------------------------------------
//  �t�F�[�h�A�E�g�ŏ�����{�[���N���X
//--------------------------------------------------

class BallFadeOutRenderer :
    public GameObjectBase,
    public ForwardRenderer
{
public:

    BallFadeOutRenderer(
        LPIEXMESH     pBallMesh,       //�{�[���̃��b�V���ւ̃|�C���^(delete���Ȃ�)
        const Matrix& BaseTransMatrix, //�X�P�[���A��]�s��
        RigidBody*    pRigidBody,      //���̃N���X�ւ̃|�C���^
        UINT          delete_frame     //������܂ł̃t���[��
        );

    ~BallFadeOutRenderer();


private:
    LPIEXMESH        m_pBallMesh;       //�{�[���̃��b�V���ւ̃|�C���^(delete���Ȃ�)
    const Matrix     m_BaseTransMatrix; //�X�P�[���A��]�s��
    RigidBody* const m_pRigidBody;      //���̃N���X�ւ̃|�C���^
    const UINT       m_Delete_frame;    //������܂ł̃t���[��
    UINT             m_Counter;         
    Vector3          m_Pos;

    bool Update();
    bool Msg(MsgType mt);

    void Render();
    void CalcZ();

};