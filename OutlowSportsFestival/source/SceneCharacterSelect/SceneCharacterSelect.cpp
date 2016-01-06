#include "SceneCharacterSelect.h"
#include "../Camera/Camera.h"
#include	"../IexSystem/System.h"

SceneCharacterSelect::SceneCharacterSelect(UINT PlayerNum) :
m_Texture("DATA\\Texture\\particle.png")
{
    //	環境設定
    {
        iexLight::SetAmbient(0x404040);
        iexLight::SetFog(800, 1000, 0);

        Vector3 dir(1.0f, -1.0f, -0.5f);
        dir.Normalize();
        iexLight::DirLight(shader, 0, &dir, 0.8f, 0.8f, 0.8f);
    }

    //マネージャ
    m_pManager = new CursorManager();

    
    //キャラクタポイントをセット
    std::array<SelectPointBase*, 4> ChrPoint;

    SetCharacterPoint(ChrPoint);

    //コンピュータ用のポイントをセット
    //SetComputerPoint();

    //カーソルをセット
    for (UINT i = 0; i < PlayerNum; ++i)
    {
        new SelectCursor(m_pManager, (controller::CONTROLLER_NUM)i, &m_Texture, RectI(0,0,128,128), ChrPoint[0]);
    }
}

SceneCharacterSelect::~SceneCharacterSelect()
{
    DefGameObjMgr.Release();

    DefCamera.Release();

    delete m_pManager;
}

void SceneCharacterSelect::Update()
{
    DefCamera.Update();
    DefGameObjMgr.Update();
}

void SceneCharacterSelect::Render()
{
    DefCamera.Clear();
    
    DefRendererMgr.Render();
}


void SceneCharacterSelect::SetCharacterPoint(std::array<SelectPointBase*, 4>& ChrPoint)
{
    //キャラのカーソルポイントたち
    {
        SelectPointBase* p;
        Vector2 Center((float)iexSystem::ScreenWidth*0.5f, (float)iexSystem::ScreenHeight*0.5f);
        const float Space = 120.0f;

        struct
        {
            SelectPointBase::PointType  Type;
            Vector2                     Pos;
        }
        ChrPointData[4] =
        {
            { SelectPointBase::PointType::Tennis, Center + Vector2(-Space, -Space) },
            { SelectPointBase::PointType::Soccer, Center + Vector2(Space, -Space) },
            { SelectPointBase::PointType::BaseBall, Center + Vector2(-Space, Space) },
            { SelectPointBase::PointType::AmericanFootBall, Center + Vector2(Space, Space) },
        };

        for (int i = 0; i < (int)ARRAYSIZE(ChrPointData); ++i)
        {
            p = new SelectPointBase(m_pManager, ChrPointData[i].Type, &m_Texture, RectI(0, 0, 64, 64));
            p->m_Pos = Center;
            p->m_MoveTargetPos = ChrPointData[i].Pos;

            ChrPoint[i] = p;
        }
    }
}


//-------------------------------------------------------------------//
