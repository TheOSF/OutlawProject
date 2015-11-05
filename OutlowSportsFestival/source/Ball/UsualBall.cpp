#include "UsualBall.h"
#include "../character/CharacterBase.h"
#include "../Render/MeshRenderer.h"
#include "../Camera/Camera.h"
#include "../GameSystem/ResourceManager.h"
#include "../Stage/PhysicsMeshObject.h"
#include "../Collision/Collision.h"
#include "BallFadeOutRenderer.h"
#include "../Effect/ParticleHDRRenderer.h"
#include "../Effect/ParticleMoveObject.h"
#include "../Effect/EffectFactory.h"


UsualBall::UsualBall(
    BallBase::Params	params,			//ボールパラメータ
    DamageBase::Type	damage_type,	//ダメージ判定のタイプ
    float				damage_val,		//ダメージ量
    UINT                hit_num         //ヒット数
    ) :
    m_DeleteFrame(180),
    m_Locus(15),
    m_pRigitBody(nullptr),
    m_HitNum(hit_num),
    m_HitCountSave(0),
    m_HitStopFrame(0),
    m_pStateFunc(&UsualBall::StateFlyMove),
    m_RotateSpeed(0.15f, 0.05f, 0.05f),
    m_EffectFrameCount(0),
    m_FirstParentType(params.pParent->m_PlayerInfo.chr_type)
{

    class PhysicUpdateCallBack :public MeshRenderer::PreRenderCallBack
    {
        RigidBody* const pRigidBody;
    public:
        PhysicUpdateCallBack(RigidBody* const pRigidBody) :pRigidBody(pRigidBody){}

        void Execute(MeshRenderer* pRenderer, Type type)
        {

        }
    };

	LPIEXMESH		pBallMesh;

	//パラメータ代入
    m_Params = params;

    {
        //ダメージ判定のパラメータを代入
        m_Damage.pBall = this;
        m_Damage.pParent = params.pParent;
        m_Damage.m_Param.width = 2.0f;	//大きさはボールによって異なる?
        m_Damage.type = damage_type;
        m_Damage.Value = damage_val;
        m_Damage.m_Enable = true;
        m_Damage.m_Param.pos1 = m_Params.pos;
        m_Damage.m_Param.pos2 = m_Params.pos;

        UpdateDamageClass();
    }

    {
        const float MeshScale = GetBallScale(params.pParent->m_PlayerInfo.chr_type);
        Matrix m;

        //ボールのメッシュを作成
        GetBallMesh(params.pParent->m_PlayerInfo.chr_type, &pBallMesh);

        //メッシュのレンダラー作成(最終的にメッシュを使いまわして描画するべき)
        m_pMeshRenderer = new MeshRenderer(
            pBallMesh,
            false,
            MeshRenderer::RenderType::UseColorSpecular
            );

        D3DXMatrixScaling(&m, MeshScale, MeshScale, MeshScale);

        m._41 = m_Params.pos.x;
        m._42 = m_Params.pos.y;
        m._43 = m_Params.pos.z;

        m_pMeshRenderer->SetMatrix(m);
    }


    {
        //軌跡の設定
        m_Locus.m_Division = 0;
        m_Locus.m_StartParam.Width = 3.6f * GetBallScale(params.pParent->m_PlayerInfo.chr_type);
        m_Locus.m_EndParam.Width = 0.1f;

        UpdateColor();
    }

  

}

UsualBall::~UsualBall()
{
    delete m_pMeshRenderer;
    if (m_pRigitBody != nullptr)
    {
        DefBulletSystem.RemoveRigidBody(m_pRigitBody);
    }
}

bool UsualBall::GetBallMesh(
	CharacterType::Value	type,
	LPIEXMESH*				ppOut)
{
	*ppOut = nullptr;

	switch (type)
	{
	case CharacterType::_Tennis:
        *ppOut = DefResource.Get(Resource::MeshType::Tennis_ball);
		break;
	case CharacterType::_Baseball:
        *ppOut = DefResource.Get(Resource::MeshType::BaseBall_ball);
		break;
	case CharacterType::_Americanfootball:
        *ppOut = DefResource.Get(Resource::MeshType::Amefoot_ball);
		break;
	case CharacterType::_Soccer:
        *ppOut = DefResource.Get(Resource::MeshType::Soccer_ball);
		break;
	case CharacterType::_Lacrosse:
        *ppOut = DefResource.Get(Resource::MeshType::Lacrosse_ball);
		break;
	case CharacterType::_Volleyball:
        *ppOut = DefResource.Get(Resource::MeshType::Volley_ball);
	default:
		break;
	}

	return (*ppOut != nullptr);
}


float UsualBall::GetBallScale(
    CharacterType::Value	type    //ボールのキャラクタタイプ
    )
{
    switch (type)
    {
    case CharacterType::_Americanfootball:
        return 0.18f;

    case CharacterType::_Baseball:
        return 0.1f;

    case CharacterType::_Lacrosse:
        return 0.18f;

    case CharacterType::_Soccer:
        return 0.18f;


    case CharacterType::_Tennis:
        return 0.1f;


    case CharacterType::_Volleyball:
        return 0.18f;

    default:
        break;
    }

    MyAssert(false, "存在しないタイプのキャラクタタイプがUsualBall::GetBallScaleに渡されました　type= %d ", (int)type);


    return 0;
}

UsualBall::PhysicsParam UsualBall::GetBallPhysics(
    CharacterType::Value	type	//ボールのキャラクタタイプ
    )
{
    PhysicsParam params[]=
    {
        { 0.5f, 100.0f, 0.36f, 0.2f },
        { 0.5f, 100.0f, 0.36f, 0.2f },
        { 0.5f, 100.0f, 0.55f, 0.2f },
        { 0.5f, 100.0f, 0.5f, 0.2f },
        { 0.5f, 100.0f, 0.5f, 0.2f },
        { 0.5f, 100.0f, 0.5f, 0.2f },
    };                     

    MyAssert((int)type >= 0 && (int)type < (int)ARRAYSIZE(params), "存在しないタイプのキャラクタタイプがUsualBall::GetBallPhysicsに渡されました　type= %d ", (int)type);

    return params[(int)type];
}

bool UsualBall::Update()
{
    return (this->*m_pStateFunc)();       
}


bool UsualBall::Msg(MsgType mt)
{

	return false;
}

void UsualBall::SetRotateSpeed(CrVector3 AngleSpeed)
{
    m_RotateSpeed = AngleSpeed;
}


bool UsualBall::isOutofField()const
{
	return Vector3Length(m_Params.pos) > 100;
}


void UsualBall::UpdateDamageClass()
{
	m_Damage.vec = m_Params.move;

    m_Damage.m_Param.pos2 = m_Damage.m_Param.pos1;

    {
        m_Damage.m_Param.pos1 = m_Params.pos;

        if (Vector3Distance(m_Damage.m_Param.pos1, m_Damage.m_Param.pos2) < m_Damage.m_Param.width)
        {
            Vector3 v = m_Damage.m_Param.pos2 - m_Damage.m_Param.pos1;

            v.Normalize();

            m_Damage.m_Param.pos2 = m_Damage.m_Param.pos1 + v*m_Damage.m_Param.width;
        }
    }
	
}


void UsualBall::UpdateColor()
{
    const COLORf Color = CharacterBase::GetPlayerColorF(m_Params.pParent->m_PlayerInfo.number);

    m_Locus.m_StartParam.Color = Color.toVector4();
    m_Locus.m_StartParam.Color.w = 0.3f;
    m_Locus.m_StartParam.HDRColor = m_Locus.m_StartParam.Color;
   
    m_Locus.m_StartParam.HDRColor.w = 0.5f;

    m_Locus.m_EndParam.Color = m_Locus.m_StartParam.Color;
    m_Locus.m_EndParam.Color.w = 0;

    m_Locus.m_EndParam.HDRColor = m_Locus.m_StartParam.HDRColor;
    m_Locus.m_EndParam.HDRColor.w = 0;


    {
        //メッシュを光らせる
        COLORf Color = CharacterBase::GetPlayerColorF(m_Params.pParent->m_PlayerInfo.number);

        m_pMeshRenderer->m_HDR = Vector3(1,1,1) * 0.1f;

    }
}


bool UsualBall::UpdateWallCheck(Vector3& outNewMove)
{
    Vector3 Out, Pos(m_Params.pos), Vec(m_Params.move);
    float Dist = m_Params.move.Length()*2.0f;
    int Material;

    Vec.Normalize();

    if (DefCollisionMgr.RayPick
		(
        &Out,
        &Pos,
        &Vec,
        &Dist,
        &Material,
        CollisionManager::RayType::_Usual
		)
		)
    {
        outNewMove = Vector3Refrect(m_Params.move, Vec);
        outNewMove *= 0.75f;
        return true;
    }

    return false;
}

void UsualBall::AddLocusPoint()
{
    Vector3 v;
    Vector3Cross(v, m_Params.move, DefCamera.GetForward());
    v.Normalize();

    m_Locus.AddPoint(m_Params.pos, v);
}

void UsualBall::Counter(CharacterBase* pCounterCharacter)
{
    m_Damage.pParent = m_Params.pParent = pCounterCharacter;

    UpdateColor();

    m_Damage.type = DamageBase::Type::_VanishDamage;
    m_Damage.Value += 1.0f; //ダメージを増やす

    //エフェクトカウント設定
    m_EffectFrameCount = 45;


    //EffectFactory::CircleAnimationBillbord(
    //    m_Params.pos,
    //    Vector3Zero,
    //    Vector3Zero,
    //    Vector2(8, 10), 
    //    0xFFFFFFFF,
    //    RS_ADD
    //    );

}

void UsualBall::ToNoWork()
{
    //すでに攻撃判定のない状態になっていたらreturn
    if (m_Params.type == BallBase::Type::_DontWork)
    {
        return;
    }

    //攻撃判定のない状態にする
    m_Params.type = BallBase::Type::_DontWork;
    m_Damage.m_Enable = false;

    //ステート関数を切り替え
    m_pStateFunc = &UsualBall::StatePhysicMove;

    m_BaseMatrix = m_pMeshRenderer->GetMatrix();

    m_BaseMatrix._41 = 0;
    m_BaseMatrix._42 = 0;
    m_BaseMatrix._43 = 0;

    const PhysicsParam p = GetBallPhysics(m_Params.pParent->m_PlayerInfo.chr_type);

    m_pRigitBody = DefBulletSystem.AddRigidSphere(
        p.Mass,
        RigidBody::ct_dynamic,
        m_Params.pos,
        Vector3Zero,
        p.Radius,
        p.Friction,
        p.Restitution,
        m_Params.move * 45.0f
        );
}

RATIO UsualBall::GetMovePower()const
{
    const float m = m_Params.move.Length();
    return min(m / 1.5f, 0.8f);
}

bool UsualBall::StateFlyMove()
{
    //移動更新
    {
        //ヒットストップフレームなら移動更新をしない
        if (m_HitStopFrame == 0)
        {
            m_Params.pos += m_Params.move;
        }
        else
        {
            //ヒットストップカウント減少
            --m_HitStopFrame;
        }
    }

    //ダメージ関連の更新
    {
        //もしダメージカウントが前回と違った(攻撃があたったならヒットストップフレームを設定)
        if (m_Damage.HitCount != m_HitCountSave)
        {
            m_HitCountSave = m_Damage.HitCount;
            m_HitStopFrame = 5; //適当です
        }

        //ヒット最大値ならダメージ判定のない状態へ移行する
        if (m_Damage.HitCount >= (int)m_HitNum)
        {
            m_Params.move *= -0.25f;
            m_Params.move.y += 0.2f;


            //攻撃判定のない状態にする
            ToNoWork();

        }

        //ダメージ判定の位置を現在の位置に更新
        UpdateDamageClass();
}

    //ステージとのあたり判定
    {
        //もし壁に当たっていたらダメージ判定のない状態へ移行する
        Vector3 NewMoveVec(0, 0, 0);

        if (UpdateWallCheck(NewMoveVec))
        {
            //新しい移動値をセット
            m_Params.move = NewMoveVec;

            //攻撃判定のない状態にする
            ToNoWork();
        }
    }

    //メッシュ更新
    {
        Matrix m = m_pMeshRenderer->GetMatrix();
        
        m._41 = m._42 = m._43 = 0.0f;
        
        {
            Matrix R;
            //回転
            D3DXMatrixRotationYawPitchRoll(&R, m_RotateSpeed.x, m_RotateSpeed.y, m_RotateSpeed.z);
            m *= R;
        }
        
        m._41 = m_Params.pos.x;
        m._42 = m_Params.pos.y;
        m._43 = m_Params.pos.z;

        m_pMeshRenderer->SetMatrix(m);

        //軌跡の点を追加
        AddLocusPoint();
    }

    //パーティクル
    {
        const float EffectScale = UsualBall::GetBallScale(m_FirstParentType);

        m_EffectFrameCount = max(m_EffectFrameCount - 1, 0);

        if (m_EffectFrameCount % 2 == 0 && m_EffectFrameCount > 0)
        {
            EffectFactory::CircleAnimation(
                m_Params.pos,
                m_Params.move,
                Vector3Zero,
                Vector3Zero,
                Vector2(23.f, 23.f)*EffectScale*((float)m_EffectFrameCount / 45.0f),
                0xFFFFFFFF,
                CharacterBase::GetPlayerColor(m_Params.pParent->m_PlayerInfo.number)
                );
        }
    }


    //フィールド外なら更新失敗
    return !isOutofField();
}

bool UsualBall::StatePhysicMove()
{
    //RigidBodyクラスの行列を自身に適用する
    {
        Matrix M;
        Vector3 PrePos = m_Params.pos;

        m_pRigitBody->Get_TransMatrix(M);

        M = m_BaseMatrix * M;

        m_pMeshRenderer->SetMatrix(M);

        m_Params.pos = Vector3(M._41, M._42, M._43);
        m_Params.move = m_Params.pos - PrePos;

    }


    //寿命管理
    {
        //消滅タイマー
        m_DeleteFrame--;

        //消去時間なら
        if (m_DeleteFrame == 0)
        {
            //フェードアウトして消えるボールクラスを作成する
            iexMesh* pMesh;

            GetBallMesh(m_Params.pParent->m_PlayerInfo.chr_type, &pMesh);

            new BallFadeOutRenderer(
                pMesh,
                m_BaseMatrix,
                m_pRigitBody,
                60
                );

            //自身で開放しないようにnullに設定しておく
            m_pRigitBody = nullptr;
        }
}

    //軌跡の不透明度を徐々に減らしていく
    {
        //軌跡
        m_Locus.m_StartParam.Color.w *= 0.95f;
        m_Locus.m_StartParam.HDRColor.w *= 0.95f;

        //太さが一定以下なら描画しない
        if (m_Locus.m_StartParam.Color.w < 0.01f)
        {
            m_Locus.m_Visible = false;
        }

        if (m_Locus.m_Visible)
        {
            //軌跡の点を追加
            AddLocusPoint();
        }
    }

    //光値を減少
    {
        m_pMeshRenderer->m_HDR *= 0.92f;
    }

    //スケールを0.75倍に…？
    {
        
    }

    //フィールド外,もしくは消滅タイマーが０　なら更新失敗
    return !isOutofField() && m_DeleteFrame > 0;
}