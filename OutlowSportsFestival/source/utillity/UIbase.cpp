#include "UIbase.h"


//------------------------------------------------------------//
// �ėp�t�h�`��N���X
//------------------------------------------------------------//


UIbase::UIbase()
{
    m_pStateMachine = new MyStateMacihne(this);

    param.Delete = false;
    param.Texture = nullptr;
    param.UV = RectI(0, 0, 100, 100);
    param.pos = Vector3(0, 0, 0);
    param.scale = Vector2(100, 100);
    param.angle = 0;
    param.color = 0xFFFFFFFF;
    param.dw_type = DrawType::Usual;
}

UIbase::~UIbase()
{
    delete m_pStateMachine;
    m_pStateMachine = nullptr;
}


void UIbase::SetState(State* NewState)  //�V�����X�e�[�g���Z�b�g
{
    m_pStateMachine->set_state(NewState);
}

void UIbase::CalcZ()
{
    m_SortZ = param.pos.z;
}

void UIbase::Render()
{
    MyAssert(param.Texture != nullptr, "�e�N�X�`�����ݒ肳��Ă��Ȃ���ԂŌĂ΂�܂���");

    DWORD dw_flag = RS_COPY;

    switch (param.dw_type)
    {
    case DrawType::Usual:
        dw_flag = RS_COPY;
        break;
    case DrawType::Add:
        dw_flag = RS_ADD;
        break;
    case DrawType::Sub:
        dw_flag = RS_SUB;
        break;
    default:
        MyAssert(false, "�F���ł��Ȃ��`��^�C�v");
        break;
    }

    param.Texture->RenderPlus(
        Vector2(param.pos.x, param.pos.y),
        param.scale,
        param.angle,
        Vector2((float)param.UV.x, (float)param.UV.y),
        Vector2((float)param.UV.w, (float)param.UV.h),
        dw_flag,
        param.color,
        0 //����Ȃ��H
        );

}


bool UIbase::Update()
{
    //�X�e�[�g���s
    m_pStateMachine->state_execute();

    return !param.Delete;
}

bool UIbase::Msg(MsgType mt)
{
    return false;
}