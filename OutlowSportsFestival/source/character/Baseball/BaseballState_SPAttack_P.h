#pragma once

#include "BaseballPlayer.h"
#include "../../Ball/Ball.h"
#include "../../Damage/Damage.h"
#include "../../utillity/Locus.h"
#include "../../Render/MeshRenderer.h"

//***************************************************
//		�싅(�s�b�`���[)�K�E�Z
//***************************************************

class Baseball_SPAttack_P :public GameObjectBase, public BallBase
{
public:

	//�R���X�g���N�^
	Baseball_SPAttack_P(BaseballPlayer* b, BallBase::Params& param);
	~Baseball_SPAttack_P();

	bool Update();
	bool Msg(MsgType mt);

private:
	BaseballPlayer* const m_pBaseball;
	LpMeshRenderer		m_pMeshRenderer;
	DamageShpere		m_Damage;
	UINT                m_DeleteFrame;
	D3DXQUATERNION		m_Ballrot;
	Locus               m_Locus;
	Matrix              m_BaseMatrix;
	bool                m_DeleteFlag;

	bool isOutofField()const;

	void UpdateDamageClass();
	void UpdateMesh();
	void UpdateLocusColor();
	void UpdateMove();
	void UpdateWallCheck();
	void ToNoWork();

	void Counter(CharacterBase* pCounterCharacter)override;
};

//***************************************************
//		�싅(�s�b�`���[)�K�E�Z�X�e�[�g�N���X
//***************************************************
class CharacterShotAttack;
class BaseballState_SPAttack_P : public BaseballState
{
private:
	CharacterShotAttack* CreateSpAttack_P(BaseballPlayer* b);
	CharacterShotAttack* m_pSpAttack_P;
public:

	BaseballState_SPAttack_P();
	

	// �X�e�[�g�J�n
	void Enter(BaseballPlayer* b)override;

	// �X�e�[�g���s
	void Execute(BaseballPlayer* b)override;

	// �X�e�[�g�I��
	void Exit(BaseballPlayer* b)override;


};

