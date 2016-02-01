#include "SelectCharacterWindow.h"
#include "../character/CharacterBase.h"
#include "../character/Baseball/BaseballEquip.h"

#include "character/Tennis/TennisPlayer.h"
#include "character/Amefoot/AmefootPlayer.h"
#include "character/Soccer/SoccerPlayer.h"
#include "character/Baseball/BaseballPlayer.h"
#include "../UI/SceneCharacterSelectUI.h"


int GetCharacterSelectedMotion(CharacterType::Value type)
{

    switch ( type )
    {
    case CharacterType::_Tennis:
        return TennisPlayer::_mt_WinPose;
    case CharacterType::_Baseball:
        return 1;
    case CharacterType::_Soccer:
        return SoccerPlayer::_ms_Win;
    case CharacterType::_Americanfootball:
        return AmefootPlayer::Motion_Win_Pose;
    case CharacterType::_Volleyball:
    case CharacterType::_Lacrosse:
    case CharacterType::__ErrorType:
    default:
        MyAssert(false, "�g���Ȃ��L�����N�^�[�ł��B");
        break;
    }
    return 0;
}

int GetCharacterDeselectedMotion(CharacterType::Value type)
{

    switch ( type )
    {
    case CharacterType::_Tennis:
        return TennisPlayer::_mt_Stand;
    case CharacterType::_Baseball:
        return baseball_player::_mb_Stand_B;
    case CharacterType::_Soccer:
        return SoccerPlayer::_ms_Stand;
    case CharacterType::_Americanfootball:
        return AmefootPlayer::Motion_Stand;
    case CharacterType::_Volleyball:
    case CharacterType::_Lacrosse:
    case CharacterType::__ErrorType:
    default:
        MyAssert(false, "�g���Ȃ��L�����N�^�[�ł��B");
        break;
    }
    return 0;
}

SelectCharacterWindow::SelectCharacterWindow(
    SelectCursor*   pCursor,
    SceneCharacterSelectUI* pComUI
    ) :
    m_pCursor(pCursor),
    m_PreFrameIsSelected(false),
    m_pComUI(pComUI)
{
    //�L�����N�^��ǂݍ���
    LoadCharacterModels();

    for ( auto& it : m_ChrRenderers )
    {
        it->Update(1.0f);
    }

    //�����_�����b�V��
 /*   m_pRandomMesh = new MeshRenderer(
        new iexMesh("DATA\\ChrSelect\\Model\\random.imo"),
        true,
        MeshRenderer::RenderType::UseColor
        );*/
}

SelectCharacterWindow::~SelectCharacterWindow()
{
    for ( auto& it : m_ChrRenderers )
    {
        delete it;
    }
}

void SelectCharacterWindow::DoMotion()
{

}


void SelectCharacterWindow::SelectingRenderer(SelectPointBase::PointType type)
{

}

void SelectCharacterWindow::LoadCharacterModels()
{

    Matrix Work;
    const Vector3 Pos = GetCharacterPos();

    //���f���X�P�[��
    const float Scale = 0.055f;
    RADIAN Angle = acosf(-Vector3Normalize(Pos).z);

    if ( Pos.x > 0 )
    {
        Angle = -Angle;
    }

    for ( int i = 0; i < (int)m_ChrRenderers.size(); ++i )
    {
        const CharacterType::Value Type = (CharacterType::Value)i;

        //�ǂݍ���
        m_ChrRenderers[i] = new CharacterRenderer(
            new BlendAnimationMesh(CharacterBase::GetCharacterModelPath(Type))
            );

        //�싅�̂݃o�b�g������
        if ( Type == CharacterType::_Baseball )
        {
            m_pBaseballEquip = new BaseballEquip(
                m_ChrRenderers[i],
                BaseballEquip::MeshType::Bat,
                1.0f
                );

            m_pBaseballEquip->SetVisible(false);
        }

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

        m_ChrRenderers[i]->SetMotion(
            GetCharacterDeselectedMotion(Type)
            );

    }
}

Vector3 SelectCharacterWindow::GetCharacterPos()
{
    const Vector3 _1Pl(-6.0f, 0, 13), _2Pl(-2.2f, 0.0f, 15);

    switch ( m_pCursor->m_PlayerInfo.number )
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
    CharacterRenderer* pSelectCharacter = nullptr;

    //�`��t���O�̍X�V
    for ( int i = 0; i < (int)m_ChrRenderers.size(); ++i )
    {
        m_ChrRenderers.at(i)->m_Visible =
            (CharacterType::Value)i == m_pCursor->m_PlayerInfo.chr_type;

        if ( m_ChrRenderers.at(i)->m_Visible )
        {
            pSelectCharacter = m_ChrRenderers.at(i);

            //�T�b�J�[�̂݉��}���u
            if ( m_pCursor->m_PlayerInfo.chr_type == CharacterType::_Soccer )
            {
                m_ChrRenderers.at(i)->Update(0.5f);
            }
            else
            {
                m_ChrRenderers.at(i)->Update(1);
            }
        }
    }

    //�싅�o�b�g�̕`��t���O���Z�b�g
    m_pBaseballEquip->SetVisible(m_pCursor->m_PlayerInfo.chr_type == CharacterType::_Baseball);

    // m_pRandomMesh


    if ( pSelectCharacter != nullptr )
    {
        //����t���[���őI�����Ă�����
        if ( m_PreFrameIsSelected == false &&
            m_pCursor->m_Selected == true
            )
        {
            m_PreFrameIsSelected = true;
            m_ChrRenderers.at((int)m_pCursor->m_PlayerInfo.chr_type)->SetMotion(
                GetCharacterSelectedMotion(m_pCursor->m_PlayerInfo.chr_type)
                );
        }
        else if ( m_PreFrameIsSelected == true &&
            m_pCursor->m_Selected == false )
        {
            m_PreFrameIsSelected = false;
            m_ChrRenderers.at((int)m_pCursor->m_PlayerInfo.chr_type)->SetMotion(
                GetCharacterDeselectedMotion(m_pCursor->m_PlayerInfo.chr_type)
                );
        }
    }

    //�R���s���[�^����Ŗ��ݒ�Ȃ�//�u�R���s���[�^�v�t�h�\��
    if ( m_pComUI )
    {
        m_pComUI->m_Visible =
            (m_pCursor->m_PlayerInfo.chr_type == CharacterType::__ErrorType);
    }

    return true;
}

bool SelectCharacterWindow::Msg(MsgType mt)
{

    return false;
}

void SelectCharacterWindow::Render()
{

}


void SelectCharacterWindow::CalcZ()
{
    m_SortZ = 0;
}


