#include "StageResourceLoadFaction.h"
#include "../GameSystem/ResourceManager.h"


//ステージ１のメッシュをリソースに登録
void StageResourceLoadFaction::LoadStage1Object()
{
    //プレスチックコーン
    DefResource.Regist(
        Resource::MeshType::Cone,
        new iexMesh("DATA\\Stages\\Stage1\\Cone.imo")
        );


    //青いベンチ
    DefResource.Regist(
        Resource::MeshType::Bench,
        new iexMesh("DATA\\Stages\\Stage1\\Bench.imo")
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


    //鉄パイプ
    DefResource.Regist(
        Resource::MeshType::Pipe,
        new iexMesh("DATA\\Stages\\Stage1\\Pipe.imo")
        );
}