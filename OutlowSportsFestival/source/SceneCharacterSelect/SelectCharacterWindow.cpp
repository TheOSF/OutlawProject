#include "SelectCharacterWindow.h"
#include "../character/CharacterBase.h"
#include "../character/Baseball/BaseballEquip.h"


SelectCharacterWindow::SelectCharacterWindow(
    SelectCursor*   pCursor
    ) :
    m_pCursor(pCursor),
    m_PreFrameIsSelected(false)
{
    //キャラクタを読み込み
    LoadCharacterModels();

    //ランダムメッシュ
 /*   m_pRandomMesh = new MeshRenderer(
        new iexMesh("DATA\\ChrSelect\\Model\\random.imo"), 
        true,
        MeshRenderer::RenderType::UseColor
        );*/
}

SelectCharacterWindow::~SelectCharacterWindow()
{
    for (auto& it : m_ChrRenderers)
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

    //モデルスケール
    const float Scale = 0.055f;
    RADIAN Angle = acosf(-Vector3Normalize(Pos).z);

    if (Pos.x > 0)
    {
        Angle = -Angle;
    }
    
    for (int i = 0; i < (int)m_ChrRenderers.size(); ++i)
    {
        const CharacterType::Value Type = (CharacterType::Value)i;

        //読み込み
        m_ChrRenderers[i] = new CharacterRenderer(
            new BlendAnimationMesh(CharacterBase::GetCharacterModelPath(Type))
            );

        //野球のみバットをつける
        if (Type == CharacterType::_Baseball)
        {
            m_pBaseballEquip =  new BaseballEquip(
                m_ChrRenderers[i],
                BaseballEquip::MeshType::Bat, 
                1.0f
                );

            m_pBaseballEquip->SetVisible(false);
        }

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

        //描画On Off切り替え
        m_ChrRenderers[i]->m_Visible = (Type == m_pCursor->m_PlayerInfo.chr_type);
        
        if (Type == CharacterType::_Americanfootball)
        {
            m_ChrRenderers[i]->SetMotion(2);
        }
        else
        {
            m_ChrRenderers[i]->SetMotion(1);
        }
        
    }
}

Vector3 SelectCharacterWindow::GetCharacterPos()
{
    const Vector3 _1Pl(-6.0f, 0, 13), _2Pl(-2.2f, 0.0f, 15);

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
        MyAssert(false, "対応していないプレイヤー番号");
        break;
    }

    return Vector3Zero;
}



bool SelectCharacterWindow::Update()
{
    CharacterRenderer* pSelectCharacter = nullptr;

    //描画フラグの更新
    for (int i = 0; i < (int)m_ChrRenderers.size(); ++i)
    {
        m_ChrRenderers.at(i)->m_Visible =
            (CharacterType::Value)i == m_pCursor->m_PlayerInfo.chr_type;

        if (m_ChrRenderers.at(i)->m_Visible)
        {
            pSelectCharacter = m_ChrRenderers.at(i);

            //サッカーのみ応急処置
            if (m_pCursor->m_PlayerInfo.chr_type == CharacterType::_Soccer)
            {
                m_ChrRenderers.at(i)->Update(0.5f);
            }
            else
            {
                m_ChrRenderers.at(i)->Update(1);
            }
        }
    }

    //野球バットの描画フラグをセット
    m_pBaseballEquip->SetVisible(m_pCursor->m_PlayerInfo.chr_type == CharacterType::_Baseball);

   // m_pRandomMesh


    if (pSelectCharacter != nullptr)
    {
        //今回フレームで選択していたら
        if (m_PreFrameIsSelected == false &&
            m_pCursor->m_Selected == true
            )
        {
            
        }
    }

    

    //コンピュータ操作で未設定なら//「コンピュータ」ＵＩ表示
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


