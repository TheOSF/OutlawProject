#include "StageResourceLoadFaction.h"
#include "../GameSystem/ResourceManager.h"


//�X�e�[�W�P�̃��b�V�������\�[�X�ɓo�^
void StageResourceLoadFaction::LoadStage1Object()
{
    //�X�e�[�W�P�����G���W���p���b�V��
    DefResource.Regist(
        Resource::MeshType::Stage1_HitPhysic,
        new iexMesh("DATA\\Stages\\Stage1\\kasenziki\\kasenziki_Hit.imo")
        );


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
        new iexMesh("DATA\\Stages\\Stage1\\Cars\\Bus.imo")
        );

    //��(��
    DefResource.Regist(
        Resource::MeshType::Car1,
        new iexMesh("DATA\\Stages\\Stage1\\Cars\\Car.imo")
        );


    //�S�p�C�v
    DefResource.Regist(
        Resource::MeshType::Pipe,
        new iexMesh("DATA\\Stages\\Stage1\\Pipe.imo")
        );


    //��
    DefResource.Regist(
        Resource::MeshType::River,
        new iexMesh("DATA\\Stages\\Stage1\\river\\sea.imo")
        );


}

//�X�e�[�W�P�̃��b�V�������\�[�X����폜
void StageResourceLoadFaction::ReleaseStage1Object()
{
    //�v���X�`�b�N�R�[��
    DefResource.Erace(
        Resource::MeshType::Cone
        );


    //���x���`
    DefResource.Erace(
        Resource::MeshType::Bench
        );


    //�o�X
    DefResource.Erace(
        Resource::MeshType::Bus
        );

    //��(��
    DefResource.Erace(
        Resource::MeshType::Car1
        );


    //�S�p�C�v
    DefResource.Erace(
        Resource::MeshType::Pipe
        );


    //��
    DefResource.Erace(
        Resource::MeshType::River
        );
}