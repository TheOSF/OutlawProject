#ifndef __PLAYER_GAUGE_H__
#define __PLAYER_GAUGE_H__

#include "../character/CharacterBase.h"
#include "../Render/Renderer.h"

//-------------------------------------------
//   プレイヤーのゲージ表示クラス
//-------------------------------------------

class PlayerGauge :
	public GameObjectBase,
	public ForwardRenderer
{
public:

	PlayerGauge(
		CharacterBase*   pOwner //表示するキャラクタ
		);

	~PlayerGauge();

private:

	bool Update();
	bool Msg(MsgType mt);

	void CalcZ();
	void Render();
};

#endif