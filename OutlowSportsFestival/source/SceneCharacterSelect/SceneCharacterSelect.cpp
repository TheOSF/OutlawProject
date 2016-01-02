#include "SceneCharacterSelect.h"
#include "../Camera/Camera.h"
#include	"../IexSystem/System.h"

SceneCharacterSelect::SceneCharacterSelect(UINT PlayerNum) :
m_Texture("DATA\\Texture\\particle.png")
{
    CursorPoint* p;

    m_pManager = new CursorManager();

    for (int i = 0; i < 4; ++i)
    {
        p = new CursorPoint(m_pManager);
        
        p->m_Pos = Vector2(i % 2, i / 2)*200.0f + Vector2(300, 200);
    }

    m_pCursor = new CursorObject(m_pManager, p, 0);

    //	ŠÂ‹«Ý’è
    {
        iexLight::SetAmbient(0x404040);
        iexLight::SetFog(800, 1000, 0);

        Vector3 dir(1.0f, -1.0f, -0.5f);
        dir.Normalize();
        iexLight::DirLight(shader, 0, &dir, 0.8f, 0.8f, 0.8f);
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
    int W = 100, H = 100;

    DefCamera.Clear();

    iexPolygon::Rect(
        0,
        0,
        100,
        100,
        RS_COPY,
        0xFFFFFF80
        );

    m_Texture.Render(
        m_pCursor->m_Pos.x - W / 2,
        m_pCursor->m_Pos.y - H / 2, 
        W,
        H,
        
        0,
        0,
        128,
        128

        );

    
}

