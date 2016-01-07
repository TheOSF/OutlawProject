#include "SelectCharacterWindow.h"
#include "../character/CharacterBase.h"

SelectCharacterWindow::SelectCharacterWindow(
    SelectCursor*   pCursor
    ):
    m_pCursor(pCursor)
{
    //�L�����N�^��ǂݍ���
    LoadCharacterModels();
}

SelectCharacterWindow::~SelectCharacterWindow()
{
    for (auto& it : m_ChrRenderers)
    {
        delete it;
    }
}

void SelectCharacterWindow::LoadCharacterModels()
{

    Matrix Work;
    const Vector3 Pos = GetCharacterPos();

    //���f���X�P�[��
    const float Scale = 0.055f;
    RADIAN Angle = acosf(-Vector3Normalize(Pos).z);

    if (Pos.x > 0)
    {
        Angle = -Angle;
    }
    
    for (int i = 0; i < (int)m_ChrRenderers.size(); ++i)
    {
        const CharacterType::Value Type = (CharacterType::Value)i;

        //�ǂݍ���
        m_ChrRenderers[i] = new CharacterRenderer(
            new BlendAnimationMesh(CharacterBase::GetCharacterModelPath(Type))
            );

        //�\�z
        CharacterBase::CreateCharacterModel(m_ChrRenderers[i], Type, (PlayerNum::Value)m_pCursor->m_PlayerInfo.number);

        //���[���h�ϊ��s��Z�b�g
        {
            Matrix* const T = &m_ChrRenderers[i]->m_TransMatrix;

            D3DXMatrixScaling(&Work, Scale, Scale, Scale);
            D3DXMatrixRotationY(T, Angle);
            
            *T *= Work;

            T->_41 = Pos.x;
            T->_42 = Pos.y;
            T->_43 = Pos.z;
        }

        //�`��On Off�؂�ւ�
        m_ChrRenderers[i]->m_Visible = (Type == m_pCursor->m_PlayerInfo.chr_type);
        
        m_ChrRenderers[i]->SetMotion(1);
    }
}

Vector3 SelectCharacterWindow::GetCharacterPos()
{
    const Vector3 _1Pl(-6.0f, 0, 13), _2Pl(-2.2f, 0.0f, 16);

    switch (m_pCursor->m_PlayerInfo.number)
    {
    case 0:
        return _1Pl;
    case 1:
        return _2Pl;
    case 2:
        return Vector3(-_2Pl.x, _2Pl.y, _2Pl.z);
    case 3:
        return Vector3(-_1Pl.x, _1Pl.y, _1Pl.z);

    default:
        MyAssert(false, "�Ή����Ă��Ȃ��v���C���[�ԍ�");
        break;
    }

    return Vector3Zero;
}



bool SelectCharacterWindow::Update()
{
    //�`��t���O�̍X�V
    for (int i = 0; i < (int)m_ChrRenderers.size(); ++i)
    {
        m_ChrRenderers.at(i)->m_Visible =
            (CharacterType::Value)i == m_pCursor->m_PlayerInfo.chr_type;

        if (m_ChrRenderers.at(i)->m_Visible)
        {
            m_ChrRenderers.at(i)->Update(1);
        }
    }

    //�R���s���[�^����Ŗ��ݒ�Ȃ�//�u�R���s���[�^�v�t�h�\��
    m_DrawComputerUI = 
        (m_pCursor->m_PlayerInfo.chr_type == CharacterType::__ErrorType);


    return true;
}

bool SelectCharacterWindow::Msg(MsgType mt)
{

    return false;
}

void SelectCharacterWindow::Render()
{
    if (m_DrawComputerUI == false)
    {
        return;
    }

    //

}


void SelectCharacterWindow::CalcZ()
{
    m_SortZ = 0;
}


