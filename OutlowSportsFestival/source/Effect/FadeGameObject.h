#ifndef __FADE_GAME_OBJECT_H__
#define __FADE_GAME_OBJECT_H__

#include "../GameSystem/GameObject.h"
#include "../utillity/ColorUtility.h"
#include "../Render/Renderer.h"

//-----------------------------------------------------------
// �@�t�F�[�h�A�E�g���ăt�F�[�h�C������Q�[���I�u�W�F�N�g
//-----------------------------------------------------------

class FadeGameObject :public GameObjectBase
{
public:

    FadeGameObject(
        COLORf Color,        //�F
        UINT   ChangeFrame,  //�������̐F�ɕς��܂ł̃t���[��
        UINT   FreezeFrame,  //�F���ς���Ă���Ƃ܂�t���[��
        UINT   BackFrame     //���ɖ߂�t���[��
        );

    ~FadeGameObject();

    bool Update();
    bool Msg(MsgType mt);

private:

    class Renderer :public ForwardRenderer
    {
    public:
        COLORf m_Color;

        Renderer();
        ~Renderer();

    private:
        void CalcZ();
        void Render();
    };

    const COLORf m_Color;        //�F
    const UINT   m_ChangeFrame;  //�ς��܂ł̃t���[��
    const UINT   m_FreezeFrame;    //�F���ς���Ă���Ƃ܂�t���[��
    const UINT   m_BackFrame;    //���ɖ߂�t���[��
    UINT         m_Count;        //�o�ߎ��ԃJ�E���g
    Renderer     m_Renderer;     //��`�`��N���X
};

#endif