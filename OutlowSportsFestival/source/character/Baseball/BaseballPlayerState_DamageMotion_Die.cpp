#include "BaseballPlayerState_DamageMotion_Die.h"
#include "Baseball_HitEvent.h"
#include "../CharacterFunction.h"
#include "../../Effect/BlurImpact.h"
#include "../../Effect/HitEffectObject.h"
#include "../../Ball/Ball.h"
#include "../../Camera/Camera.h"
#include "../../Effect/EffectFactory.h"
#include "../../GameSystem/GameController.h"


BaseballState_DamageMotion_Die::BaseballState_DamageMotion_Die(
	BaseballPlayer* pBaseball,
	const Vector3& Damage_vec  //ダメージを受けた方向
	) :
	m_pBaseball(pBaseball),
	m_Damage_vec(Damage_vec)
{

}

void BaseballState_DamageMotion_Die::Enter(BaseballPlayer* b)
{
	//キャラクタ共通ひるみクラスのテニス固有イベントクラス
	class BaseballEvent :public CharacterDamageVanish::Event
	{
	public:
		BaseballEvent(BaseballPlayer* pBaseball) :m_pBaseball(pBaseball){}

		void FlyStart()
		{
			//吹き飛びモーションをセット
            m_pBaseball->getNowModeModel()->SetMotion(baseball_player::_mb_Damage_Vanish_Fly);
		}

        void Flying(const Matrix& Rotate, RATIO t)
		{
			//モデルのアニメーション更新
            m_pBaseball->getNowModeModel()->Update(t);

			//位置にもとずき、ワールド変換行列を計算
            chr_func::CreateTransMatrix(m_pBaseball, &m_pBaseball->getNowModeModel()->m_TransMatrix);

			//吹き飛びの回転を入れる
            m_pBaseball->getNowModeModel()->m_TransMatrix = Rotate*m_pBaseball->getNowModeModel()->m_TransMatrix;
		}

		void DownStart()
		{
			//ダウンモーションをセット
            m_pBaseball->getNowModeModel()->SetMotion(baseball_player::_mb_Damage_Vanish_Down);
		}

		void Downing()
		{
            m_pBaseball->getNowModeModel()->Update(1);

			//ワールド変換行列を計算
            chr_func::CreateTransMatrix(m_pBaseball, &m_pBaseball->getNowModeModel()->m_TransMatrix);
		}

		void StandUpStart()
		{
			//何もしない
		}

		void StandUping()
		{
			//モデルのアニメーション更新
            m_pBaseball->getNowModeModel()->Update(1);

			//ワールド変換行列を計算
            chr_func::CreateTransMatrix(m_pBaseball, &m_pBaseball->getNowModeModel()->m_TransMatrix);
		}

		void End()
		{
			//カメラ写すフラグをfalseに
            m_pBaseball->m_DrawObject.m_isDraw = false;
		}

		void HitWall()
		{
			//壁に当たったモーションをセット
            m_pBaseball->getNowModeModel()->SetMotion(baseball_player::_mb_Damage_Vanish_Wall);
		}

		void HitFloor()
		{
			//床に当たったモーションをセット
            m_pBaseball->getNowModeModel()->SetMotion(baseball_player::_mb_Damage_Vanish_Bound);
		}

		void HitFloorAndStandUp()
		{
			//立ち上がりモーションをセット
            m_pBaseball->getNowModeModel()->SetMotion(baseball_player::_mb_Damage_Vanish_StandUp);
		}

		void HitWallUpdate()
		{
			//モデルのアニメーション更新
            m_pBaseball->getNowModeModel()->Update(1);

			//ワールド変換行列を計算
            chr_func::CreateTransMatrix(m_pBaseball, &m_pBaseball->getNowModeModel()->m_TransMatrix);
		}

	private:
		BaseballPlayer*  m_pBaseball;
	};

	//ダメージモーションパラメーターを作成する
	CharacterDamageVanish::Param Param;

	Param.rotate_speed = Vector3(0.0f, 0.0f, 0.0);
	Param.move = Vector3Normalize(m_Damage_vec) * 0.7f;
	Param.move.y = 0.2f;

	Param.down_frame = 15;
	Param.standup_frame = 50;


	//ひるみクラスを作成
	m_pDamageVanishClass = new CharacterDamageVanish(
		m_pBaseball,
		Param,
		new BaseballEvent(b),
		new DamageManager::HitEventBase()
		);

	//死亡エフェクト
	EffectFactory::DieEffect(
		b,
		m_Damage_vec
		);

	//コントローラを振動
    chr_func::SetControllerShock(
        m_pBaseball,
        0.8f,
        0.5f
        );
}

void BaseballState_DamageMotion_Die::Execute(BaseballPlayer* b)
{
	m_pDamageVanishClass->Update();
}

void BaseballState_DamageMotion_Die::Exit(BaseballPlayer* b)
{
	delete m_pDamageVanishClass;
}