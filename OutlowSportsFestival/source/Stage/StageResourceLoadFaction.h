#pragma once

class StageResourceLoadFaction
{
public:
    //�X�e�[�W�P�̃��b�V�������\�[�X�ɓo�^
    static void LoadStage1Object();
    static void ReleaseStage1Object();

    //�X�e�[�W�Q�̃��b�V�������\�[�X�ɓo�^
    static void LoadStage2Object();
    static void ReleaseStage2Object();

private:

    static void LoadCommonObject();
    static void ReleaseCommonObject();
    
};