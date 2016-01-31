#include "BaseballPlayer.h"
#include "BaseballPlayerState.h"

#include "Computer\/BaseballPlayerState_ComMove.h"
#include "BaseballState_Change.h"
#include "../../Damage/Damage.h"
#include "../../GameSystem/GameController.h"
#include "../../Ball/UsualBall.h"
#include "../../Effect/EffectFactory.h"
#include "Sound/Sound.h"
#include "../CharacterFunction.h"
#include "../CharacterManager.h"

//----------------------------------
//		�싅�v���C���[�N���X
//----------------------------------

//�R���X�g���N�^
BaseballPlayer::BaseballPlayer(const CharacterBase::PlayerInfo& info) :
CharacterBase(info, new  BlendAnimationMesh(GetCharacterModelPath(CharacterType::_Baseball))),
m_BatterFlg(false)
{
	m_pStateMachine = new BaseballStateMachine(this);
    SetState(BaseballState_PlayerControll_Move::GetPlayerControllMove(this));

    {
        m_pBatterModel = &m_Renderer;
        m_pPitcherModel = new CharacterRenderer(new  BlendAnimationMesh("DATA\\CHR\\SanoBaseBall\\baseball.iem"));
        CreateCharacterModel(m_pPitcherModel, CharacterType::_Baseball, m_PlayerInfo.number);

        SetMode(true);
    }

	m_Params.maxHP = m_Params.HP = 110;
	
    m_ModelSize = 0.06f;
}

//�@�f�X�g���N�^
BaseballPlayer::~BaseballPlayer()
{
	delete m_pStateMachine;
    delete m_pPitcherModel;
}

//���݃t�H�[���̃��f���Ƀ��[�V�������Z�b�g
void BaseballPlayer::SetMotion(int motion)
{
    CharacterRenderer* const pNow = getNowModeModel();
    
    pNow->SetMotion(motion);
}

//���݃t�H�[���̃��f�����X�V
void BaseballPlayer::ModelUpdate(RATIO Speed)
{
    getNowModeModel()->Update(Speed);
}

//�@�X�e�[�g�Z�b�g
bool BaseballPlayer::SetState(BaseballState* state, int Important)
{
	//�@�V�����X�e�[�g���Z�b�g
	return m_pStateMachine->set_state(state,Important);
}

//�@�X�V
bool BaseballPlayer::Update()
{
	// �X�e�[�g���s
	m_pStateMachine->state_execute();

	//�L�����N�^��{�X�V
	BaseUpdate();

    //�s�b�`���[�p�̃��f���͎��g�ōX�V
    RendererUpdate(m_pPitcherModel);
	
	return true;	//���true��Ԃ��Ə�������Ȃ�
}


bool  BaseballPlayer::CharacterMsg(MsgType mt)
{
	//�@���E���h���ƂɃ��Z�b�g
	if (mt == MsgType::_RoundReset)
	{
		Reset();
	}

	return m_pStateMachine->Msg(mt);
}

//�@�����؂�ւ�
void BaseballPlayer::ChangeMode()
{
    SetMode(!m_BatterFlg);
}

//�t�H�[�����w�肵�ăZ�b�g����
bool BaseballPlayer::SetMode(bool isBatter)
{

    m_pBatterModel->m_Visible = isBatter;
    m_pPitcherModel->m_Visible = !isBatter;

    //�����������ꍇ
    if (isBatter == m_BatterFlg)
    {
        return false;
    }

    CharacterRenderer*const pPre = getNowModeModel();

    m_BatterFlg = isBatter;

    //�s��𓯊�
    getNowModeModel()->m_TransMatrix = pPre->m_TransMatrix;

    return true;
}

//�@���Z�b�g
void BaseballPlayer::Reset()
{
	SetState(BaseballState_PlayerControll_Move::GetPlayerControllMove(this));
}


//���݃t�H�[���̃��f�����Q�b�g
CharacterRenderer* BaseballPlayer::getNowModeModel()
{
    if (isBatter())
    {
        return m_pBatterModel;
    }

    return m_pPitcherModel;
}