#include "StageResourceLoadFaction.h"
#include "../GameSystem/ResourceManager.h"


//ステージ１のメッシュをリソースに登録
void StageResourceLoadFaction::LoadStage1Object()
{
    //ステージ１物理エンジン用メッシュ
    DefResource.Regist(
        Resource::MeshType::Stage1_HitPhysic,
        new iexMesh("DATA\\Stages\\Stage1\\kasenziki\\kasenziki_Hit.imo")
        );


    //バス
    DefResource.Regist(
        Resource::MeshType::Bus,
        new iexMesh("DATA\\Stages\\Stage1\\Cars\\Bus.imo")
        );

    //車(赤
    DefResource.Regist(
        Resource::MeshType::Car1,
        new iexMesh("DATA\\Stages\\Stage1\\Cars\\Car.imo")
        );


    //川
    DefResource.Regist(
        Resource::MeshType::River,
        new iexMesh("DATA\\Stages\\Stage1\\river\\sea.imo")
        );



    LoadCommonObject();
}

//ステージ１のメッシュをリソースから削除
void StageResourceLoadFaction::ReleaseStage1Object()
{
    //ステージ１物理エンジン用メッシュ
    DefResource.Erace(
        Resource::MeshType::Stage1_HitPhysic
        );

    //バス
    DefResource.Erace(
        Resource::MeshType::Bus
        );

    //車(赤
    DefResource.Erace(
        Resource::MeshType::Car1
        );


    //川
    DefResource.Erace(
        Resource::MeshType::River
        );

    ReleaseCommonObject();
}





//ステージ２のメッシュをリソースに登録
void StageResourceLoadFaction::LoadStage2Object()
{
    //ステージ１物理エンジン用メッシュ
    DefResource.Regist(
        Resource::MeshType::Stage2_HitPhysic,
        new iexMesh("DATA\\Stages\\Stage2\\Ground.imo")
        );


    LoadCommonObject();

}

//ステージ２のメッシュをリソースから削除
void StageResourceLoadFaction::ReleaseStage2Object()
{
    DefResource.Erace(
        Resource::MeshType::Stage2_HitPhysic
        );


    ReleaseCommonObject();
}



void StageResourceLoadFaction::LoadCommonObject()
{
    //プラスチックコーン
    DefResource.Regist(
        Resource::MeshType::Cone,
        new iexMesh("DATA\\Stages\\Stage1\\Cone.imo")
        );


    //青いベンチ
    DefResource.Regist(
        Resource::MeshType::Bench,
        new iexMesh("DATA\\Stages\\Stage1\\Bench.imo")
        );


    //鉄パイプ
    DefResource.Regist(
        Resource::MeshType::Pipe,
        new iexMesh("DATA\\Stages\\Stage1\\Pipe.imo")
        );
}

void StageResourceLoadFaction::ReleaseCommonObject()
{
    //プラスチックコーン
    DefResource.Erace(
        Resource::MeshType::Cone
        );


    //青いベンチ
    DefResource.Erace(
        Resource::MeshType::Bench
        );


    //鉄パイプ
    DefResource.Erace(
        Resource::MeshType::Pipe
        );
}