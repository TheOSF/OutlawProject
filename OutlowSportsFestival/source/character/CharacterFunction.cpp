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

//��{�I�ȍX�V(���W�X�V�A�ǂƂ̔���Ȃ�)�����ׂčs��
void chr_func::UpdateAll(
    CharacterBase* p,
    DamageManager::HitEventBase*	pHitEvent)
{
    //�d�͉��Z
    UpdateMoveY(p);

    //�ʒu���X�V
    PositionUpdate(p);

    //�ǂƂ̐ڐG����
    CheckWall(p);

    //���Ƃ̐ڐG����
    CheckGround(p);

    //�����蔻������
    DamageCheck(p, pHitEvent);

}

//��{�I�ȍX�V(���W�X�V�A�ǂƂ̔���Ȃ�)�����ׂčs��
void chr_func::UpdateAll(
    CharacterBase* p,
    DamageManager::HitEventBase*	pHitEvent,
    RATIO Speed)
{
    //�d�͉��Z
    UpdateMoveY(p, Speed);

    //�ʒu���X�V
    PositionUpdate(p,Speed);

    //�ǂƂ̐ڐG����
    CheckWall(p);

    //���Ƃ̐ڐG����
    CheckGround(p);

    //�����蔻������
    DamageCheck(p, pHitEvent);
}

//���W�Ɉړ��ʂ��X�V����
void chr_func::PositionUpdate(CharacterBase* p, RATIO t)
{
    p->m_Params.pos += p->m_Params.move*t;
}

//�w�y���ňړ��ʂ𑫂�(max_speed�𒴂��Ȃ��悤�ɐݒ肳���)
void chr_func::AddXZMove(CharacterBase*p, float x, float z, float max_speed)
{
	p->m_Params.move.x += x;
	p->m_Params.move.z += z;

	SetMaxXZspeed(p, max_speed);
}

//�O�����Ɉړ��ʂ��^�X
void chr_func::AddMoveFront(CharacterBase*p, float speed, float max_speed)
{
	p->m_Params.move.x += sinf(p->m_Params.angle)*speed;
	p->m_Params.move.z += cosf(p->m_Params.angle)*speed;

	SetMaxXZspeed(p, max_speed);
}

//���X�Ɍ�������(t���P�ɂ���ƈ�u�Ŏ~�܂�,0�Ō��ʂȂ�)
void chr_func::XZMoveDown(CharacterBase*p, float t)
{
	t = min(t, 1);
	t = max(t, 0);
	t = 1 - t;

	p->m_Params.move.x *= t;
	p->m_Params.move.z *= t;
}

//�w�y�����̈ړ��ʂ�max_speed�𒴂��Ă�����Amax_speed�ɐݒ肷��
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


//�w�肵���ꏊ�Ɍ���
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

//�w�肵���ꏊ�Ɍ���(��u��)
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

//�w�肵���L�����N�^�Ɍ���(��u��)
void chr_func::AngleControll(CharacterBase* p, const CharacterBase* target)
{
    AngleControll(p, target->m_Params.pos);
}

//���݂̈ʒu�A���݂�angle�AScale�����Ƃɕϊ��s��𐶐�����
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

//�O�����x�N�g���𓾂�
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

//�E�����x�N�g���𓾂�
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

//�_���[�W������Ƃ�
void chr_func::DamageCheck(
	LpCharacterBase					pCharacter,	//������Ƃ�L�����N�^
	DamageManager::HitEventBase*	pHitEvent	//�C�x���g����
	)
{
	//�����蔻����Ƃ�
    SphereParam sp;

	sp.pos = pCharacter->m_Params.pos;
	sp.pos.y += BallBase::UsualBallShotY;
	sp.size = pCharacter->m_Params.hitScale;

	DefDamageMgr.HitCheckSphere(sp, *pHitEvent);
}

//�n�ʔ�����Ƃ�(�߂�l�F�n�ʂɂ��Ă��邩�ǂ���)
bool chr_func::isTouchGround(CharacterBase* p)
{
    return 
        p->m_Params.pos.y <= CharacterBase::m_CommonParams.GroundY + 0.1f &&
        p->m_Params.move.y <= 0.0f;
}


//Y�������̈ړ��A���x���X�V����
void chr_func::UpdateMoveY(CharacterBase* p, RATIO t)
{
    p->m_Params.move.y += CharacterBase::m_CommonParams.Glavity*t;
}


//Y�������̒n�ʔ�����s��
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
        pVec->Normalize();	//���ׂ̖@��
        
        *pDist = client_size - *pDist;//�������钷��

        *pPos = *pVec**pDist;	//�␳�x�N�g��

        *pDist = pPos->Length();	//�␳����

        *pVec = *pPos;
        pVec->Normalize();	//�␳�x�N�g�����K��

        TempDist = *pDist;//�␳����temp

        if (!GetWall(*pPos, *pVec, *pDist) ||
            TempDist < *pDist)
        {
            *pOutChecked += *pPos;
        }
        return true;
    }

    return false;
}


//XZ�����̕ǂƂ̐ڐG������s��(�߂�l���ǂƂ������Ă��邩�ǂ���)
bool chr_func::CheckWall(CharacterBase* p)
{
    //�߂�l
    bool isHit = false;

    //���C���΂���x�N�g��
    Vector3 CheckVec;

    //�ړ����Ă���ꍇ�͂��̕������`�F�b�N�A�Ⴄ�ꍇ�͑O�������`�F�b�N
    if (Vector3XZLength(p->m_Params.move) > 0)
    {
        CheckVec = Vector3Normalize(Vector3(p->m_Params.move.x, 0, p->m_Params.move.z));
    }
    else
    {
        GetFront(p, &CheckVec);
    }

    //�����̉�]�p�x
    float RotateCheckVec[]=
    {
        0,
        PI / 3,
        -PI / 3,

        PI / 2, 
        -PI / 2,

        PI
    };

    //�v�Z�p�p�����[�^
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
//XZ�����̕ǂƂ̐ڐG������s��(���C�s�b�N1�{���ʒu�����Ȃ�ver)
bool chr_func::IsHitWall(CharacterBase* p)
{
	//�߂�l
	bool isHit = false;

	//���C���΂���x�N�g��
	Vector3 CheckVec;

	//�ړ����Ă���ꍇ�͂��̕������`�F�b�N�A�Ⴄ�ꍇ�͑O�������`�F�b�N
	if (Vector3XZLength(p->m_Params.move) > 0)
	{
		CheckVec = Vector3Normalize(Vector3(p->m_Params.move.x, 0, p->m_Params.move.z));
	}
	else
	{
		GetFront(p, &CheckVec);
	}

	//�����̉�]�p�x
	float RotateCheckVec[] =
	{
		0,
		PI / 3,
		-PI / 3,
	};

	//�v�Z�p�p�����[�^
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


//���݂̗̑͂̊����𓾂�i�O�`�P�j
RATIO chr_func::GetLifeRatio(CharacterBase* p)
{
    return p->m_Params.HP / p->m_Params.maxHP;
}


//�ړ��ʂ��[���ɂ���
void chr_func::ResetMove(CharacterBase* p)
{
    p->m_Params.move = Vector3Zero;
}

//�����̏ꏊ�������̃L�����N�^����݂ĉE���ǂ���
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

//�����̏ꏊ�������̃L�����N�^����݂đO���ǂ���
bool chr_func::isFront(CharacterBase* p, CrVector3 pos)
{
    Vector3 v1 = pos - p->m_Params.pos, v2;
    GetFront(p, &v2);

    return Vector3Dot(v1, v2) > 0.0f;
}


//�����̃L�����N�^�̃Q�[�W�����Z����
void chr_func::AddSkillGauge(CharacterBase* p, RATIO value)
{
    const float pre_value = p->m_Params.SP;

    p->m_Params.SP += value;
    p->m_Params.SP = fClamp(p->m_Params.SP, 1.0f, 0.0f);

    //(�K�E�Z���łĂ�悤�ɂȂ�����)
    if (isCanSpecialAttack(pre_value) == false &&
        isCanSpecialAttack(p->m_Params.SP))
    {
        //�G�t�F�N�g
        EffectFactory::CircleAnimationBillbord(
            p->m_Params.pos + Vector3(0, 3, 0),
            Vector3Zero,
            Vector3Zero,
            Vector2(10, 10),
            0xFFFFFFFF,
            RS_ADD
            );

        //���ʉ�
      //  Sound::Play(Sound::Scene_Enter);
    }
}


//�����̃L�����N�^�̃Q�[�W�����Z�b�g����(�O�ɂ���)
void chr_func::ResetSkillGauge(CharacterBase* p)
{
    p->m_Params.SP = 0;
}

//�����̃L�����N�^���K�E�Z�𔭓��ł��邩�ǂ���
bool chr_func::isCanSpecialAttack(CharacterBase* p)
{
    return isCanSpecialAttack(p->m_Params.SP);
}

//�����̃L�����N�^�̂r�o���O���ǂ���
bool chr_func::isSkillZero(CharacterBase* p)
{
    return p->m_Params.SP <= 0.0f;;
}

//�L�����N�^�̗̑͂��_���[�W�ɂ���Č���������
void chr_func::CalcDamage(CharacterBase* p, float value, bool DontDie)
{
    //�퓬���ȊOor���łɎ���ł���Ȃ�v�Z���Ȃ�
    if (p->GetStateType() != CharacterBase::State::Usual || 
        isDie(p))
    {
        return;
    }

    p->m_Params.HP -= value;

    //�݂˂����Ȃ�P�c��
    if (DontDie)
    {
        p->m_Params.HP = max(p->m_Params.HP, 1);
    }
    else
    {
        p->m_Params.HP = max(p->m_Params.HP, 0);
    }

}

//�U���̃^�[�Q�b�g�𓾂�
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

//�U���̃^�[�Q�b�g�𓾂�(�����̃^�[�Q�b�g�������ꍇ�A���g�̂����Ƃ��O�ɂ���L�����N�^��I��)
bool chr_func::CalcAtkTarget(
    CharacterBase* pOwner, 
    CrVector3 OwnerPos, 
    CrVector3 OwnerViewVec,
    RADIAN check_angle,
    float check_distance,
    CharacterBase** ppOut)
{
    //�^�[�Q�b�g�I�聕�����␳
    CharacterManager::CharacterMap ChrMap = DefCharacterMgr.GetCharacterMap();

    RADIAN MostMinAngle = check_angle;   //�����Ƃ������p�x
    RADIAN TempAngle;

    Vector3 toEnemy;
    Vector3 MyFront = OwnerViewVec;

    auto it = ChrMap.begin();

    //�߂�l��������
    *ppOut = nullptr;


    while (it != ChrMap.end())
    {
        //���g�Ǝ���ł���L���������O
        if (pOwner->m_PlayerInfo.number == it->first->m_PlayerInfo.number ||
            chr_func::isDie(it->first)
            )
        {
            ++it;
            continue;
        }

        //�G�ւ̃x�N�g��
        toEnemy = it->first->m_Params.pos - OwnerPos;

        //�����O�Ȃ�continue
        if (toEnemy.Length() > check_distance)
        {
            ++it;
            continue;
        }

        //�O�x�N�g���ƓG�ւ̃x�N�g���̊p�x���v�Z����
        TempAngle = Vector3Radian(MyFront, toEnemy);

        //�p�x����ԋ���������X�V
        if (TempAngle < MostMinAngle)
        {
            *ppOut = it->first;
            MostMinAngle = TempAngle;
        }

        ++it;
    }

    return *ppOut != nullptr;
}

//�L�����N�^�̃��E���h�����������Z����
void chr_func::AddWinPoint(CharacterBase* p)
{
    p->m_Params.win++;
}

//�����̃X�L���l���K�E�Z�𔭓��ł��邩�ǂ���
bool chr_func::isCanSpecialAttack(RATIO value)
{
    return value >= 0.99f;
}


//�R���g���[���̐U�����Z�b�g
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
