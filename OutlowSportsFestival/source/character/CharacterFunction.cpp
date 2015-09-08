#include "CharacterFunction.h"
#include "CharacterBase.h"
#include "../Ball/Ball.h"
#include "../Collision/Collision.h"

//座標に移動量を更新する
void chr_func::PositionUpdate(CharacterBase* p)
{
	p->m_Params.pos += p->m_Params.move;
}

//ＸＺ軸で移動量を足す(max_speedを超えないように設定される)
void chr_func::AddXZMove(CharacterBase*p, float x, float z, float max_speed)
{
	p->m_Params.move.x += x;
	p->m_Params.move.z += z;

	SetMaxXZspeed(p, max_speed);
}

//前方向に移動量をタス
void chr_func::AddMoveFront(CharacterBase*p, float speed, float max_speed)
{
	p->m_Params.move.x += sinf(p->m_Params.angle)*speed;
	p->m_Params.move.z += cosf(p->m_Params.angle)*speed;

	SetMaxXZspeed(p, max_speed);
}

//徐々に減速する(tを１にすると一瞬で止まる,0で効果なし)
void chr_func::XZMoveDown(CharacterBase*p, float t)
{
	t = min(t, 1);
	t = max(t, 0);
	t = 1 - t;

	p->m_Params.move.x *= t;
	p->m_Params.move.z *= t;
}

//ＸＺ方向の移動量がmax_speedを超えていたら、max_speedに設定する
void chr_func::SetMaxXZspeed(CharacterBase*p, float max_speed)
{
	const float l = sqrtf(p->m_Params.move.x*p->m_Params.move.x + p->m_Params.move.z*p->m_Params.move.z);

	if (l > max_speed)
	{
		p->m_Params.move.x /= l;
		p->m_Params.move.x *= max_speed;

		p->m_Params.move.z /= l;
		p->m_Params.move.z *= max_speed;
	}
}


//指定した場所に向く
void chr_func::AngleControll(CharacterBase* p, CrVector3 view_pos, float speed)
{
	Vector3 mepos = p->m_Params.pos;

	if (mepos.x == view_pos.x&&mepos.z == view_pos.z) return;

	float cos0, angle = p->m_Params.angle;

	Vector3 v1(sinf(angle), 0, cosf(angle)), v2(view_pos - mepos);

	v2.y = 0;

	float VectorLength = v1.Length()*v2.Length();

	if (VectorLength == 0.0f)
	{
		return;
	}

	cos0 = Vector3Dot(v1, v2) / VectorLength;
	cos0 = 1.0f - cos0;

	if (cos0>speed)cos0 = speed;

	if (v1.z*v2.x - v1.x*v2.z < 0)
	{
		angle -= cos0;
	}
	else{
		angle += cos0;
	}

	p->m_Params.angle = angle;
}

//指定した場所に向く(一瞬で)
void chr_func::AngleControll(CharacterBase* p, CrVector3 view_pos)
{
	Vector3 mepos = p->m_Params.pos;

	if (mepos.x == view_pos.x&&mepos.z == view_pos.z) return;

	mepos = view_pos - mepos;
	mepos.y = 0;
	mepos.Normalize();

	p->m_Params.angle = acosf(mepos.z);

	if (mepos.x < 0)
	{
		p->m_Params.angle = -p->m_Params.angle;
	}
}

//現在の位置、現在のangle、Scaleをもとに変換行列を生成する
void chr_func::CreateTransMatrix(CharacterBase* p, float Scale, Matrix* pOutMatrix)
{
	Matrix m;

	D3DXMatrixScaling(pOutMatrix, Scale, Scale, Scale);
	D3DXMatrixRotationY(&m, p->m_Params.angle);

	*pOutMatrix *= m;

    pOutMatrix->_41 = p->m_Params.pos.x;
    pOutMatrix->_42 = p->m_Params.pos.y;
    pOutMatrix->_43 = p->m_Params.pos.z;

}

//前方向ベクトルを得る
void chr_func::GetFront(CharacterBase* p, Vector3* pOut)
{
	pOut->x = sinf(p->m_Params.angle);
	pOut->y = 0;
	pOut->z = cosf(p->m_Params.angle);
}

bool chr_func::isDie(CharacterBase* p)
{
	return p->m_Params.HP <= 0;
}

//ダメージ判定をとる
void chr_func::DamageCheck(
	LpCharacterBase					pCharacter,	//判定をとるキャラクタ
	DamageManager::HitEventBase*	pHitEvent	//イベント処理
	)
{
	//あたり判定をとる
	ShpereParam sp;

	sp.pos = pCharacter->m_Params.pos;
	sp.pos.y += BallBase::UsualBallShotY;
	sp.size = pCharacter->m_Params.hitScale;

	DefDamageMgr.HitCheckSphere(sp, *pHitEvent);
}

//地面判定をとる(戻り値：地面についているかどうか)
bool chr_func::isTouchGround(CharacterBase* p)
{
    return p->m_Params.pos.y <= CharacterBase::m_CommonParams.GroundY;
}


//Y軸方向の移動、速度を更新する
void chr_func::UpdateMoveY(CharacterBase* p)
{
    p->m_Params.move.y += CharacterBase::m_CommonParams.Glavity;
}


//Y軸方向の地面判定を行う
void chr_func::CheckGround(CharacterBase* p)
{
    if (isTouchGround(p))
    {
        p->m_Params.pos.y = CharacterBase::m_CommonParams.GroundY;
        p->m_Params.move.y = 0;
    }
}


//XZ方向の壁との接触判定を行う(戻り値＝壁とあたっているかどうか)
bool chr_func::CheckWall(CharacterBase* p)
{
    //戻り値
    bool isHit = false;

    //レイを飛ばす基準ベクトル
    Vector3 CheckVec;

    //移動している場合はその方向をチェック、違う場合は前方向をチェック
    if (p->m_Params.move.Length() > 0)
    {
        CheckVec = Vector3Normalize(Vector3(p->m_Params.move.x, 0, p->m_Params.move.z));
    }
    else
    {
        GetFront(p, &CheckVec);
    }

    //基準からの回転角度
    float RotateCheckVec[]=
    {
        PI / 4,
        -PI / 4,
    };

    //計算用パラメータ
    Vector3 out, pos, vec;
    float dist;
    int material;

    const float Character_size = 2.0f;

    for (int i = 0; i < (int)ARRAYSIZE(RotateCheckVec); ++i)
    {
        pos = p->m_Params.pos;
        pos.y += 1.0f;
        vec = Vector3RotateAxis(Vector3AxisY, RotateCheckVec[i], CheckVec);
        dist = 100;

        if (DefCollisionMgr.RayPick(
            &out,
            &pos,
            &vec,
            &dist,
            &material,
            CollisionManager::RayType::_Usual
            ))
        {
            vec.y = 0;
            vec.Normalize();

            pos = p->m_Params.pos - out;
            pos.y = 0;

            pos = p->m_Params.pos - vec * Vector3Dot(vec, pos);
            pos -= p->m_Params.pos;

            dist = pos.Length();

            if (dist < Character_size)
            {
                p->m_Params.pos += vec * (Character_size - dist);
                isHit = true;
            }
        }
    }

    return isHit;
}


//現在の体力の割合を得る（０～１）
RATIO chr_func::GetLifeRatio(CharacterBase* p)
{
    return p->m_Params.HP / p->m_Params.maxHP;
}
