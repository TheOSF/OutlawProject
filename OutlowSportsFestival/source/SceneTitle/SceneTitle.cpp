
#include    "SceneTitle.h"
#include    "../Sound/Sound.h"
#include    "../GameSystem/GameController.h"
#include    "../IexSystem/Framework.h"
#include    "../SceneOption/SceneOption.h"

SceneTitle::SceneTitle()
{
    

}

SceneTitle::~SceneTitle()
{

}

void SceneTitle::Update()
{
    //�I�v�V�����Z���N�g��
    MainFrame->ChangeScene(new SceneOption());
}

void SceneTitle::Render()
{

}