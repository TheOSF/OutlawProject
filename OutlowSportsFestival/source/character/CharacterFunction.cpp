#include "CharacterFunction.h"
#include "CharacterBase.h"
#include "../Ball/Ball.h"
#include "../Collision/Collision.h"
#include "../Effect/ParticleRenderer.h"
#include "../Effect/ParticleMoveObject.h"
#include "../GameSystem/ResourceManager.h"
#include "../Sound/Sound.h"
#include "../Effect/EffectFactory.h"
#include "CharacterManager.h"
#include "../GameSystem/GameController.h"

//基本的な更新(座標更新、壁との判定など)をすべて行う
void chr_func::UpdateAll(
    CharacterBase* p,
    DamageManager::HitEventBase*	pHitEvent)
{
    //重力加算
    UpdateMoveY(p);

    //位置を更新
    PositionUpdate(p);

    //壁との接触判定
    CheckWall(p);

    //床との接触判定
    CheckGround(p);

    //あたり判定を取る
    DamageCheck(p, pHitEvent);

}

//基本的な更新(座標更新、壁との判定など)をすべて行う
void chr_func::UpdateAll(
    CharacterBase* p,
    DamageManager::HitEventBase*	pHitEvent,
    RATIO Speed)
{
    //重力加算
    UpdateMoveY(p, Speed);

    //位置を更新
    PositionUpdate(p,Speed);

    //壁との接触判定
    CheckWall(p);

    //床との接触判定
    CheckGround(p);

    //あたり判定を取る
    DamageCheck(p, pHitEvent);
}

//座標に移動量を更新する
void chr_func::PositionUpdate(CharacterBase* p, RATIO t)
{
    p->m_Params.pos += p->m_Params.move*t;
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
    const float l = Vector3XZLength(p->m_Params.move);

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

    p->m_Params.angle = acosf(fClamp(mepos.z, 1, -1));

	if (mepos.x < 0)
	{
		p->m_Params.angle = -p->m_Params.angle;
	}
}

//指定したキャラクタに向く(一瞬で)
void chr_func::AngleControll(CharacterBase* p, const CharacterBase* target)
{
    AngleControll(p, target->m_Params.pos);
}

//現在の位置、現在のangle、Scaleをもとに変換行列を生成する
void chr_func::CreateTransMatrix(CharacterBase* p, Matrix* pOutMatrix)
{
	Matrix m;
    const float Scale = p->m_ModelSize;

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

Vector3 chr_func::GetFront(CharacterBase* p)
{
    Vector3 ret;
    GetFront(p, &ret);
    return ret;
}

//右方向ベクトルを得る
void chr_func::GetRight(CharacterBase* p, Vector3* pOut)
{
    const float angle = p->m_Params.angle + PI*0.5f;
    pOut->x = sinf(angle);
    pOut->y = 0;
    pOut->z = cosf(angle);
}

Vector3 chr_func::GetRight(CharacterBase* p)
{
    Vector3 ret;
    GetRight(p, &ret);
    return ret;
}

bool chr_func::isDie(const CharacterBase* p)
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
    SphereParam sp;

	sp.pos = pCharacter->m_Params.pos;
	sp.pos.y += BallBase::UsualBallShotY;
	sp.size = pCharacter->m_Params.hitScale;

	DefDamageMgr.HitCheckSphere(sp, *pHitEvent);
}

//地面判定をとる(戻り値：地面についているかどうか)
bool chr_func::isTouchGround(CharacterBase* p)
{
    return 
        p->m_Params.pos.y <= CharacterBase::m_CommonParams.GroundY + 0.1f &&
        p->m_Params.move.y <= 0.0f;
}


//Y軸方向の移動、速度を更新する
void chr_func::UpdateMoveY(CharacterBase* p, RATIO t)
{
    p->m_Params.move.y += CharacterBase::m_CommonParams.Glavity*t;
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


static bool GetWall(const Vector3& pos, Vector3& v, float& outLen)
{
    Vector3 out;
    outLen = 10000.0f;
    Vector3 p = pos;
    int m;

    if (DefCollisionMgr.RayPick(&out, &p, &v, &outLen, &m, CollisionManager::RayType::_Usual))
    {
        out -= pos;
        outLen = out.Length();

        return true;
    }

    return false;
}


static bool HitCheckWall(
    LPVECTOR3 pOutChecked,
    LPVECTOR3 pOut,
    LPVECTOR3 pPos,
    LPVECTOR3 pVec,
    float* pDist,
    float client_size)
{
    float TempDist = 0;
    *pOutChecked = *pPos;

    if (GetWall(*pPos, *pVec, *pDist) &&
        *pDist<client_size)
    {
        pVec->Normalize();	//かべの法線
        
        *pDist = client_size - *pDist;//調整する長さ

        *pPos = *pVec**pDist;	//補正ベクトル

        *pDist = pPos->Length();	//補正距離

        *pVec = *pPos;
        pVec->Normalize();	//補正ベクトル正規化

        TempDist = *pDist;//補正距離temp

        if (!GetWall(*pPos, *pVec, *pDist) ||
            TempDist < *pDist)
        {
            *pOutChecked += *pPos;
        }
        return true;
    }

    return false;
}


//XZ方向の壁との接触判定を行う(戻り値＝壁とあたっているかどうか)
bool chr_func::CheckWall(CharacterBase* p)
{
    //戻り値
    bool isHit = false;

    //レイを飛ばす基準ベクトル
    Vector3 CheckVec;

    //移動している場合はその方向をチェック、違う場合は前方向をチェック
    if (Vector3XZLength(p->m_Params.move) > 0)
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
        0,
        PI / 3,
        -PI / 3,

        PI / 2, 
        -PI / 2,

        PI
    };

    //計算用パラメータ
    Vector3 out, pos, vec;
    float dist;
    int material;

    const float Character_size = 2.0f;
    Vector3 calc_out = p->m_Params.pos;

    for (int i = 0; i < (int)ARRAYSIZE(RotateCheckVec); ++i)
    {
        pos = p->m_Params.pos;
        pos.y = 2.0f;
        vec = Vector3RotateAxis(Vector3AxisY, RotateCheckVec[i], CheckVec);
        dist = 100;

        //if (HitCheckWall(
        //    &calc_out,
        //    &out,
        //    &pos,
        //    &vec,
        //    &dist,
        //    Character_size))
        //{
        //    isHit = true;
        //}

        if (DefCollisionMgr.RayPick(
            &out,
            &pos,
            &vec,
            &dist,
            &material,
            CollisionManager::RayType::_Character
            ))
        {
            vec.y = 0;
            vec.Normalize();


            //pos = p->m_Params.pos - vec * Vector3Dot(vec, pos);
            //pos -= p->m_Params.pos;

            //dist = pos.Length();

            dist = Vector3Dot(vec, p->m_Params.pos - out);

            if (dist < Character_size)
            {
                p->m_Params.pos += vec * (Character_size - dist);
                isHit = true;
            }
           
        }
    }

    /*if (isHit)
    {
        p->m_Params.pos.x = calc_out.x;
        p->m_Params.pos.z = calc_out.z;
    }*/

    return isHit;
}
//XZ方向の壁との接触判定を行う(レイピック1本＆位置調整なしver)
bool chr_func::IsHitWall(CharacterBase* p)
{
	//戻り値
	bool isHit = false;

	//レイを飛ばす基準ベクトル
	Vector3 CheckVec;

	//移動している場合はその方向をチェック、違う場合は前方向をチェック
	if (Vector3XZLength(p->m_Params.move) > 0)
	{
		CheckVec = Vector3Normalize(Vector3(p->m_Params.move.x, 0, p->m_Params.move.z));
	}
	else
	{
		GetFront(p, &CheckVec);
	}

	//基準からの回転角度
	float RotateCheckVec[] =
	{
		0,
		PI / 3,
		-PI / 3,
	};

	//計算用パラメータ
	Vector3 out, pos, vec;
	float dist;
	int material;

	const float Character_size = 2.0f;
	Vector3 calc_out = p->m_Params.pos;
	for (int i = 0; i < (int)ARRAYSIZE(RotateCheckVec); ++i)
	{
		pos = p->m_Params.pos;
		pos.y = 2.0f;
		vec = Vector3RotateAxis(Vector3AxisY, RotateCheckVec[i], CheckVec);
		dist = 100;

		pos = p->m_Params.pos;
		pos.y = 2.0f;
		vec = Vector3RotateAxis(Vector3AxisY, RotateCheckVec[i], CheckVec);
		dist = 100;


		if (DefCollisionMgr.RayPick(
			&out,
			&pos,
			&vec,
			&dist,
			&material,
			CollisionManager::RayType::_Character
			))
		{
			vec.y = 0;
			vec.Normalize();
			dist = Vector3Dot(vec, p->m_Params.pos - out);

			if (dist < Character_size)
			{
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


//移動量をゼロにする
void chr_func::ResetMove(CharacterBase* p)
{
    p->m_Params.move = Vector3Zero;
}

//引数の場所が引数のキャラクタからみて右かどうか
bool chr_func::isRight(CharacterBase* p, CrVector3 pos)
{
    return isRight(p->m_Params.pos,GetFront(p),pos);
}

bool chr_func::isRight(CrVector3 from, CrVector3 view_vec, CrVector3 judge_target)
{
    Vector3 v1, v2;

    v1 = judge_target - from;
    Vector3Cross(v2, Vector3AxisY, view_vec);

    return Vector3Dot(v1, v2) > 0.0f;
}

//引数の場所が引数のキャラクタからみて前かどうか
bool chr_func::isFront(CharacterBase* p, CrVector3 pos)
{
    Vector3 v1 = pos - p->m_Params.pos, v2;
    GetFront(p, &v2);

    return Vector3Dot(v1, v2) > 0.0f;
}


//引数のキャラクタのゲージを加算する
void chr_func::AddSkillGauge(CharacterBase* p, RATIO value)
{
    const float pre_value = p->m_Params.SP;

    p->m_Params.SP += value;
    p->m_Params.SP = fClamp(p->m_Params.SP, 1.0f, 0.0f);

    //(必殺技が打てるようになったら)
    if (isCanSpecialAttack(pre_value) == false &&
        isCanSpecialAttack(p->m_Params.SP))
    {
        //エフェクト
        EffectFactory::CircleAnimationBillbord(
            p->m_Params.pos + Vector3(0, 3, 0),
            Vector3Zero,
            Vector3Zero,
            Vector2(10, 10),
            0xFFFFFFFF,
            RS_ADD
            );

        //効果音
      //  Sound::Play(Sound::Scene_Enter);
    }
}


//引数のキャラクタのゲージをリセットする(０にする)
void chr_func::ResetSkillGauge(CharacterBase* p)
{
    p->m_Params.SP = 0;
}

//引数のキャラクタが必殺技を発動できるかどうか
bool chr_func::isCanSpecialAttack(CharacterBase* p)
{
    return isCanSpecialAttack(p->m_Params.SP);
}

//引数のキャラクタのＳＰが０かどうか
bool chr_func::isSkillZero(CharacterBase* p)
{
    return p->m_Params.SP <= 0.0f;;
}

//キャラクタの体力をダメージによって減少させる
void chr_func::CalcDamage(CharacterBase* p, float value, bool DontDie)
{
    //戦闘中以外orすでに死んでいるなら計算しない
    if (p->GetStateType() != CharacterBase::State::Usual || 
        isDie(p))
    {
        return;
    }

    p->m_Params.HP -= value;

    //みねうちなら１残す
    if (DontDie)
    {
        p->m_Params.HP = max(p->m_Params.HP, 1);
    }
    else
    {
        p->m_Params.HP = max(p->m_Params.HP, 0);
    }

}

//攻撃のターゲットを得る
bool chr_func::CalcAtkTarget(
    CharacterBase*  pOwner,
    RADIAN          check_angle,
    float           check_distance,
    CharacterBase** ppOut)
{
    return CalcAtkTarget(
        pOwner,
        pOwner->m_Params.pos,
        GetFront(pOwner),
        check_angle,
        check_distance,
        ppOut
        );
}

//攻撃のターゲットを得る(複数のターゲットがいた場合、自身のもっとも前にいるキャラクタを選ぶ)
bool chr_func::CalcAtkTarget(
    CharacterBase* pOwner, 
    CrVector3 OwnerPos, 
    CrVector3 OwnerViewVec,
    RADIAN check_angle,
    float check_distance,
    CharacterBase** ppOut)
{
    //ターゲット選定＆向き補正
    CharacterManager::CharacterMap ChrMap = DefCharacterMgr.GetCharacterMap();

    RADIAN MostMinAngle = check_angle;   //もっとも狭い角度
    RADIAN TempAngle;

    Vector3 toEnemy;
    Vector3 MyFront = OwnerViewVec;

    auto it = ChrMap.begin();

    //戻り値を初期化
    *ppOut = nullptr;


    while (it != ChrMap.end())
    {
        //自身と死んでいるキャラを除外
        if (pOwner->m_PlayerInfo.number == it->first->m_PlayerInfo.number ||
            chr_func::isDie(it->first)
            )
        {
            ++it;
            continue;
        }

        //敵へのベクトル
        toEnemy = it->first->m_Params.pos - OwnerPos;

        //距離外ならcontinue
        if (toEnemy.Length() > check_distance)
        {
            ++it;
            continue;
        }

        //前ベクトルと敵へのベクトルの角度を計算する
        TempAngle = Vector3Radian(MyFront, toEnemy);

        //角度が一番狭かったら更新
        if (TempAngle < MostMinAngle)
        {
            *ppOut = it->first;
            MostMinAngle = TempAngle;
        }

        ++it;
    }

    return *ppOut != nullptr;
}

//キャラクタのラウンド勝利数を加算する
void chr_func::AddWinPoint(CharacterBase* p)
{
    p->m_Params.win++;
}

//引数のスキル値が必殺技を発動できるかどうか
bool chr_func::isCanSpecialAttack(RATIO value)
{
    return value >= 0.99f;
}


//コントローラの振動をセット
void chr_func::SetControllerShock(CharacterBase* p, RATIO power, float second)
{
    if (p->m_PlayerInfo.player_type == PlayerType::_Computer)
    {
        return;
    }

    controller::SetVibration(
        (DWORD)(power * 10000),
        second,
        p->m_PlayerInfo.number
        );
}
