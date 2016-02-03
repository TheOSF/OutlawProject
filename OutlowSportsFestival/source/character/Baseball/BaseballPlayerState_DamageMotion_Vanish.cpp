#include "BaseballPlayerState_DamageMotion_Vanish.h"
#include "BaseballPlayerState.h"
#include "Baseball_HitEvent.h"
#include "../CharacterFunction.h"
#include "../../Effect/EffectFactory.h"
#include "../../GameSystem/GameController.h"
#include "BaseballState_PlayerControll_Evasion.h"

BaseballState_DamageVanish::BaseballState_DamageVanish(
	BaseballPlayer* pBaseball,
	const Vector3& Damage_vec  //ダメージを受けた方向
	) :
	m_pBaseball(pBaseball),
	m_Damage_vec(Damage_vec)
{

}

void BaseballState_DamageVanish::Enter(BaseballPlayer* b)
{
	
	class BaseballEvent :public CharacterDamageVanish::Event
	{
	public:
		BaseballEvent(BaseballPlayer* pBaseball) :m_pBaseball(pBaseball){}

		void FlyStart()
		{
			//吹き飛びモーションをセット
            if (m_pBaseball->isBatter())
            {
                m_pBaseball->SetMotion(baseball_player::_mb_Damage_Vanish_Fly_B);
            }
            else
            {
                m_pBaseball->SetMotion(baseball_player::_mb_Damage_Vanish_Fly_P);
            }

		}

        void Flying(const Matrix& Rotate, RATIO t)
		{
			//モデルのアニメーション更新
            m_pBaseball->getNowModeModel()->Update(t*0.5f);

			//位置にもとずき、ワールド変換行列を計算
            chr_func::CreateTransMatrix(m_pBaseball, &m_pBaseball->getNowModeModel()->m_TransMatrix);

			//吹き飛びの回転を入れる
            m_pBaseball->getNowModeModel()->m_TransMatrix = Rotate*m_pBaseball->getNowModeModel()->m_TransMatrix;
		}

		void DownStart()
        {
            if (m_pBaseball->isBatter())
            {
                //ダウンモーションをセット
                m_pBaseball->SetMotion(baseball_player::_mb_Damage_Vanish_Down_B);
            }
            else
            {
                //ダウンモーションをセット
                m_pBaseball->SetMotion(baseball_player::_mb_Damage_Vanish_Down_P);
            }
		}

		void Downing()
		{
            m_pBaseball->getNowModeModel()->Update(0.5f);

			//ワールド変換行列を計算
            chr_func::CreateTransMatrix(m_pBaseball, &m_pBaseball->getNowModeModel()->m_TransMatrix);
		}

		void StandUpStart()
		{
			//起き上がりモーションをセット
            if (m_pBaseball->isBatter())
            {
                m_pBaseball->SetMotion(baseball_player::_mb_Damage_Vanish_StandUp_B);
            }
            else
            {
                m_pBaseball->SetMotion(baseball_player::_mb_Damage_Vanish_StandUp_P);
            }
		}

		void StandUping()
		{
			//モデルのアニメーション更新
            m_pBaseball->getNowModeModel()->Update(0.5f);

			//ワールド変換行列を計算
            chr_func::CreateTransMatrix(m_pBaseball, &m_pBaseball->getNowModeModel()->m_TransMatrix);
		}

		void End()
		{
			//通常ステートをセット
			m_pBaseball->SetState(
				BaseballState_PlayerControll_Move::GetPlayerControllMove(m_pBaseball)
				);
		}
		void HitWall()
		{
			//壁に当たったモーションをセット
            if (m_pBaseball->isBatter())
            {
                m_pBaseball->SetMotion(baseball_player::_mb_Damage_Vanish_WallHitAndDown_B);
            }
            else
            {
                m_pBaseball->SetMotion(baseball_player::_mb_Damage_Vanish_WallHitAndDown_P);
            }
		}

		void HitFloor()
		{
			//床に当たったモーションをセット
            if (m_pBaseball->isBatter())
            {
                m_pBaseball->SetMotion(baseball_player::_mb_Damage_Vanish_Bound_B);
            }
            else
            {
                m_pBaseball->SetMotion(baseball_player::_mb_Damage_Vanish_Bound_P);
            }
		}

		void HitFloorAndStandUp()
		{
            if (m_pBaseball->isBatter())
            {
                //立ち上がりモーションをセット
                m_pBaseball->SetMotion(baseball_player::_mb_Damage_Vanish_StandUp_B);
            }
            else
            {
                m_pBaseball->SetMotion(baseball_player::_mb_Damage_Vanish_StandUp_P);
            }
            
		}

		void HitWallUpdate()
		{
			//モデルのアニメーション更新
            m_pBaseball->getNowModeModel()->Update(0.5f);

			//ワールド変換行列を計算
            chr_func::CreateTransMatrix(m_pBaseball, &m_pBaseball->getNowModeModel()->m_TransMatrix);
		}

		void CanActionUpdate()
		{
			//行動分岐が可能なときに呼ばれる
            if (m_pBaseball->m_PlayerInfo.player_type == PlayerType::_Player)
            {
                if (controller::GetPush(controller::button::batu, m_pBaseball->m_PlayerInfo.number))
                {
                    m_pBaseball->SetState(new BaseballState_Rolling());
                }
            }
            else
            {
                if (frand() < 0.02f)
                {
                    m_pBaseball->SetState(new BaseballState_Rolling(Vector3Rand() - Vector3Normalize(m_pBaseball->m_Params.pos)));
                }
            }
		}

	private:
		BaseballPlayer*  m_pBaseball;
	};

	//ダメージモーションパラメーターを作成する
	CharacterDamageVanish::Param Param;

	Param.rotate_speed = Vector3(0.0f, 0.0f, 0.0);
	Param.move = m_Damage_vec;

	Param.down_frame = 15;
	Param.standup_frame = 50;


	//ひるみクラスを作成
	m_pDamageVanishClass = new CharacterDamageVanish(
		m_pBaseball,
		Param,
		new BaseballEvent(b),
		new BaseballHitEvent(b)
		);

    //エフェクト
    EffectFactory::VanishEffect(
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

void BaseballState_DamageVanish::Execute(BaseballPlayer* b)
{
	m_pDamageVanishClass->Update();
}

void BaseballState_DamageVanish::Exit(BaseballPlayer* b)
{
	delete m_pDamageVanishClass;
}