#include "StageResourceLoadFaction.h"
#include "../GameSystem/ResourceManager.h"


//�X�e�[�W�P�̃��b�V�������\�[�X�ɓo�^
void StageResourceLoadFaction::LoadStage1Object()
{
    //�v���X�`�b�N�R�[��
    DefResource.Regist(
        Resource::MeshType::Cone,
        new iexMesh("DATA\\Stages\\Stage1\\Cone.imo")
        );


    //���x���`
    DefResource.Regist(
        Resource::MeshType::Bench,
        new iexMesh("DATA\\Stages\\Stage1\\Bench.imo")
        );


    //�o�X
    DefResource.Regist(
        Resource::MeshType::Bus,
        new iexMesh("DATA\\Stages\\Stage1\\Bus\\Bus.imo")
        );

    //�S�p�C�v
    DefResource.Regist(
        Resource::MeshType::Pipe,
        new iexMesh("DATA\\Stages\\Stage1\\Pipe.imo")
        );
}