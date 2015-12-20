#include "SelectMenuUI.h"

//--------------------------------------------------------//
// �@�ȈՃ��j���[�쐬�t�h�N���X�̃��j���[�\����
//--------------------------------------------------------//

SelectMenuUI::MenuParam::MenuParam()
{
    pDecideEvent = nullptr;

    {
        TextureRect.x = 0;
        TextureRect.y = 0;
        TextureRect.w = 500;
        TextureRect.h = 100;
    }

    SizeX = 500;
    SizeY = 100;

    SelectRatio = 0;
}

SelectMenuUI::MenuParam::~MenuParam()
{

}

void SelectMenuUI::MenuParam::SetSize(UINT X, UINT Y)
{
    SizeX = X;
    SizeY = Y;
}

void SelectMenuUI::MenuParam::MulRect(RectI* pOut)const  //�g�又�����s�������̂�Ԃ�
{
    const int W = (int)((float)pOut->w * SelectRatio*0.2f);
    const int H = (int)((float)pOut->h * SelectRatio*0.2f);

    pOut->w += W;
    pOut->h += H;
    pOut->x -= W / 2;
    pOut->y -= H / 2;
}


//--------------------------------------------------------//
// �@�ȈՃ��j���[�쐬�t�h�N���X
//--------------------------------------------------------//

SelectMenuUI::SelectMenuUI(
    IController*    pController,     //�R���g���[��(���j���[����)�N���X�ւ̃|�C���^
    iex2DObj*       pBaseTexture,    //���j���[�\�����ڂ��Q�Ƃ���e�N�X�`��
    bool            isTextureDelete, //�e�N�X�`����Delete���邩�ǂ���
    float           Z                //���s��(�`�揇��
    ) :
    m_NowSelectNum(0),
    m_pController(pController),
    m_pBaseTexture(pBaseTexture),
    m_isTextureDelete(isTextureDelete),
    m_Z(Z),
    m_Alpha(0),
    m_BackBlack(0),
    m_pStateFunc(&SelectMenuUI::State_FadeIn)
{

}

SelectMenuUI::~SelectMenuUI()
{
    delete m_pController;

    if (m_isTextureDelete)
    {
        delete m_pBaseTexture;
    }

    for (auto& it : m_MenuParamList)
    {
        delete it.pDecideEvent;
    }
}

//���ڂ�ǉ�
void SelectMenuUI::AddMenu(
    IDecideEvent*   pDecideEvent,   //���肵���ۂɌĂяo�����
    RectI           TextureRect,    //�\������e�N�X�`���̋�`
    UINT            SizeX,          //�\������傫���w
    UINT            SizeY           //�\������傫���x
    )
{
    //�p�����[�^�ݒ�
    MenuParam Param;

    Param.pDecideEvent = pDecideEvent;
    Param.TextureRect = TextureRect;
    Param.SetSize(SizeX, SizeY);

    Param.SelectRatio = 0;

    //�ǉ�
    m_MenuParamList.push_back(Param);
}


//------------------------------�X�e�[�g�֐�------------------------------//

void SelectMenuUI::State_FadeIn()
{
    const float Speed = 0.05f;

    //�s�����x�����Z
    m_Alpha += Speed;
   
    //�P�ȏ�ŃX�e�[�g�ڍs
    if (m_Alpha > 1)
    {
        m_Alpha = 1.0f;
        m_pStateFunc = &SelectMenuUI::State_CanControll;
    }

    //�w�i�̍��͕s�����x�Ɠ��@
    m_BackBlack = m_Alpha * 0.75f;


}

void SelectMenuUI::State_FadeOut()
{
    const float Speed = 0.05f;

    //�s�����x�����Z
    m_Alpha -= Speed;

    //�O�ȉ��ŃX�e�[�g�ڍs
    if (m_Alpha < 0)
    {
        m_Alpha = 1.0f;
        m_pStateFunc = &SelectMenuUI::State_Delete;
    }

    //�w�i�̍��͕s�����x�Ɠ�������
    m_BackBlack = m_Alpha;

    //�I���X�V
    SelectUpdate(false);
}

void SelectMenuUI::State_CanControll()
{

    //���j���[���ڂ�����������I��
    if (m_MenuParamList.empty())
    {
        return;
    }

    //�ړ�����
    const MoveVec Vec = m_pController->GetVec();

    //�J�[�\���ʒu���X�V
    m_NowSelectNum = CalcCursorNum(Vec);

    //���肩�ǂ���
    if (m_pController->isDecide())
    {
        //���菈��
        m_MenuParamList.at(m_NowSelectNum).pDecideEvent->Decide();
        
        //�t�F�[�h�A�E�g�X�e�[�g��
        m_pStateFunc = &SelectMenuUI::State_FadeOut;
    }

    //�I���X�V
    SelectUpdate(false);
}

void SelectMenuUI::State_Delete()
{
    
}

//--------------------------------------------------------------------------//


void SelectMenuUI::CalcZ()
{
    m_SortZ = m_Z;
}

//�`��(�����I�ɌĂ΂��)
void SelectMenuUI::Render()
{
    //���ڂ�����������I��
    if (m_MenuParamList.empty())
    {
        return;
    }

    //UI�w�i�������`��
    iexPolygon::Rect(
        0,
        0,
        iexSystem::ScreenWidth,
        iexSystem::ScreenHeight,
        RS_COPY,
        D3DCOLOR_COLORVALUE(0, 0, 0, m_BackBlack)
        );

    //���ڂ��e�`��
    {
        RectI DrawRect, TempRect;
        float Color = 0;

        //�`��J�n�ʒu���Z�o
        CalcRenderStartPos(&DrawRect.x, &DrawRect.y);

        //�`��
        for (auto &it : m_MenuParamList)
        {

            TempRect = DrawRect;

            //�`��T�C�Y�̎擾
            {
                TempRect.w = (int)it.SizeX;
                TempRect.h = (int)it.SizeY;

                it.MulRect(&TempRect);
            }

            //�F�̓x�������v�Z
            Color = fClamp(0.5f + it.SelectRatio*0.5f, 1, 0);

            //�`��
            m_pBaseTexture->Render(
                TempRect,
                it.TextureRect,
                RS_COPY,
                D3DCOLOR_COLORVALUE(Color, Color, Color, m_Alpha)
                );

            //�`�悵���c�̑傫���̕��ړ�����
            DrawRect.y += it.SizeY;
        }
    }

}

//�`��J�n�ʒu���v�Z
void SelectMenuUI::CalcRenderStartPos(int* OutX, int* OutY)const
{
    //���ڂ�����������I��
    if (m_MenuParamList.empty())
    {
        *OutX = 0;
        *OutY = 0;
        return;
    }

    int W = 0, H = 0;


    {
        for (auto &it : m_MenuParamList)
        {
            W = max(W, (int)it.SizeX);
            H += it.SizeY;
        }
    }

    W /= 2;
    H /= 2;

    *OutX = (int)iexSystem::ScreenWidth / 2 - W;
    *OutY = (int)iexSystem::ScreenHeight / 2 - H;
}

int  SelectMenuUI::CalcCursorNum(MoveVec vec)const
{   
    int CalcCursorNum = m_NowSelectNum;

    switch (vec)
    {
    case MoveVec::_NoMove:
        //�ړ����Ă��Ȃ������ꍇ�͉������Ȃ�
        break;

    case MoveVec::Up:
        //��Ɉړ�
        --CalcCursorNum;
        break;

    case MoveVec::Down:
        //���Ɉړ�
        ++CalcCursorNum;
        break;
    }

    //�͈͓��Ɏ��߂�
    CalcCursorNum = max(CalcCursorNum, 0);
    CalcCursorNum = min(CalcCursorNum, (int)m_MenuParamList.size() - 1);

    return CalcCursorNum;
}

void SelectMenuUI::SelectUpdate(bool NoSelect)
{
    //���j���[���ڂ̍X�V
    const int NumMenu = (int)m_MenuParamList.size();

    for (int i = 0; i < NumMenu; ++i)
    {
        auto& it = m_MenuParamList.at(i);

        //���ݑI�𒆂Ȃ�
        if (NoSelect == false && i == m_NowSelectNum)
        {
            //�I��x�������グ��
            it.SelectRatio += (1.0f - it.SelectRatio)*0.2f;
        }
        else
        {
            //�I��x������������
            it.SelectRatio *= 0.2f;
        }
    }

}

bool SelectMenuUI::Update()	//false��Ԃ��Ə�������
{
    //���݂̃X�e�[�g�֐������s
    (this->*m_pStateFunc)();

    //�I���X�e�[�g�Ȃ�false��Ԃ�
    return m_pStateFunc != &SelectMenuUI::State_Delete;
}
