#ifndef __CHARACTER_BASE_H__
#define __CHARACTER_BASE_H__

#include "../GameSystem/GameObject.h"
#include "../GameSystem/GameSystem.h"
#include "../utillity/ColorUtility.h"

#include "CharacterRenderer.h"
#include "iextreme.h"
#include "CharacterPhysic.h"
#include "../Camera/Camera.h"

//***************************************************
//		�L�����N�^�N���X
//***************************************************


class CharacterBase :public GameObjectBase
{
private:
	CharacterBase(const CharacterBase&);
public:

	struct PlayerInfo
	{
		PlayerNum::Value		number;			//�v���C���[�ԍ�
		PlayerType::Value		player_type;	//�v���C���[�^�C�v
		StrongType::Value		strong_type;	//�����^�C�v
		CharacterType::Value	chr_type;		//�L�����N�^�^�C�v
	};

	struct Params
	{
		UINT                win;        //���������E���h��
		Vector3				pos;		//���W
		Vector3				move;		//�ړ���
		float				hitScale;	//�����蔻��̉~�̑傫��
		float				angle;		//�A���O��(���W�A��)
		float				maxHP;		//�ő�̗͂O�`
		float				HP;			//�O�`
		float				SP;			//�O�`�P�O�O
        float               size;       //���̃L�����N�^�������̂���傫��
        bool                DoCheckOtherChrSpace; //���̃L�����N�^�������̂��邩�ǂ���
	};

    //�L�����N�^���ʃp�����[�^
    struct CommonParams
    {
        float  GroundY; //�n�ʂ̍���    
        float  Glavity; //�d�͉����x
    };

    //�L�����N�^�̌��݂̏��
    enum class State
    {
        Usual,    //�ʏ�(�퓬��
        WinPose,  //�����|�[�Y��
        LosePose, //�����|�[�Y��
        Freeze,   //�Ƃ܂��Ă���
    };

    static const CommonParams m_CommonParams;

	const PlayerInfo	m_PlayerInfo; //�v���C���[���ɂ���\����
	Params				m_Params;     //�L�����N�^��{�p�����[�^�[
    CharacterRenderer	m_Renderer;   //�����_��
    float               m_ModelSize;  //���f���X�P�[��
    CameraDrawObject    m_DrawObject; //�J�������ʂ��I�u�W�F�N�g
   
    CharacterBase(const PlayerInfo& info, BlendAnimationMesh* pMesh);
	virtual ~CharacterBase();

    static COLOR  GetPlayerColor (PlayerNum::Value number);
    static COLORf GetPlayerColorF(PlayerNum::Value number);

    static const char* GetCharacterModelPath(CharacterType::Value chr_type);
    static void CreateCharacterModel(CharacterRenderer* pOut, CharacterType::Value chr_type, PlayerNum::Value PlNum);

    static int GetWinMotion(CharacterType::Value chr_type);
    static int GetLoseMotion(CharacterType::Value chr_type);

    //�����蔻����Ƃ���W�𓾂�
    Vector3 CalcHitCheckPos()const;

    //���E���h���Z�b�g���̃p�����^�[�ɃZ�b�g����
    void ResetRound();

    //���t���[���̍X�V
    void BaseUpdate();

	virtual bool Update() = 0;	//���̊֐���false��Ԃ���delete�����
    virtual bool CharacterMsg(MsgType mt) = 0; //��M�ł������b�Z�[�W��true��
    
    bool Msg(MsgType mt);

    State GetStateType()const;
private:
    CharacterPhysic     m_PhysicObj;  //��������I�u�W�F�N�g
    State               m_StateType;  //���݂̃X�e�[�g�^�C�v

    void CameraDrawObjUpdate();

protected:

    void RendererUpdate(CharacterRenderer* pRenderer);
};

#endif