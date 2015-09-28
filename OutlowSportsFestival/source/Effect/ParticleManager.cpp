#include"ParticleManager.h"
#include"../Camera/Camera.h"
#include "../utillity/ColorUtility.h"


ParticleManager* ParticleManager::m_pInstance = nullptr;

void ParticleManager::Initialze(
    UINT         maxParticle,  //パーティクル最大数
    iex2DObj*    pTexture,     //パーティクルテクスチャ
    UINT         TextureCellX, //テクスチャコマ数Ｘ
    UINT         TextureCellY  //テクスチャコマ数Ｘ
    )
{
    MyAssert(m_pInstance == nullptr, "インスタンスの二重初期化が行われました");

    m_pInstance = new ParticleManager(
        maxParticle,
        pTexture,
        TextureCellX,
        TextureCellY
        );
}

ParticleManager& ParticleManager::GetInstance()
{
    MyAssert(m_pInstance, "インスタンスがない状態でParticleManager::GetInstanceが呼ばれました");

    return *m_pInstance;
}

void ParticleManager::Release()
{
    MyAssert(m_pInstance, "インスタンスがない状態でParticleManager::Releaseが呼ばれました");

    delete m_pInstance;
    m_pInstance = nullptr;
}

//パーティクルセット
void ParticleManager::Set(
    UINT        type,
    CrVector3   pos,
    CrVector3   move,
    CrVector3   power,
    CrVector2   size,

    UINT        md_frame,
    UINT        end_frame,

    DWORD       start_color,
    DWORD       md_color,
    DWORD       end_color,
    DWORD       dw_flag      //描画フラグ
    )
{
    const int iMax = (int)m_MaxParticle;
    ParticleData* it;

    for (int i = 0; i < iMax; ++i)
    {
        it = &m_ParticleDataArray[m_SetCount];

        if (it->count >= it->end_frame)
        {
            m_SetCount = (m_SetCount + 1) % iMax;
            continue;
        }

        it->type = type;
        it->pos = pos;
        it->move = move;
        it->power=power;
        it->size = size;

        it->md_frame = md_frame;
        it->end_frame = end_frame;

        it->start_color = start_color;
        it->md_color = md_color;
        it->end_color = end_color;

        it->dw_flag = dw_flag;

        return;
    }

    MyAssert(false, "パーティクルが出現最大数に達しました 現在の数 = %d ", iMax);
}


//パーティクルセット
void ParticleManager::Set(
    UINT        type,
    CrVector3   pos,
    CrVector3   move,
    CrVector3   power,
    CrVector2   size,
    UINT        end_frame,
    DWORD       color,
    DWORD       dw_flag
    )
{
    Set(
        type,
        pos,
        move,
        power,
        size,
        end_frame / 2,
        end_frame,
        color,
        color,
        color,
        dw_flag
        );
}


//描画
void ParticleManager::Render()
{
    
    ParticleData* it;

    const Vector3 R = DefCamera.GetRight();
    const Vector3 U = DefCamera.GetUp();
    
    LVERTEX v[4];

    const int iMax = (int)m_MaxParticle;
    RATIO t;

    const Vector2 TexCell(1.0f / (float)m_TextureCellX, 1.0f / (float)m_TextureCellY);

    //パーティクルをソート
    ParticleSort();


    for (int i = 0; i < iMax; ++i)
    {
        it = m_pWorkParticleDataPointerArray[i];

        //生存確認
        if (it->count >= it->end_frame)
        {
            continue;
        }

        {
            //場所セット
            v[0].pos = R * -it->size.x + U*it->size.y + it->pos;
            v[1].pos = R *  it->size.x + U*it->size.y + it->pos;
            v[2].pos = R * -it->size.x + -U*it->size.y + it->pos;
            v[3].pos = R *  it->size.x + -U*it->size.y + it->pos;
        }

        {
            //色の補間
            if (it->count < it->md_frame)
            {
                t = (float)it->count / (float)it->md_frame;

                v[0].color = ColorLerp(it->start_color, it->md_color, t);
            }
            else
            {
                t = (float)(it->count - it->md_frame) / (float)(it->end_frame - it->md_frame);

                v[0].color = ColorLerp(it->md_color, it->end_color, t);
            }

            //各頂点にセット
            v[1].color = v[2].color = v[3].color = v[0].color;
        }


        //テクスチャ座標を計算
        {
            //左上
            v[0].tu = (float)(it->count%m_TextureCellX)*TexCell.x;
            v[0].tv = (float)(it->count/m_TextureCellX)*TexCell.y;

            //その他
            v[1].tu = v[0].tu + TexCell.x;
            v[1].tv = v[0].tv;

            v[2].tu = v[0].tu;
            v[2].tv = v[0].tv + TexCell.y;

            v[3].tu = v[1].tu;
            v[3].tv = v[2].tv;
        }

        iexPolygon::Render3D(
            v,
            2,
            m_pTexture,
            it->dw_flag
            );
    }
}


//更新
void ParticleManager::Update()
{
    const int iMax = (int)m_MaxParticle;
    ParticleData* it;

    for (int i = 0; i < iMax; ++i)
    {
        it = &m_ParticleDataArray[i];

        if (it->count >= it->end_frame)
        {
            continue;
        }

        it->count += 1;
        it->move += it->power;
        it->pos += it->move;
    }
}



ParticleManager::ParticleManager(
    UINT         maxParticle,  //パーティクル最大数
    iex2DObj*    pTexture,     //パーティクルテクスチャ
    UINT         TextureCellX, //テクスチャコマ数Ｘ
    UINT         TextureCellY  //テクスチャコマ数Ｘ
    ) :
    m_MaxParticle(maxParticle),
    m_pTexture(pTexture),
    m_TextureCellX(TextureCellX),
    m_TextureCellY(TextureCellY),
    m_SetCount(0)
{
    m_ParticleDataArray = new ParticleData[maxParticle];
    m_pWorkParticleDataPointerArray = new ParticleData*[maxParticle];
}


ParticleManager::~ParticleManager()
{
    delete[] m_ParticleDataArray;
    delete[] m_pWorkParticleDataPointerArray;
}

//Z値比較用関数
static int CompareParticleZFunc(const void*p1, const void* p2)
{
    return ((*(ParticleManager::ParticleData*)p1).cameraZ > (*(ParticleManager::ParticleData*)p2).cameraZ) ? (-1) : (1);
}


void ParticleManager::ParticleSort()
{
    const int iMax = (int)m_MaxParticle;
    ParticleData* it;

    for (int i = 0; i < iMax; ++i)
    {
        it = &m_ParticleDataArray[i];

        //Z値を計算する
        it->cameraZ = DefCamera.GetCameraZ(it->pos);


        m_pWorkParticleDataPointerArray[i] = &m_ParticleDataArray[i];
    }

    //遠い順番にソート
    std::qsort(
        &m_pWorkParticleDataPointerArray[0],
        m_MaxParticle,
        sizeof(ParticleData*),
        CompareParticleZFunc
        );
}