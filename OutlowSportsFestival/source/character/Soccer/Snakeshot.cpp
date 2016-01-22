#include "Snakeshot.h"
#include "../../character/CharacterBase.h"
#include "../../Render/MeshRenderer.h"
#include "Collision/Collision.h"

#include "Camera/Camera.h"

#include "character/CharacterFunction.h"
#include "character/CharacterManager.h"
#include "Ball/BallFadeOutRenderer.h"
#include "../../Effect/SoccerSpecialHitEffect.h"
#include "../../GameSystem/ResourceManager.h"

#include "../../Effect/AnimationBordRenderer.h"

#include "../../Effect/EffectFactory.h"
#include "../../Effect/BlurImpact.h"


Snakeshot::Snakeshot(
    CrVector3 pos,
    CrVector3 vec,
    CharacterBase* pParent,
    RATIO          power
    ) :
    m_pRigidBody(nullptr),
    m_pOriginParent(pParent),
    m_pTornadoEffect(nullptr),
    m_Timer(0),
    m_pTarget(nullptr),
    m_CounterNoDamage(0)
{
	m_pStatefunc = &Snakeshot::State_TargetDecision;

    m_Params.pos = pos;
    m_Params.move = vec * 0.5f;  //スピード
    m_Params.pParent = pParent;
    m_Params.scale = 1.0f;
    m_Params.type = BallBase::Type::_Usual;

    m_Angle = Vector3Zero;


    //メッシュ設定
    {
        iexMesh* pMesh;

        UsualBall::GetBallMesh(CharacterType::_Soccer, &pMesh); 

        m_pMeshRenderer = new MeshRenderer(
            pMesh,
            false,
            MeshRenderer::RenderType::UseColorSpecular
            );

        UpdateMesh();
    }

    //ダメージ設定
    m_Damage.m_Param.size = 1.5f;
    m_Damage.m_VecType = DamageShpere::DamageVecType::PosToCenterXZ;
    m_Damage.m_VecPower = Vector2(0.5f, 0.5f);
    m_Damage.pBall = this;
    m_Damage.pParent = m_Params.pParent;
    m_Damage.Value = 0.0f;
    m_Damage.m_Enable = true;

    //エフェクト
    TornadoEffect::Param param;

    param.highWidth = 0.25f;
    param.middleWidth = 2.5f;
    param.lowWidth = 0.25f;

    param.Length = 2.2f;
    param.LocusWidthStart = 0.35f;
    param.LocusWidthEnd = 0.35f;
    param.middle_height = 10;
    param.right = Vector3AxisX;
    param.vec = Vector3AxisY;
    param.RotateSpeed = 0.4f;

    m_pTornadoEffect = new TornadoEffect(param, 3, 30);


    UpdateEffect();
}

Snakeshot::~Snakeshot()
{
	delete	m_pMeshRenderer;
}

bool Snakeshot::Update()
{
    const bool ret = m_pStatefunc != &Snakeshot::State_Delete;

	(this->*m_pStatefunc)();

    m_Params.pos += m_Params.move;

    UpdateDamageClass();
    UpdateMesh();
    UpdateEffect();

	return ret;
}
bool Snakeshot::Msg(MsgType mt)
{
    if (mt == MsgType::_GameSet &&
        m_pStatefunc == &Snakeshot::State_ToTagetMove || 
        m_pStatefunc == &Snakeshot::State_TargetDecision 
        )
    {
        m_pStatefunc = &Snakeshot::State_NoWork;

        return true;
    }

	return false;
}

//-------------------------------------------------------------------------

void Snakeshot::State_TargetDecision()
{
	//ターゲット選択してState_ToTagetMoveに移行
	//もしターゲットがなければState_NoWorkに移行

	//ターゲット取得
	m_pTarget = CalcTarget();

    if (m_pTarget != nullptr)
    {
        //State_ToTagetMoveに移行
        m_pStatefunc = &Snakeshot::State_ToTagetMove;
    }
}

void Snakeshot::State_ToTagetMove()
{
	//ターゲットに向かって移動し、もしターゲットが死んでいたら再度
	//State_TargetDecisionに移行

    //カウンター直後はダメージ判定なし！
    if (m_Damage.m_Enable == false)
    {
        m_Damage.m_Enable = --m_CounterNoDamage <= 0;
    }
    
	//もしターゲットが死んでいたら再度State_TargetDecisionに移行
	if (chr_func::isDie(m_pTarget))
	{
		m_pStatefunc = &Snakeshot::State_TargetDecision;
	}

	//敵に当たっていたら攻撃判定をなくす
    if (m_Damage.HitCount > 0 || isHitWall())
	{
		//攻撃ステートにいこう
		m_pStatefunc = &Snakeshot::State_Attack;

        //カメラのゆれ
        DefCamera.SetShock(Vector2(0.2f, 0.2f), 30);

        State_Delete();

        m_Timer = 0;

        //エフェクト
        new SoccerSpecialHit(
            m_Params.pParent,
            m_Params.pos,
            1,
            60
            );

        //ブラーエフェクト
        new BlurImpactSphere(
            m_Params.pos,
            15,
            140,
            30
            );
	}

	//ステージ範囲なら消去ステートへ
	if (isOutofField())
	{
		m_pStatefunc = &Snakeshot::State_Delete;
	}

    //パーティクル
    EffectFactory::SmokeParticle(
        m_Params.pos,
        Vector3Rand()*0.01f,
        15,
        0.5f+frand()*0.5f,
        0x20FFFFFF
        );

    m_Angle.y += 0.5f;

	MoveHomingRotate(m_pTarget->m_Params.pos);
}


void Snakeshot::State_NoWork()
{
    m_Params.type = BallBase::Type::_DontWork;

    m_pStatefunc = &Snakeshot::State_Delete;

    //ポトンと落ちる用のボール
    {
        BallBase::Params param = m_Params;

        param.type = BallBase::Type::_DontWork;
        param.pParent = m_pOriginParent;

        new UsualBall(
            param,
            DamageBase::Type::_WeekDamage,
            0,
            UsualBall::GetUsualMoveControll(),
            1,
            1
            );
    }
}


void Snakeshot::State_Delete()
{
    if (m_pTornadoEffect != nullptr)
    {
        m_pTornadoEffect->Destroy();
        m_pTornadoEffect = nullptr;
    }
}

void Snakeshot::State_Attack()
{
    //回転しながら停滞
    m_Params.type = BallBase::Type::_DontWork;
    m_Damage.m_Enable = false;

    m_Params.move = Vector3Zero;
    m_Angle.y += 3.0f*(1 - ((float)m_Timer / (float)50));
    
    if (++m_Timer > 50)
    {
        m_pStatefunc = &Snakeshot::State_NoWork;
    }
}


bool Snakeshot::isOutofField()const
{
	return Vector3Length(m_Params.pos) > 100;
}


void Snakeshot::UpdateDamageClass()
{
	m_Damage.m_Vec = m_Params.move;
	m_Damage.m_Param.pos = m_Params.pos;
}


void Snakeshot::UpdateMesh()
{
	//メッシュのワールド変換行列を更新する
    Matrix m;

    float Ballsize = UsualBall::GetBallScale(CharacterType::_Soccer);

    SetTransformMatrixXYZ(&m, m_Params.pos.x, m_Params.pos.y, m_Params.pos.z, m_Angle.x, m_Angle.y, m_Angle.z);

    {
        m._11 *= Ballsize; m._12 *= Ballsize; m._13 *= Ballsize;
        m._21 *= Ballsize; m._22 *= Ballsize; m._23 *= Ballsize;
        m._31 *= Ballsize; m._32 *= Ballsize; m._33 *= Ballsize;
    }

    m_pMeshRenderer->SetMatrix(m);
}


bool Snakeshot::isHitWall()
{
	Vector3 Out, Pos(m_Params.pos), Vec(m_Params.move);
	float Dist = m_Params.move.Length()*2.0f;
	int Material;

	Vec.Normalize();

	if (DefCollisionMgr.RayPick(
		&Out,
		&Pos,
		&Vec,
		&Dist,
		&Material,
		CollisionManager::RayType::_Usual
		)
		)
	{
		//移動量を反射
		return true;
	}

	return false;
}

void Snakeshot::UpdateEffect()
{
    if (m_pTornadoEffect == nullptr)
    {
        return;
    }

    m_pTornadoEffect->m_Param.pos = m_Params.pos + Vector3(0, m_pTornadoEffect->m_Param.Length*-0.5f, 0);
}

void Snakeshot::Counter(CharacterBase* pCounterCharacter)
{
    //ターゲットが親しかいなかった場合はカウンターしたものを再度標的に
    if (DefCharacterMgr.GetCharacterLiveCount() == 2 &&
        chr_func::isDie(m_pOriginParent) == false
        )
    {   
        m_Damage.pParent = m_Params.pParent = m_pOriginParent;

        //撃ったキャラクタ以外のキャラクタをターゲットにする
        for (auto &it : DefCharacterMgr.GetCharacterMap())
        {
            if (it.first != m_pOriginParent)
            {
                m_pTarget=it.first;
                break;
            }
        }

        //10フレーム間判定なしに
        m_CounterNoDamage = 10;
        m_Damage.m_Enable = false;
    }
    else
    {
        m_Damage.pParent = m_Params.pParent = pCounterCharacter;

        m_pStatefunc = &Snakeshot::State_TargetDecision;
    }
}

void Snakeshot::DeleteEffect()
{
    for (int i = 0; i < 10; ++i)
    {
        EffectFactory::Smoke(
            m_Params.pos + Vector3Rand(),
            Vector3(0, 0.05f, 0),
            2.0f,
            0.1f
            );
    }
}

void Snakeshot::EffectApper(int n, RATIO scale)
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

        r->m_Pos = m_Params.pos;
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

//　ホーミング計算
void Snakeshot::MoveHomingRotate(Vector3 TargetPos)
{
	//引数の位置に向かって移動する(ホーミング)
	const RADIAN HomingRad = D3DXToRadian(4);
	RADIAN rotate;
    Matrix R;
	Vector3 v1, v2;



    Vector3 tpos(m_Params.pos), tmove(m_Params.move);

	v1 = m_Params.move;
	v2 = TargetPos - m_Params.pos;

    v1.y = 0;
    v2.y = 0;

    if (v2 == Vector3Zero || v1 == Vector3Zero)
    {
        return;
    }
    
    rotate = Vector3Radian(v1, v2);

    if (HomingRad > rotate)
    {
        return;
    }

	rotate = min(HomingRad, rotate);

    if (v1.z*v2.x - v1.x*v2.z < 0)
	{
		rotate = -rotate;
	}

    m_Params.move = Vector3RotateAxis(Vector3AxisY, rotate, m_Params.move);
}

//　遠距離ターゲット選定
CharacterBase* Snakeshot::CalcTarget()const
{
	Vector3 v1, v2;

    RADIAN         MostMaxAngle = PI, TempAngle = 0;
	CharacterBase* pTarget = nullptr;
    
	//　map代入
	const CharacterManager::CharacterMap& chr_map =
		DefCharacterMgr.GetCharacterMap();
	
    v1 = m_Params.move;
    v1.y = 0;
    v1.Normalize();

	for (auto it = chr_map.begin(); it != chr_map.end(); ++it)
	{
		//　死んでるorはじめに撃った人or現在のダメージの親ならcontinue
        if (chr_func::isDie(it->first) ||
            it->first == m_pOriginParent ||
            it->first == m_Damage.pParent
            )
		{
			continue;
		}

		//　視野角計算
		v2 = it->first->m_Params.pos - m_Params.pos;
		v2.y = 0;

        if (v2 == Vector3Zero)
        {
            continue;
        }

        TempAngle = Vector3Radian(v1, v2);

		//角度外なら適していない
        if (TempAngle > MostMaxAngle)
		{
			continue;
		}

        MostMaxAngle = TempAngle;
        pTarget = it->first;

	}

	return pTarget;
}
