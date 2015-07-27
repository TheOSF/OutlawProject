#ifndef __PLAYER_GAUGE_H__
#define __PLAYER_GAUGE_H__

#include "../character/CharacterBase.h"
#include "../Render/Renderer.h"

//-------------------------------------------
//   �v���C���[�̃Q�[�W�\���N���X
//-------------------------------------------

class PlayerGauge :
	public GameObjectBase,
	public ForwardRenderer
{
public:

	PlayerGauge(
		CharacterBase*   pOwner //�\������L�����N�^
		);

	~PlayerGauge();

private:

	bool Update();
	bool Msg(MsgType mt);

	void CalcZ();
	void Render();
};

#endif