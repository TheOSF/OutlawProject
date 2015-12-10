#pragma once

class StageResourceLoadFaction
{
public:
    //ステージ１のメッシュをリソースに登録
    static void LoadStage1Object();
    static void ReleaseStage1Object();

    //ステージ２のメッシュをリソースに登録
    static void LoadStage2Object();
    static void ReleaseStage2Object();

private:

    static void LoadCommonObject();
    static void ReleaseCommonObject();
    
};