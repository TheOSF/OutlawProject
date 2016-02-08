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
#include "../Effect/BlurImpact.h"
#include "../Stage/PhysicsMeshObject.h"

#include "../UI/RoundUI.h"
#include "../UI/FightUI.h"
#include "../UI/GameSetUI.h"

#include "../Sound/Sound.h"
#include "MatchLightManager.h"
#include "MatchManager.h"
#include "../UI/PlayerCursorBillbord.h"


//----------------------------------------------------
//  �����J�ڃ��b�Z�[�W�𑗐M����N���X
//----------------------------------------------------

GameEventer::GameEventer(const Param& p, State* pInitState) :
m_Param(p)
{
    m_Param.round_count = 1;

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

void GameEventer::SetLightChange()
{
    DefMatchLightManager.LightChangeAndBack(
        3,
        30,
        10,
        Vector3(0.6f, 0.6f, 0.6f)
        );
}

void GameEventer::ResultCheck(CharacterBase* pNowWinCharacter)
{
    for (auto& it : DefCharacterMgr.GetCharacterMap())
    {
        UINT Point = it.first->m_Params.win;

        if (it.first == pNowWinCharacter)
        {
            ++Point;
        }

        if (Point >= m_Param.round)
        {
            ++it.first->m_Params.win;
            DefMatchManager.SceneChange(MatchManager::NextSceneType::GoResult);
            break;
        }
    }
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
    CameraState* pSetState;

    struct CameraPoint
    {
        Vector3 pos, target;
        float speed;
    };

    std::list<CameraPoint> CposList;

    CposList.push_front({ Vector3(18.710548f, 13.073265f, -40.158432f), Vector3(18.517427f, 12.809369f, -39.213413f),0.2f, });
    CposList.push_front({ Vector3(-7.837331f, 13.073265f, -44.821030f), Vector3(-7.887062f, 12.809370f, -43.857761f),0.2f });

    pSetState = new CameraStateGamePlay(true);

    for (auto& it : CposList)
    {
        pSetState =
        new CameraStateMovetoPoint(
            it.pos,
            it.target,
            it.speed,
            300,
            CameraStateMovetoPoint::CalcType::_Linear,
            pSetState
            );
    }

    //CameraStateGamePlay
    //CameraStateMovetoPoint

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


//------------------------------------------------------------
//       �v���C���[����̃L�����N�^�ɖ����o���X�e�[�g
//------------------------------------------------------------

bool MatchState::PlayerCharacterDrawCursor::GetEffectChr(CharacterBase** ppOut, UINT ChrNum)
{
    UINT CalcCount = 0;

    *ppOut = nullptr;

    //�v���C������@���@�O�����ł����Ƃ��߂��L�����N�^
    for (auto &it : DefCharacterMgr.GetCharacterMap())
    {
        if (ChrNum == (UINT)it.first->m_PlayerInfo.number)
        {
            *ppOut = it.first;
            break;

            //if (it.first->m_PlayerInfo.player_type == PlayerType::_Player)
            //{
            //    *ppOut = it.first;
            //    break;
            //}
            //else
            //{
            //    break;
            //}
        }
        
    }
    
    return *ppOut != nullptr;
}

void MatchState::PlayerCharacterDrawCursor::Effect(CharacterBase* p)
{
    //�J�[�\���t�h
    PlayerCursorBillbord* UI = new PlayerCursorBillbord(p);;
    UI->LightUp(40);

    //�U��
    chr_func::SetControllerShock(p, 0.6f, 0.5f);

}

void MatchState::PlayerCharacterDrawCursor::Enter(_Client_type_ptr p)
{
    m_Frame = 60;
    m_ChrNum = 0;
    m_PrePlayerNum = PlayerNum::Value::_ErrorType;
}

void MatchState::PlayerCharacterDrawCursor::Execute(_Client_type_ptr p)
{
    CharacterBase* pEffectChr = nullptr;

    //�J�E���g���Ȃ珈�����Ȃ�
    if (m_Frame > 0)
    {
        --m_Frame;
        return;
    }

    //�J�E���g�Z�b�g
    m_Frame = 60;

    //�G�t�F�N�g���o���L�����N�^�𓾂�
    if (GetEffectChr(&pEffectChr, m_ChrNum))
    {
        //�L�����N�^�ɃZ�b�g
        Effect(pEffectChr);

        //�L�����N�^�J�E���g���Z
        ++m_ChrNum;
    }
    else
    {
        //�G�t�F�N�g���o���L�����N�^�����Ȃ������I�I

        //�X�e�[�g���J�n�X�e�[�g��
        p->SetState(new RoundResetCountdown());
    }
}

void MatchState::PlayerCharacterDrawCursor::Exit(_Client_type_ptr p)
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
    new RoundUI(p->m_Param.round_count++, 0);
}

void MatchState::RoundResetCountdown::Execute(_Client_type_ptr p)
{

    //�t�h�\��
    if (m_Frame == 120)
    {
        new FightUI(0);
    }

    if (++m_Frame > 140)
    {
        //�S���O���Đ�
        Sound::Play(Sound::Gong_Start);

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
m_Frame(0),
m_PreLiveCount(0)
{

}

MatchState::MatchPlay::~MatchPlay()
{

}

MatchState::MatchPlay::GameStopMgr::GameStopMgr()
{
    m_Ep_frame = 0;
    m_Stop_frame = 0;
    m_pUpdateChr = nullptr;
}

//���t���[���̍X�V
void MatchState::MatchPlay::GameStopMgr::Update()
{
    if (m_Ep_frame == 0)
    {
        std::list<LpGameObjectBase> UpdateList;

        if (m_pUpdateChr != nullptr)
        {
            UpdateList.push_back(m_pUpdateChr);
        }

        DefGameObjMgr.FreezeOtherObjectUpdate(UpdateList, (UINT)m_Stop_frame);
    }

    m_Ep_frame = max(m_Ep_frame - 1, -1);
}

//�X�g�b�v������(�����F���t���[����ɃX�g�b�v���邩�A���t���[���X�g�b�v���邩)
void MatchState::MatchPlay::GameStopMgr::SetStop(UINT ep_frame, UINT stop_frame, CharacterBase* pUpdateChr)
{
    m_Ep_frame   = (int)ep_frame;
    m_Stop_frame = (int)stop_frame;
    m_pUpdateChr = pUpdateChr;
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
    //�͂��߂̃��E���h�Ȃ�Đ�
    if (p->m_Param.round_count == 2)
    {
        Sound::Play(Sound::BGM_Battle, 1, true);
    }

    //�v���C�X�^�[�g���b�Z�[�W�𑗐M
    DefGameObjMgr.SendMsg(GameObjectBase::MsgType::_PlayStart);
}

void MatchState::MatchPlay::Execute(_Client_type_ptr p)
{
    const UINT liveCount = DefCharacterMgr.GetCharacterLiveCount();

    //�Q�[���X�g�b�v�Ǘ��N���X�̍X�V
    m_GameStopMgr.Update();

    //������
    if (m_PreLiveCount == 0)
    {
        m_PreLiveCount = liveCount;
    }

    //�X�e�[�g�ύX�֐��̌Ăяo��
    SwitchState(liveCount,p);

    //�O��̃t���[���ƃL�����N�^����������ꍇ�͎��Ԃ��Ƃ߂�
    if (m_PreLiveCount != liveCount && liveCount > 1)
    {
       // Sound::Play(Sound::);

        {
            //���񂾃L�����N�^�ȊO�̍X�V���Ƃ߂�
            m_GameStopMgr.SetStop(4, 30);
        }
        p->SetLightChange();
    }

    //�����L�����N�^�J�E���^���X�V
    m_PreLiveCount = liveCount;

    //�������Ă���L�����N�^�f�[�^���X�V����
    GetLiveCharacterMap(m_CharacterMap);
}

void MatchState::MatchPlay::Exit(_Client_type_ptr p)
{

}

CharacterBase* MatchState::MatchPlay::GetNowDieCharacter()
{
    //�����񂾃L�����N�^����������
    CharacterBase* pRet = nullptr;

    CharacterManager::CharacterMap NowLiveMap;
    GetLiveCharacterMap(NowLiveMap);


    for (auto& it1 : m_CharacterMap)
    {
        //���������ǂ���
        bool find_succes = true;

        for (auto& it2 : NowLiveMap)
        {
            //�����̃f�[�^�ɐ������Ă����琶���Ă���
            if (it1.first == it2.first)
            {
                find_succes = false;
                break;
            }
        }

        if (find_succes)
        {
            pRet = it1.first;
        }
    }

    return pRet;
}

void MatchState::MatchPlay::SwitchState(const UINT liveCount, _Client_type_ptr p)
{
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

        //�v���C�I�����b�Z�[�W�𑗐M
        DefGameObjMgr.SendMsg(GameObjectBase::MsgType::_GameSet);

        return;
    }
    else if (liveCount == 0)
    {
        //���������̏ꍇ
        p->SetState(new Draw);

        //�J�������~�߂�
        DefCamera.SetNewState(new CameraState);

        //�v���C�I�����b�Z�[�W�𑗐M
        DefGameObjMgr.SendMsg(GameObjectBase::MsgType::_GameSet);

        return;
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
        p->SetState(new TimeUp(WinCharacter));

        //�v���C�I�����b�Z�[�W�𑗐M
        DefGameObjMgr.SendMsg(GameObjectBase::MsgType::_GameSet);

        return;
    }

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

    if (m_Frame == 1)
    {
        //�S���O���Đ�
        Sound::Play(Sound::Gong_End);
    }

    if (m_Frame == 6)
    {
        std::list<LpGameObjectBase> UpdateList;

        UpdateList.push_back(new GameSetUI());
        UpdateList.push_back(p);
      
        DefGameObjMgr.FreezeOtherObjectUpdate(UpdateList, 120);

        DefMatchLightManager.LightChangeAndBack(
            3,
            100,
            10,
            Vector3(0.8f, 0.8f, 0.8f)
            );
    }

    
    if (m_Frame == 124)
    {
        //���U���g��ʂɈڍs�\�ȏꍇ�͈ڍs
        p->ResultCheck(m_pWinCharacter);
    }

    if (m_Frame == 240)
    {
        //�����|�[�Y���b�Z�[�W�𑗐M
        m_pWinCharacter->Msg(GameObjectBase::MsgType::_WinPose);

        Sound::Play(Sound::Kira_n);
        DefCamera.SetNewState(new CameraStateCharacterZoom(m_pWinCharacter, 0.05f));

        //���E���h�����������Z
        chr_func::AddWinPoint(m_pWinCharacter);
    }

    if (m_Frame == 400)
    {
        p->SetState(new ResetRound());
    }
}

void MatchState::WinPose::Exit(_Client_type_ptr p)
{

}

//--------------------------------------------------
//              �^�C���A�b�v�X�e�[�g
//--------------------------------------------------

MatchState::TimeUp::TimeUp(LpCharacterBase pWinCharacter) :
m_pWinCharacter(pWinCharacter),
m_Frame(0)
{

}

void MatchState::TimeUp::Enter(_Client_type_ptr p)
{

}

void MatchState::TimeUp::Execute(_Client_type_ptr p)
{
    ++m_Frame;

    if (m_Frame == 1)
    {
        //�S���O���Đ�
        Sound::Play(Sound::Gong_End);

        //�s�������t���@�t�h��\��
        new GameSetUI();

        //���U���g��ʂɈڍs�\�ȏꍇ�͈ڍs
        p->ResultCheck(m_pWinCharacter);
    }

    if (m_Frame == 120)
    {
        //�����|�[�Y���b�Z�[�W�𑗐M
        m_pWinCharacter->Msg(GameObjectBase::MsgType::_WinPose);

        CharacterManager::CharacterMap ChrMap = DefCharacterMgr.GetCharacterMap();

        for (auto& it : ChrMap)
        {
            if (it.first != m_pWinCharacter)
            {
                it.first->Msg(GameObjectBase::MsgType::_LosePose);
            }
        }

        //���E���h�����������Z
        chr_func::AddWinPoint(m_pWinCharacter);

        Sound::Play(Sound::Kira_n);
        DefCamera.SetNewState(new CameraStateCharacterZoom(m_pWinCharacter, 0.05f));
    }

    if (m_Frame == 280)
    {
        p->SetState(new ResetRound());
    }
}

void MatchState::TimeUp::Exit(_Client_type_ptr p)
{

}



//--------------------------------------------------
//              ���������X�e�[�g
//--------------------------------------------------

void MatchState::Draw::Enter(_Client_type_ptr p)
{
    m_Frame = 0;
}

void MatchState::Draw::Execute(_Client_type_ptr p)
{
    ++m_Frame;

    if (m_Frame == 1)
    {
        //�S���O���Đ�
        Sound::Play(Sound::Gong_End);

        //�s�������t���@�t�h��\��
        new GameSetUI();
    }


    if (m_Frame == 120)
    {
        //�c�������f�������@�t�h��\��
        new GameSetUI();
    }

    if (m_Frame == 280)
    {
        p->SetState(new ResetRound());
    }
}

void MatchState::Draw::Exit(_Client_type_ptr p)
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
        20,
        5,
        30
        );
}

void MatchState::ResetRound::Execute(_Client_type_ptr p)
{
    if (++m_Frame == 21)
    {
        //���Z�b�g���b�Z�[�W�𑗐M
        DefGameObjMgr.SendMsg(GameObjectBase::MsgType::_RoundReset);
        
        //�J�����������ʒu��
        DefCamera.SetNewState(new CameraStateGamePlay(true));

        //�X�e�[�g�ڍs
        p->SetState(new RoundResetCountdown());

    }
}

void MatchState::ResetRound::Exit(_Client_type_ptr p)
{

}

