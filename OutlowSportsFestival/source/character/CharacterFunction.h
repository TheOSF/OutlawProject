#ifndef __CHARACTER_FUNCTION_H__
#define __CHARACTER_FUNCTION_H__

#include "iextreme.h"
#include "../GameSystem/ForwardDecl.h"
#include "../Damage/Damage.h"

class CharacterBase;

//�L�����N�^�p�֐��W

namespace chr_func
{
    //��{�I�ȍX�V(���W�X�V�A�ǂƂ̔���Ȃ�)�����ׂčs��
    void UpdateAll(CharacterBase* p, DamageManager::HitEventBase*	pHitEvent);

    //��{�I�ȍX�V(���W�X�V�A�ǂƂ̔���Ȃ�)�����ׂčs��
    void UpdateAll(CharacterBase* p, DamageManager::HitEventBase*	pHitEvent, RATIO Speed);

	//���W�Ɉړ��ʂ��X�V����
    void PositionUpdate(CharacterBase* p, RATIO t = 1.0f);
			
	//�w�y���ňړ��ʂ𑫂�(max_speed�𒴂��Ȃ��悤�ɐݒ肳���)
	void AddXZMove(CharacterBase*p, float x, float z, float max_speed);

	//�O�����Ɉړ��ʂ��^�X
	void AddMoveFront(CharacterBase*p, float speed, float max_speed);

	//���X�Ɍ�������(t���P�ɂ���ƈ�u�Ŏ~�܂�,0�Ō��ʂȂ�)
	void XZMoveDown(CharacterBase*p, float t);

	//�w�y�����̈ړ��ʂ�max_speed�𒴂��Ă�����Amax_speed�ɐݒ肷��
	void SetMaxXZspeed(CharacterBase*p, float max_speed);

	//�w�肵���ꏊ�Ɍ���
	void AngleControll(CharacterBase* p, CrVector3 view_pos, float speed);

	//�w�肵���ꏊ�Ɍ���(��u��)
	void AngleControll(CharacterBase* p, CrVector3 view_pos);

	//���݂̈ʒu�A���݂�angle�AScale�����Ƃɕϊ��s��𐶐�����
	void CreateTransMatrix(CharacterBase* p, float Scale, Matrix* pOutMatrix);

	//�O�����x�N�g���𓾂�
	void    GetFront(CharacterBase* p, Vector3* pOut);
    Vector3 GetFront(CharacterBase* p);

    //�E�����x�N�g���𓾂�
    void    GetRight(CharacterBase* p, Vector3* pOut);
    Vector3 GetRight(CharacterBase* p);

	//�L�����N�^������ł��邩�ǂ���
	bool isDie(const CharacterBase* p);

	//�_���[�W������Ƃ�
	void DamageCheck(
		LpCharacterBase					pCharacter,	//������Ƃ�L�����N�^
		DamageManager::HitEventBase*	pHitEvent	//�C�x���g����
		);

    //�n�ʔ�����Ƃ�(�߂�l�F�n�ʂɂ��Ă��邩�ǂ���)
    bool isTouchGround(CharacterBase* p);

    //Y�������̑��x���X�V����
    void UpdateMoveY(CharacterBase* p, RATIO t = 1.0f);

    //Y�������̒n�ʔ�����s��
    void CheckGround(CharacterBase* p);

    //XZ�����̕ǂƂ̐ڐG������s��(�߂�l���ǂƂ������Ă��邩�ǂ���)
    bool CheckWall(CharacterBase* p);

    //���݂̗̑͂̊����𓾂�i�O�`�P�j
    RATIO GetLifeRatio(CharacterBase* p);

    //�ړ��ʂ��[���ɂ���
    void ResetMove(CharacterBase* p);

    //�����̏ꏊ�������̃L�����N�^����݂ĉE���ǂ���
    bool isRight(CharacterBase* p, CrVector3 pos);

    //�����̏ꏊ�������̃L�����N�^����݂đO���ǂ���
    bool isFront(CharacterBase* p, CrVector3 pos);

    //�����̃L�����N�^�̃Q�[�W�����Z����
    void AddSkillGauge(CharacterBase* p, RATIO value);

    //�����̃L�����N�^�̃Q�[�W�����Z�b�g����(�O�ɂ���)
    void ResetSkillGauge(CharacterBase* p);

    //�����̃X�L���l���K�E�Z�𔭓��ł��邩�ǂ���
    bool isCanSpecialAttack(RATIO value);

    //�����̃L�����N�^���K�E�Z�𔭓��ł��邩�ǂ���
    bool isCanSpecialAttack(CharacterBase* p);

    //�����̃L�����N�^�̂r�o���O���ǂ���
    bool isSkillZero(CharacterBase* p);

    //�L�����N�^�̗̑͂��_���[�W�ɂ���Č���������
    void CalcDamage(CharacterBase* p, float value, bool DontDie = false);

    //�U���̃^�[�Q�b�g�𓾂�(�����̃^�[�Q�b�g�������ꍇ�A���g�̂����Ƃ��O�ɂ���L�����N�^��I��)
    bool CalcAtkTarget(CharacterBase* pOwner, RADIAN check_angle, float check_distance, CharacterBase** ppOut);

    //�L�����N�^�̃��E���h�����������Z����
    void AddWinPoint(CharacterBase* p);
}

#endif