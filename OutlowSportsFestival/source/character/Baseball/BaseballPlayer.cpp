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

//*************************************************************
//		野球プレイヤークラス
//*************************************************************

const BaseballPlayer::SkillParam BaseballPlayer::skillparams =
{
	0.05f,
	0.05f,
	0.05f,
};

//　コンストラクタ
BaseballPlayer::BaseballPlayer(const CharacterBase::PlayerInfo& info) :
CharacterBase(info, new  BlendAnimationMesh(GetCharacterModelPath(CharacterType::_Baseball))), batterflg(true), changetime(20),
changeflg(false)
{
	m_pStateMachine = new BaseballStateMachine(this);
	SetState(BaseballState_PlayerControll_Move::GetPlayerControllMove(this));
	//　体力低下(デバック用)
	m_Params.maxHP = m_Params.HP = 100;
	//m_Params.size = 1.8f;
	temp_batterflg = batterflg;

    HeadEquip = new BaseballEquip(&m_Renderer, BaseballEquip::MeshType::Helmet, 1);
    WeaponEquip = new BaseballEquip(&m_Renderer, BaseballEquip::MeshType::Bat, 1);
	
    m_ModelSize = 0.06f;
}

//　デストラクタ
BaseballPlayer::~BaseballPlayer()
{
	delete m_pStateMachine;
	
}

//　ステートセット
bool BaseballPlayer::SetState(BaseballState* state, int Important)
{
	//　新しいステートをセット
	return m_pStateMachine->set_state(state,Important);
}

//　更新
bool BaseballPlayer::Update()
{

	//　切り替え可能時間増加
	changetime++;
	// ステート実行
	m_pStateMachine->state_execute();
	//　装備品切替
	if (temp_batterflg != batterflg)
	{
		CheangeEquip();
	}
	//キャラクタ基本更新
	BaseUpdate();
	
	return true;	//常にtrueを返すと消去されない
}


bool  BaseballPlayer::CharacterMsg(MsgType mt)
{
	//　ラウンドごとにリセット
	if (mt == MsgType::_RoundReset)
	{
		Riset();
	}

	return m_pStateMachine->Msg(mt);
}

void BaseballPlayer::CheangeEquip()
{
		//　バッターへ
		if (batterflg)
		{
			//　現在装備している物を物理挙動&nullptr代入
            HeadEquip->Takeoff();
            WeaponEquip->Takeoff();

			//　新しい装備をnew
            HeadEquip = new BaseballEquip(&m_Renderer, BaseballEquip::MeshType::Helmet, 1);
            WeaponEquip = new BaseballEquip(&m_Renderer, BaseballEquip::MeshType::Bat, 1);

		}
		else
		{
			//　現在装備している物を物理挙動&nullptr代入
            HeadEquip->Takeoff();
            WeaponEquip->Takeoff();

			//　新しい装備をnew
            HeadEquip = new BaseballEquip(&m_Renderer, BaseballEquip::MeshType::Cap, 1);
            WeaponEquip = new BaseballEquip(&m_Renderer, BaseballEquip::MeshType::Grove, 1);

		}
		//　一時保存に今のフラグを代入
		temp_batterflg = batterflg;
	
}

//　リセット
void BaseballPlayer::Riset()
{
	SetState(BaseballState_PlayerControll_Move::GetPlayerControllMove(this));
	
	m_Renderer.SetMotion(baseball_player::_mb_Stand_B);
	m_Renderer.Update(0);
	//batterflg = true;
	changetime = 20;

}

