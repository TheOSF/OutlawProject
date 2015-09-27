#pragma once

#include "iextreme.h"

//------------------------------------------//
// 　　　パーティクル管理クラス
//------------------------------------------//

class ParticleManager
{
public:

    //パーティクルデータ構造体
    struct ParticleData
    {
        UINT        count;

        UINT        type;
        Vector3     pos;
        Vector3     move;
        Vector3     power;
        Vector2     size;

        UINT        md_frame;
        UINT        end_frame;

        DWORD       start_color;
        DWORD       md_color;
        DWORD       end_color;

        DWORD       dw_flag;

        float       cameraZ;
    };

    //初期化
    static void Initialze(
        UINT         maxParticle,  //パーティクル最大数
        iex2DObj*    pTexture,     //パーティクルテクスチャ
        UINT         TextureCellX, //テクスチャコマ数Ｘ
        UINT         TextureCellY  //テクスチャコマ数Ｘ
        );

    //インスタンス取得
    static ParticleManager& GetInstance();

    //インスタンス開放
    static void Release();


    //パーティクルセット
    void Set(
        UINT        type,        //パーティクルコマ番号
        CrVector3   pos,         //出現場所
        CrVector3   move,        //初期移動量
        CrVector3   power,       //力場(重力とか)
        CrVector2   size,        //大きさＸＹ

        UINT        md_frame,    //中間フレーム
        UINT        end_frame,   //消去フレーム

        DWORD       start_color, //初期カラー
        DWORD       md_color,    //中間フレームでのカラー
        DWORD       end_color,   //終了時のカラー
        DWORD       dw_flag      //描画フラグ
        );

    //パーティクルセット
    void Set(
        UINT        type,     //パーティクルコマ番号
        CrVector3   pos,      //出現場所
        CrVector3   move,     //初期移動量
        CrVector3   power,    //力場(重力とか)
        CrVector2   size,     //大きさＸＹ
        UINT        end_frame,//消去フレーム
        DWORD       color = 0xFFFFFFFF, //色
        DWORD       dw_flag = RS_COPY //描画フラグ
        );





    //描画
    void Render();

    //更新
    void Update();





private:

    

    ParticleManager(
        UINT         maxParticle,  //パーティクル最大数
        iex2DObj*    pTexture,     //パーティクルテクスチャ
        UINT         TextureCellX, //テクスチャコマ数Ｘ
        UINT         TextureCellY  //テクスチャコマ数Ｘ
        );

    ~ParticleManager();

    //ソート
    void ParticleSort();


    static ParticleManager* m_pInstance;

    const UINT         m_MaxParticle;  //パーティクル最大数
    iex2DObj* const    m_pTexture;     //パーティクルテクスチャ
    const UINT         m_TextureCellX; //テクスチャコマ数Ｘ
    const UINT         m_TextureCellY;  //テクスチャコマ数Ｘ

    ParticleData*      m_ParticleDataArray;
    ParticleData**     m_pWorkParticleDataPointerArray;
    int                m_SetCount;
};


#define DefParticle (ParticleManager::GetInstance())