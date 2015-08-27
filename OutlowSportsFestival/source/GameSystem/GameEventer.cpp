#include "GameEventer.h"
#include "../Camera/CameraState.h"
#include "../character/CharacterManager.h"
#include "../character/CharacterBase.h"
#include "../character/CharacterFunction.h"
#include "../Effect/ParticleMoveObject.h"
#include "../Effect/ParticleRenderer.h"
#include "../GameSystem/ResourceManager.h"
#include "../Collision/Collision.h"
#include "../Effect/FadeGameObject.h"


//----------------------------------------------------
//  �����J�ڃ��b�Z�[�W�𑗐M����N���X
//----------------------------------------------------

GameEventer::GameEventer(const Param& p, State* pInitState) :
m_Param(p)
{
	m_pStateMachine = new StateMachine(this);

	if (pInitState != nullptr)
	{
		m_pStateMachine->set_state(pInitState);
	}
}

GameEventer::~GameEventer()
{
	delete m_pStateMachine;
}

void GameEventer::SetState(State* pState)
{
	m_pStateMachine->set_state(pState);
}

bool GameEventer::Update()
{
	m_pStateMachine->state_execute();
	return true;
}

bool GameEventer::Msg(MsgType mt)
{

	return false;
}


//--------------------------------------------------
//         �����J�n�̃J�E���g�_�E��
//--------------------------------------------------

MatchState::StartCountdown::StartCountdown()
{
	m_Timer = 0;
}

MatchState::StartCountdown::~StartCountdown()
{

}

//�L�����N�^�����ꂼ��ڂ��J������ݒ�

void MatchState::StartCountdown::SetCamera()
{

	const CharacterManager::CharacterMap& ChrMap = DefCharacterMgr.GetCharacterMap();
	CameraState* pSetState, *pNext;

	Vector3 pos, target, front;

	DefCamera.m_Position = pos;
	DefCamera.m_Target = target;

	//�Ō�̃L�����N�^���珇�ԂɃJ�����𐶐�
	for (int i = 3; i >= 0; --i)
	{
		for (auto& it : ChrMap)
		{
			if ((int)it.first->m_PlayerInfo.number != i)
			{
				continue;
			}

			//�J�����ʒu��ݒ�
			pos = target = it.first->m_Params.pos;

			target.y += 3.0f;
			pos.y += 3.5f;

			//�ʒu���L�����N�^�̂�����ƑO����
			chr_func::GetFront(it.first, &front);
			pos += front*8.0f;

			//�Ō�̃J�����������ꍇ�͎����J���������ɐݒ�
			if (i == 3)
			{
				pNext = new CameraStateGamePlay();
			}
			else
			{
				pNext = pSetState;
			}

			pSetState = new CameraStateMovetoPoint(
				pos,
				target,
				0.8f,
				80,
				CameraStateMovetoPoint::CalcType::_Linear,
				pNext
				);
		}
	}

	DefCamera.SetNewState(pSetState);
}

void MatchState::StartCountdown::Enter(_Client_type_ptr p)
{
	//�L�����N�^�����ꂼ��ڂ��J������ݒ�
	SetCamera();

	//�T�b
	m_MaxTime = 60 * 5;

}

void MatchState::StartCountdown::Execute(_Client_type_ptr p)
{
	if (++m_Timer > m_MaxTime)
	{
		p->SetState(new MatchPlay());
	}
}

void MatchState::StartCountdown::Exit(_Client_type_ptr p)
{

}

//--------------------------------------------------
//         �Q���E���h�ڈȍ~�̊J�n�X�e�[�g
//--------------------------------------------------

MatchState::RoundResetCountdown::RoundResetCountdown()
{
    m_Frame = 0;
}

MatchState::RoundResetCountdown::~RoundResetCountdown()
{

}

void MatchState::RoundResetCountdown::Enter(_Client_type_ptr p)
{

}

void MatchState::RoundResetCountdown::Execute(_Client_type_ptr p)
{
    if (++m_Frame > 120)
    {
        DefCamera.SetNewState(new CameraStateGamePlay());
        p->SetState(new MatchPlay());
    }
}

void MatchState::RoundResetCountdown::Exit(_Client_type_ptr p)
{

}




//--------------------------------------------------
//              �������̃X�e�[�g
//--------------------------------------------------

MatchState::MatchPlay::MatchPlay():
m_Frame(0)
{

}

MatchState::MatchPlay::~MatchPlay()
{

}


void MatchState::MatchPlay::GetLiveCharacterMap(CharacterManager::CharacterMap& Out)
{
    Out = DefCharacterMgr.GetCharacterMap();
    CharacterManager::CharacterMap::iterator it = Out.begin();

    while (it != Out.end())
    {
        if (chr_func::isDie(it->first))
        {
            it = Out.erase(it);
            continue;
        }
        ++it;
    }
}

void MatchState::MatchPlay::Enter(_Client_type_ptr p)
{

}

void MatchState::MatchPlay::Execute(_Client_type_ptr p)
{
    const UINT liveCount = DefCharacterMgr.GetCharacterLiveCount();
    
    if (liveCount == 1)
    {
        //��l�����p�^�[���̏ꍇ

        CharacterManager::CharacterMap NowLiveCharacterMap;
        GetLiveCharacterMap(NowLiveCharacterMap);

        //���������L�����N�^������
        LpCharacterBase  WinCharacter = NowLiveCharacterMap.begin()->first;
        LpCharacterBase  LastDieCharacter = nullptr;
        
        //�Ō�ɓ|���ꂽ�L�����N�^������
        for (auto& it : m_CharacterMap)
        {
            if (WinCharacter != it.first)
            {
                LastDieCharacter = it.first;
                break;
            }
        }

        MyAssert(LastDieCharacter != nullptr, "�Ō�ɓ|���ꂽ�L�����N�^��������܂���ł����@�i�v���C�l�����P�̉\������j");

        //��l�����X�e�[�g���Z�b�g
        p->SetState(new WinPose(LastDieCharacter, WinCharacter));

        return;
    }
    else if (liveCount == 0)
    {
        //���������̏ꍇ

        int a = 0;
    }
    else if (++m_Frame > p->m_Param.time)
	{
        //�^�C���A�b�v�̏ꍇ
        LpCharacterBase  WinCharacter = nullptr;
        RATIO            MostManyLife = 0.0f;
        RATIO            Temp;

        m_CharacterMap = DefCharacterMgr.GetCharacterMap();

        for (auto& it : m_CharacterMap)
        {
            Temp = chr_func::GetLifeRatio(it.first);

            if (Temp > MostManyLife)
            {
                MostManyLife = Temp;
                WinCharacter = it.first;
            }
        }

        MyAssert(WinCharacter != nullptr, "�^�C���A�b�v���ɃL�����N�^�����܂���ł���(���肦�Ȃ�!)");

        //�^�C���A�b�v�����X�e�[�g���Z�b�g
     //   p->SetState(new WinPose(LastDieCharacter, WinCharacter));

        return;
	}

    //�������Ă���L�����N�^�f�[�^���X�V����
    GetLiveCharacterMap(m_CharacterMap);
}

void MatchState::MatchPlay::Exit(_Client_type_ptr p)
{

}


//--------------------------------------------------
//              ��l�����X�e�[�g
//--------------------------------------------------

MatchState::WinPose::WinPose(
    LpCharacterBase pLastDieCharacter,
    LpCharacterBase pWinCharacter
    ) :
m_Frame(0),
m_pLastDieCharacter(pLastDieCharacter),
m_pWinCharacter(pWinCharacter)
{

}

MatchState::WinPose::~WinPose()
{

}

void MatchState::WinPose::Enter(_Client_type_ptr p)
{
    DefCamera.SetNewState(new CameraStateCharacterZoom(m_pLastDieCharacter, 0.05f));
}

void MatchState::WinPose::Execute(_Client_type_ptr p)
{
    ++m_Frame;

    if (m_Frame == 120)
    {
        DefCamera.SetNewState(new CameraStateCharacterZoom(m_pWinCharacter, 0.05f));
    }

    if (m_Frame == 300)
    {
        p->SetState(new ResetRound());
    }
}

void MatchState::WinPose::Exit(_Client_type_ptr p)
{

}


//--------------------------------------------------
//            ���E���h���Z�b�g�X�e�[�g
//--------------------------------------------------

void MatchState::ResetRound::Enter(_Client_type_ptr p)
{
    m_Frame = 0;

    //�u���b�N�A�E�g������
    new FadeGameObject(
        COLORf(1, 0, 0, 0),
        40,
        0,
        30
        );
}

void MatchState::ResetRound::Execute(_Client_type_ptr p)
{
    if (++m_Frame == 40)
    {
        DefGameObjMgr.SendMsg(GameObjectBase::MsgType::_RoundReset);
        DefCamera.SetNewState(new CameraStateGamePlay(true));

        p->SetState(new RoundResetCountdown());
    }
}

void MatchState::ResetRound::Exit(_Client_type_ptr p)
{

}

