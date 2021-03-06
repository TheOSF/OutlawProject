#include "BaseballPlayerState_DamageMotionWeak.h"
#include "Baseball_HitEvent.h"
#include "BaseballPlayerState.h"
#include "../CharacterFunction.h"
#include "../../GameSystem/GameController.h"
#include "../../Effect/EffectFactory.h"

BaseballState_DamageMotion_Weak::BaseballState_DamageMotion_Weak(
	BaseballPlayer* pBaseball,
	const Vector3& Damage_vec,  //ダメージを受けた方向
    bool           Counter,
    int            Frame
	) :
	m_pBaseball(pBaseball),
	m_Damage_vec(Damage_vec),
    m_Counter(Counter),
    m_Frame(Frame)
{

}

void BaseballState_DamageMotion_Weak::Enter(BaseballPlayer* t)
{
	//キャラクタ共通ひるみクラスの野球固有イベントクラス
	class BaseballEvent :public CharacterDamageMotion::Event
	{
	public:
		BaseballEvent(BaseballPlayer* pBaseball) :m_pBaseball(pBaseball){}

		void Update(float speed)
		{
			//モデルの更新のみ
            m_pBaseball->getNowModeModel()->Update(speed*0.5f);
            chr_func::CreateTransMatrix(m_pBaseball, &m_pBaseball->getNowModeModel()->m_TransMatrix);
		}
		void Start()
		{
			//ひるみモーションをセット
            if (m_pBaseball->isBatter())
            {
                m_pBaseball->SetMotion(baseball_player::_mb_Damage_Weak_B);
            }
            else
            {
                m_pBaseball->SetMotion(baseball_player::_mb_Damage_Weak_P);
            }
            
		}
		void End()
		{
			//通常ステートをセット
			m_pBaseball->SetState(
				BaseballState_PlayerControll_Move::GetPlayerControllMove(m_pBaseball)
				);
		}
	private:
		BaseballPlayer*  m_pBaseball;
	};

	//ダメージモーションパラメーターを作成する
	CharacterDamageMotion::Params Param;

	Param.damage_vec = m_Damage_vec;
    Param.counter_hit = m_Counter;
    Param.frame = m_Frame;

	//ひるみクラスを作成
	m_pDamageMotionClass = new CharacterDamageMotion(
		m_pBaseball,
		new BaseballEvent(m_pBaseball),
		new BaseballHitEvent(m_pBaseball),
		Param
		);

    //エフェクト
    EffectFactory::HitEffect(
        m_pBaseball,
        m_Damage_vec
        );

    //コントローラを振動
    chr_func::SetControllerShock(
        m_pBaseball,
        0.5f,
        0.15f
        );
}

void BaseballState_DamageMotion_Weak::Execute(BaseballPlayer* t)
{
	m_pDamageMotionClass->Update();
}

void BaseballState_DamageMotion_Weak::Exit(BaseballPlayer* t)
{
	delete m_pDamageMotionClass;
}