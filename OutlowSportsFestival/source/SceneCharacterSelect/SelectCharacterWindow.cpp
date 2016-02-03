#include "SelectCharacterWindow.h"
#include "../character/CharacterBase.h"
#include "../character/Baseball/BaseballEquip.h"

#include "character/Tennis/TennisPlayer.h"
#include "character/Amefoot/AmefootPlayer.h"
#include "character/Soccer/SoccerPlayer.h"
#include "character/Baseball/BaseballPlayer.h"
#include "../UI/SceneCharacterSelectUI.h"


static int GetCharacterSelectedMotion(CharacterType::Value type)
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
        MyAssert(false, "使えないキャラクターです。");
        break;
    }
    return 0;
}

static int GetCharacterDeselectedMotion(CharacterType::Value type)
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
        MyAssert(false, "使えないキャラクターです。");
        break;
    }
    return 0;
}

SelectCharacterWindow::SelectCharacterWindow(
    SelectCursor*           pCursor,
    iex2DObj*               pSelectTex
    ) :
    m_pCursor(pCursor),
    m_3DMeshMoveCount(0.0f),
    m_pSelectTex(pSelectTex),
    m_SelectedCount(0.0f)
{
    //キャラクタを読み込み
    LoadCharacterModels();

    //ランダムメッシュ
    m_pRandomMesh = new MeshRenderer(
        new iexMesh("DATA\\ChrSelect\\Model\\random.imo"),
        true,
        MeshRenderer::RenderType::UseColorSpecular
        );

    //COM表示用Mesh
    m_pComMesh = new MeshRenderer(
        new iexMesh("DATA\\ChrSelect\\Model\\com.imo"),
        true,
        MeshRenderer::RenderType::UseColorSpecular
        );

    //とりあえず更新
    Update();
}

SelectCharacterWindow::~SelectCharacterWindow()
{
    for ( auto& it : m_ChrRenderers )
    {
        delete it;
    }

    delete m_pRandomMesh;
    delete m_pComMesh;
}

//描画OnOff切り替え
void SelectCharacterWindow::SelectingRenderer(SelectPointBase::PointType type)
{
    //キャラクタレンダラOnOff
    for (int i = 0; i < 4; ++i)
    {
        m_ChrRenderers[i]->m_Visible = ((int)type == i);
    }

    //Com
    if (type == SelectPointBase::PointType::ComputerChrSelect ||
        type == SelectPointBase::PointType::ComputerDefaultPoint ||
        type == SelectPointBase::PointType::ComputerStrong
        )
    {
        m_pComMesh->m_Visible = true;
    }
    else
    {
        m_pComMesh->m_Visible = false;
    }

    //Random
    if (type == SelectPointBase::PointType::Random)
    {
        m_pRandomMesh->m_Visible = true;
    }
    else
    {
        m_pRandomMesh->m_Visible = false;
    }

}

void SelectCharacterWindow::LoadCharacterModels()
{

    Matrix Work;
    const Vector3 Pos = GetCharacterPos();

    //モデルスケール
    const float Scale = 0.055f;
    RADIAN Angle = acosf(-Vector3Normalize(Pos).z);

    if ( Pos.x > 0 )
    {
        Angle = -Angle;
    }

    for ( int i = 0; i < (int)m_ChrRenderers.size(); ++i )
    {
        const CharacterType::Value Type = (CharacterType::Value)i;

        //読み込み
        m_ChrRenderers[i] = new CharacterRenderer(
            new BlendAnimationMesh(CharacterBase::GetCharacterModelPath(Type))
            );

        //構築
        CharacterBase::CreateCharacterModel(m_ChrRenderers[i], Type, (PlayerNum::Value)m_pCursor->m_PlayerInfo.number);

        //ワールド変換行列セット
        {
            Matrix* const T = &m_ChrRenderers[i]->m_TransMatrix;

            D3DXMatrixScaling(&Work, Scale, Scale, Scale);
            D3DXMatrixRotationY(T, Angle);

            *T *= Work;

            T->_41 = Pos.x;
            T->_42 = Pos.y;
            T->_43 = Pos.z;
        }

        //描画Offにセット
        m_ChrRenderers[i]->m_Visible = false;

        m_ChrRenderers[i]->SetMotion(
            GetCharacterDeselectedMotion(Type)
            );

        //更新
  //      m_ChrRenderers[i]->Update(0.1f);
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
        MyAssert(false, "対応していないプレイヤー番号");
        break;
    }

    return Vector3Zero;
}

Vector2 SelectCharacterWindow::GetUIPos()
{
    Vector2 Center((float)iexSystem::ScreenWidth*0.5f, (float)iexSystem::ScreenHeight*0.5f);

    Vector2 _1Pl(-420, 0), _2Pl(-160, -20);
    Vector2 ret(0, 0);

    switch (m_pCursor->m_PlayerInfo.number)
    {
    case 0:
        ret = Center + _1Pl;
        break;

    case 1:
        ret = Center + _2Pl;
        break;

    case 2:
        ret = Center +  Vector2(-_2Pl.x, _2Pl.y);
        break;

    case 3:
        ret = Center + Vector2(-_1Pl.x, _1Pl.y);
        break;

    default:
        MyAssert(false, "プレイヤー番号がおかしい！");
        break;
    }

    return ret;
}

bool SelectCharacterWindow::Update()
{
    //現在選択しているポイントのタイプ
    SelectPointBase::PointType PointType = m_pCursor->GetPoint()->m_Type;

    //COM　で　すでにキャラクタを選択済みの場合 キャラクタのタイプをセットする
    if (m_pCursor->m_PlayerInfo.player_type == PlayerType::_Computer &&
        m_pCursor->m_PlayerInfo.chr_type != CharacterType::__ErrorType &&
        PointType == SelectPointBase::PointType::ComputerDefaultPoint
        )
    {
        switch (m_pCursor->m_PlayerInfo.chr_type)
        {
        case CharacterType::_Tennis:
            PointType = SelectPointBase::PointType::Tennis;
            break;

        case CharacterType::_Soccer:
            PointType = SelectPointBase::PointType::Soccer;
            break;

        case CharacterType::_Baseball:
            PointType = SelectPointBase::PointType::BaseBall;
            break;

        case CharacterType::_Americanfootball:
            PointType = SelectPointBase::PointType::AmericanFootBall;
            break;

        default:

            break;
        }
         
    }


    //描画On Off切り替え
    SelectingRenderer(PointType);

    //選択してるやつの更新
    ModelUpdate(PointType);

    //選択カウント
    if (m_pCursor->m_Selected)
    {
        m_SelectedCount += (1 - m_SelectedCount)*0.25f;
    }
    else
    {
        m_SelectedCount -= m_SelectedCount * 0.25f;
    }

    return true;
}

bool SelectCharacterWindow::Msg(MsgType mt)
{

    return false;
}

void SelectCharacterWindow::ModelUpdate(SelectPointBase::PointType type)
{
    switch (type)
    {
        //キャラクタ更新
    case SelectPointBase::PointType::Tennis:
    case SelectPointBase::PointType::AmericanFootBall:
        m_ChrRenderers.at((int)type)->Update(1);
        break;

        //スロー再生
    case SelectPointBase::PointType::Soccer:
    case SelectPointBase::PointType::BaseBall:
        m_ChrRenderers.at((int)type)->Update(0.5f);
        break;

        //COM ３Dモデル更新
    case SelectPointBase::PointType::ComputerChrSelect:
    case SelectPointBase::PointType::ComputerDefaultPoint:
    case SelectPointBase::PointType::ComputerStrong:
        m_pComMesh->SetMatrix(Calc3DMeshMatrix());
        break;

        //RANDOM　３Dモデル更新
    case SelectPointBase::PointType::Random:
        m_pRandomMesh->SetMatrix(Calc3DMeshMatrix());
        break;

    default:
        MyAssert(false, "選択しているポイントタイプの値がおかしい！")
        break;
    }
}

Matrix SelectCharacterWindow::Calc3DMeshMatrix()
{
    Matrix Out,S;

    m_3DMeshMoveCount += 0.01f;

    if (m_3DMeshMoveCount > 10000.0f)
    {
        m_3DMeshMoveCount = 0.0f;
    }

    const float Angle = sinf(m_3DMeshMoveCount) * D3DXToRadian(10) + PI;
    const float DiffPosY = sinf(m_3DMeshMoveCount*2.0f)*0.3f;
    Vector3 Pos = GetCharacterPos();

    D3DXMatrixRotationY(&Out, Angle);

    {
        D3DXMatrixScaling(&S, 0.15f, 0.15f, 0.15f);
        Out *= S;
    }

    Pos.y += DiffPosY + 2.3f;

    Out._41 = Pos.x;
    Out._42 = Pos.y;
    Out._43 = Pos.z;

    return Out;
}

void SelectCharacterWindow::Render()
{
    const int W = (int)(m_SelectedCount * 218);
    const int H = (int)(m_SelectedCount * 155);
    
    Vector2 pos = GetUIPos();

    m_pSelectTex->Render(
        (int)pos.x - W / 2,
        (int)pos.y - H / 2,
        W,
        H,

        218 * m_pCursor->m_PlayerInfo.number, 
        577,
        218,
        155
        );

}


void SelectCharacterWindow::CalcZ()
{
    m_SortZ = 0;
}


