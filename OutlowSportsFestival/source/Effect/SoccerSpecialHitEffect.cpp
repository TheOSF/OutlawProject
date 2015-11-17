#include "SoccerSpecialHitEffect.h"
#include "../GameSystem/ResourceManager.h"
#include "ParticleRenderer.h"
#include "ParticleMoveObject.h"
#include "../GameSystem/ResourceManager.h"
#include "HitEffectObject.h"
#include "AnimationBordRenderer.h"
#include "EffectFactory.h"
#include "GlavityLocus.h"


SoccerSpecialHit::SoccerSpecialHit(
    CharacterBase* pOwner,//親キャラクタへのポインタ
    CrVector3      pos,   //出現座標
    RATIO          level,  //エフェクトのクオリティ(０～１)
    UINT           time
    ) :
    m_pOwner(pOwner),
    m_Pos(pos),
    m_Level(level),
    m_Count(0),
    m_pStateFunc(&SoccerSpecialHit::State_Init),
    m_LiveTime((int)time)
{

    //ダメージ設定
    m_Damage.HitCount = 0;
    m_Damage.m_Enable = true;
    m_Damage.m_Param.pos1 = m_Pos;
    m_Damage.m_Param.pos2 = m_Pos + Vector3(0, 20, 0);
    m_Damage.m_Param.width = 3.5f;
    m_Damage.m_Vec = Vector3AxisZ;
    m_Damage.m_VecPower = Vector2(0.1f, 0.3f);
    m_Damage.m_VecType = DamageCapsure::DamageVecType::PosToCenterXZ;
    m_Damage.pBall = nullptr;
    m_Damage.pParent = pOwner;
    m_Damage.type = DamageBase::Type::_VanishDamage;
    m_Damage.Value = 2.5f*level;

    //竜巻
    TornadoEffect::Param p;

    p.Length = 17.0f;
    p.LocusWidthEnd = 0.4f;
    p.LocusWidthStart = 1.5f;
    p.lowWidth = 0.1f;
    p.middleWidth = 1.5f;
    p.highWidth = 0.1f;
    p.pos = m_Pos + Vector3(0, -5, 0);
    p.right = Vector3AxisX;
    p.RotateSpeed = 0.8f;
    p.vec = Vector3AxisY;
    p.middle_height = 12;
    
    m_pTornadoEffect = new TornadoEffect(
        p,
        3,
        40
        );


}

SoccerSpecialHit::~SoccerSpecialHit()
{
    
}

bool SoccerSpecialHit::Update()
{
    //現在のステートを実行
    (this->*m_pStateFunc)();

    //終了ステートでなければtrueを返す
    return m_pStateFunc != &SoccerSpecialHit::State_Finish;
}

bool SoccerSpecialHit::Msg(MsgType mt)
{
    //特にすることなし
    return false;
}

void SoccerSpecialHit::EffectApper(int n, RATIO scale)
{
    for (int i = 0; i < n; ++i)
    {
        Vector3 tvec(Vector3Normalize(Vector3Rand()));

        AnimationBordRenderer* r = new AnimationBordRenderer(
            DefResource.Get(Resource::TextureType::Anime_Circle),
            4,
            4,
            16,
            0x80FFFFFF,
            0x80FFFFFF
            );

        r->m_Pos = m_Pos + Vector3(0, 3, 0);
        r->m_CellCount = 0;
        r->m_Size = Vector2(2, 2)*scale;


        Vector3Cross(r->m_Right, tvec, Vector3AxisX);

        if (r->m_Right == Vector3Zero)
        {
            Vector3Cross(r->m_Right, tvec, Vector3AxisZ);
        }

        Vector3Cross(r->m_Up, r->m_Right, tvec);


        r->m_Right.Normalize();
        r->m_Up.Normalize();

        AnimationBordGameObj* m = new AnimationBordGameObj(
            r
            );

        m->animation_end_delete = true;
        m->animation_loop = false;
        m->animation_speed = 1.0f;

        m->move_power = Vector3Zero;
        m->move_speed = Vector3Zero;

        m->scale_speed = Vector2(1, 1)*1.2f*scale;
    }
}

void SoccerSpecialHit::Particle(int n)
{
    Vector3 power(0, -0.02f, 0);
    COLORf Color(0.1f, 1, 1, 1), HdrColor(0.5f, 1, 1, 1);

    for (int i = 0; i < n; ++i)
    {
        EffectFactory::LocusParticle(
            m_Pos,
            Vector3Rand()*2.0f, 
            power,
            0.05f,
            4,
            Color,
            HdrColor,
            120,
            0.5f
            );
    }
}

//--------------------------State_Func-----------------------------//

void SoccerSpecialHit::State_Init()
{

    //ブラー縮小ステートへ
    m_pStateFunc = &SoccerSpecialHit::State_BlurToSmal;
}

void SoccerSpecialHit::State_BlurToSmal()
{
    m_Count++;

    //間隔をあけてダメージを有効にする
    if (m_Count < m_LiveTime-5)
    {
        m_Damage.m_Enable = (m_Count % 7 == 0) || (m_Count % 7 == 1);
    }
    else
    {
        m_Damage.m_Enable = false;
    }
  

    //パワーを弱める
    if (m_Count > m_LiveTime)
    {
        const float smal_val = 0.8f;

        m_pTornadoEffect->m_Param.LocusWidthStart *= smal_val;
        m_pTornadoEffect->m_Param.LocusWidthEnd *= smal_val;

        m_pTornadoEffect->m_Param.highWidth *= smal_val;
        m_pTornadoEffect->m_Param.middleWidth *= smal_val;
        m_pTornadoEffect->m_Param.lowWidth *= smal_val;
    }
    else
    {
        m_pTornadoEffect->m_Param.highWidth += (8.0f - m_pTornadoEffect->m_Param.highWidth)*0.07f;
        m_pTornadoEffect->m_Param.lowWidth += (4.0f - m_pTornadoEffect->m_Param.lowWidth)*0.07f;;
    }

    
    if (m_Count > m_LiveTime + 15)
    {
        //終了ステートへ
        m_pStateFunc = &SoccerSpecialHit::State_Finish;

        m_pTornadoEffect->Destroy(); //Gameobjectはdeleteできないため
        
    }
}


void SoccerSpecialHit::State_Finish()
{

}