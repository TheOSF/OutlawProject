#include "UIbase.h"


//------------------------------------------------------------//
// 汎用ＵＩ描画クラス
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


void UIbase::SetState(State* NewState)  //新しいステートをセット
{
    m_pStateMachine->set_state(NewState);
}

void UIbase::CalcZ()
{
    m_SortZ = param.pos.z;
}

void UIbase::Render()
{
    MyAssert(param.Texture != nullptr, "テクスチャが設定されていない状態で呼ばれました");

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
        MyAssert(false, "認識できない描画タイプ");
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
        0 //いらない？
        );

}


bool UIbase::Update()
{
    //ステート実行
    m_pStateMachine->state_execute();

    return !param.Delete;
}

bool UIbase::Msg(MsgType mt)
{
    return false;
}