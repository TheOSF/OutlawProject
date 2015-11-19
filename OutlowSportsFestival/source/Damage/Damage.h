#ifndef __DAMAGE_H__
#define __DAMAGE_H__

#include "iextreme.h"
#include "../GameSystem/ForwardDecl.h"
#include <map>
#include <bitset>
#include <array>
//*************************************************************
//		�_���[�W����N���X�w�b�_
//*************************************************************


//���̍\����
struct SphereParam
{
	Vector3		pos;
	float		size;
};

//�J�v�Z���̍\����
struct CapsureParam
{
    float       width;
    Vector3		pos1;
    Vector3		pos2;
};

//**************************************************************************
//	�_���[�W�N���X(�R���X�g���N�^�E�f�X�g���N�^�Ŏ����I�ɓo�^�E�폜������)
//**************************************************************************
class DamageBase
{
public:
    
    class HitCallBack
    {
    public:
        virtual ~HitCallBack(){}
        virtual void Hit(const SphereParam* sp) = 0;
    };

	enum Type
	{
		_WeekDamage,	//���݂݂̂̃_���[�W
		_VanishDamage,	//������у_���[�W
		_UpDamage,		//��ɐ������(�o���[�Ƃ�)�ǌ��\
	};

    enum class Option
    {
        _DontDie,   //���S���Ȃ�(�K���P�c��
    };

	LpCharacterBase	    pParent;	    //���̃_���[�W����̌��̃L�����N�^(�e���L�����N�^�łȂ��ꍇ��nullptr�������Ă���)
	LpBallBase		    pBall;		    //���̔���ɂ��Ă���{�[��(�{�[���łȂ����null)
    HitCallBack*        pCallBackClass; //�R�[���o�b�N�N���X(null�̏ꍇ�͂�яo���Ȃ�)
    Type			    type;		    //���̃_���[�W�̃^�C�v
	float			    Value;		    //�l
	int				    HitCount;	    //����������
    int                 MaxChrHit;      //�P�L�����N�^�ɓ����鑍��

	DamageBase();
	virtual ~DamageBase();

    bool isOptionOn(Option op)const;       //�����̃I�v�V������on���ǂ���
    void SetOption(Option op, bool flag);  //�I�v�V������on,off���Z�b�g

    void ResetCounts();                    //�q�b�g�J�E���g�A�L�����q�b�g�J�E���g�����Z�b�g
    void AddHitCount(CharacterBase* pHitChr);            //�L�����N�^�ɓ��������ꍇ�J�E���g�����Z
    bool isCanHitCharacter(CharacterBase* pHitChr)const; //�����̃L�����N�^�ɓ�����邩�ǂ���

    virtual bool HitCheckSphere(const SphereParam* sp) = 0;
    virtual void CalcPosVec(CrVector3 hit_pos, Vector3* pOutPos, Vector3* pOutVec) = 0;
   
    virtual void DebugDraw() = 0;

private:
    static const int            MaxPlayer = 4;
    std::bitset<16>             m_OptionFlags;
    std::array<int, MaxPlayer>  m_PlayerHitCounts;
};



//*************************************************************
//	�_���[�W�N���X(��)
//*************************************************************
class DamageShpere :public DamageBase
{
public:

    //������ѕ����̗�
    enum class DamageVecType
    {
        MemberParam,     //�����o�[�ϐ��̕���(m_Vec�̕���)
        CenterToPos,     //���S������ˏ��
        PosToCenter,     //���S�Ɍ�������

        CenterToPosXZ, //���S������ˏ��(Y�����͏��m_VecPower.y�̒l�ɂȂ�)
        PosToCenterXZ, //���S�Ɍ�������  (Y�����͏��m_VecPower.y�̒l�ɂȂ�)
    };

    
	bool			m_Enable;	//���̃_���[�W���L�����ǂ���
    SphereParam		m_Param;	//���̃_���[�W�̋��̍\����

    DamageVecType   m_VecType;  //�_���[�W�����^�C�v
    Vector3         m_Vec;      //�_���[�W����(m_VecType��MemberParam�̏ꍇ�̂ݗL��)
    Vector2         m_VecPower; //�_���[�W�̐�����ё��x(x = XZ���� y=Y������)

	DamageShpere();

    bool HitCheckSphere(const SphereParam* sp);
    void CalcPosVec(CrVector3 hit_pos, Vector3* pOutPos, Vector3* pOutVec);

private:
    void DebugDraw();
};

//*************************************************************
//	�_���[�W�N���X(�J�v�Z��)
//*************************************************************
class DamageCapsure :public DamageBase
{
public:
    //������ѕ����̗�
    enum class DamageVecType
    {
        MemberParam,  //�����o�[�ϐ��̕���(m_Vec�̕���)
        CenterToPos,  //���S������ˏ��
        PosToCenter,  //���S�Ɍ�������

        CenterToPosXZ, //���S������ˏ��(Y�����͏��m_VecPower.y�̒l�ɂȂ�)
        PosToCenterXZ, //���S�Ɍ�������  (Y�����͏��m_VecPower.y�̒l�ɂȂ�)
    };

    bool			m_Enable;	//���̃_���[�W���L�����ǂ���
    CapsureParam	m_Param;	//���̃_���[�W�̋��̍\����

    DamageVecType   m_VecType;  //�_���[�W�����^�C�v
    Vector3         m_Vec;      //�_���[�W����(m_VecType��MemberParam�̏ꍇ�̂ݗL��)
    Vector2         m_VecPower; //�_���[�W�̐�����ё��x(x = XZ���� y=Y������)

    DamageCapsure();

    bool HitCheckSphere(const SphereParam* sp);
    void CalcPosVec(CrVector3 hit_pos, Vector3* pOutPos, Vector3* pOutVec);

private:
    void DebugDraw();
};








//*************************************************************
//		�_���[�W����}�l�[�W��
//*************************************************************

class DamageManager
{
public:
	//�C���X�^���X�Q�b�^
	static DamageManager& GetInstance();
	static void Release();

	//�_���[�W�q�b�g���̃C�x���g�N���X
	class HitEventBase
	{
	public:
		virtual ~HitEventBase(){}
		virtual bool Hit(DamageBase* pDmg){ return false; }	//�����������ɌĂ΂��֐�(�߂�l�F�����������ǂ���)
	};

	//���Ń_���[�W������擾����
	void HitCheckSphere(
        const SphereParam&	sp,
		HitEventBase&		HitEvent
		);

    //�����蔻����f�o�b�O�`��
    void DebugDraw();

    //�f�o�b�O�`������s���邩�ǂ���
    bool m_DebugDrawVisible;

private:
	friend class DamageBase;
	typedef std::map<DamageBase*, DamageBase*> DamageBaseMap;
	
	static DamageManager*	m_pInstance;
	DamageBaseMap			m_DamageBaseMap;
	
	DamageManager();
	~DamageManager();

	bool AddDamage(DamageBase* pDmg);
	bool EraceDamage(DamageBase* pDmg);
};

typedef DamageManager::HitEventBase HitEventBase;

#define DefDamageMgr (DamageManager::GetInstance())

#endif