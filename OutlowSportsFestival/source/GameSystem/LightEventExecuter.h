#pragma once

#include "GameObject.h"
#include "../character/CharacterManager.h"

//-------------------------------------------------------------------------------------//
//  �����@�Ɓ@���s���̃C�x���g���s������N���X(�L�����N�^���S���A��ɕς��@�Ȃ�)
//-------------------------------------------------------------------------------------//

class LightEventExecuter :public GameObjectBase
{
public:
    LightEventExecuter(
        LPVECTOR3 pDirLightColor,  //�V�[���I�����܂�delete���Ȃ��O��
        LPVECTOR3 pAmbLightColor   //�V�[���I�����܂�delete���Ȃ��O��
        );

    ~LightEventExecuter();

    bool Update();	//false��Ԃ��Ə�������
    bool Msg(MsgType mt);

private:

    LPVECTOR3 const m_pDirLightColor;
    LPVECTOR3 const m_pAmbLightColor;

    UINT      m_ChrLiveCount;

    Vector3   m_InitDir;
    Vector3   m_InitAmb;

    bool EventCheck();  //���C�g�I�t�̃C�x���g�����邩�ǂ���
    void LightOff();    //���C�g���Â�����
    void LightUpdate(); //���C�g�l���X�V
};