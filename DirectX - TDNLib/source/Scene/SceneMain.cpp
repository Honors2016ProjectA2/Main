#include	"TDNLIB.h"
#include	"../system/Framework.h"
#include	"../system/System.h"
#include	"SceneMain.h"
#include	"../Sound/SoundManager.h"
#include "AI\NPC\PersonManager.h"
#include "GossipRipple\GossipRipple.h"
#include "GossipRipple\GossipRippleManager.h"

//GossipRipple* rip;

iex3DObj *airou2;

iexMesh* school;

//******************************************************************
//		初期化・解放
//******************************************************************
bool sceneMain::Initialize()
{
	tdnView::Init();
	tdnMouse::Initialize(TRUE);
	tdnView::Set(Vector3(.0f,65.0f,-70.0f), Vector3(.0f, .0f, .0f));


	//rip = new GossipRipple(RIPPLE_TYPE::RED, Vector3(-20, 0, 20));

	GossipRippleMgr;


	airou2 = new iex3DObj("DATA/CHR/airou_toire.IEM");
	airou2->SetScale(1.0f);

	PersonMgr;
	PersonMgr.AddPerson(PERSON_TYPE::RED,Vector3(30, 10, 30));
	PersonMgr.AddPerson(PERSON_TYPE::RED, Vector3(10, 10, 30));
	PersonMgr.AddPerson(PERSON_TYPE::RED, Vector3(-10, 10, 30));
	PersonMgr.AddPerson(PERSON_TYPE::BLUE,Vector3(-30, 10, -20));
	PersonMgr.AddPerson(PERSON_TYPE::RED, Vector3(-30, 10, 30));

	school = new iexMesh("Data/Stage/school.imo");
	school->SetScale(0.8f);
	school->SetAngle(PI);
	school->Update();

	return true;
}

sceneMain::~sceneMain()
{
	delete airou2;
	//delete rip;
	GossipRippleMgr.Release();

	PersonMgr.Reset();
	PersonMgr.Release();
	
	delete school;

	
	SoundManager::Release();
}


//******************************************************************
//		更新・描画
//******************************************************************
bool sceneMain::Update()
{
	tdnMouse::Update();

	if (KeyBoard('Z')==1)
	{
	//	rip->Action();
		float ramX = (rand() % 100)-50;
		float ramZ = (rand() % 100)-50;

		GossipRippleMgr.AddRipple(RIPPLE_TYPE::RED, Vector3(ramX, 0, ramZ));
	}
	if (KeyBoard('X') == 1)
	{

		PersonMgr.GetPerson(0)->ActionGossipRipple();
	}
	
	airou2->Animation();
	airou2->Update();

	//rip->Update();
	GossipRippleMgr.Update();


	PersonMgr.Update();

	return true;	
}

void sceneMain::Render()
{
	tdnView::Activate();
	tdnView::Clear(0xff005522);

	airou2->Render();
	
	school->Render();

	//rip->Render();
	GossipRippleMgr.Render();

	PersonMgr.Render();

	tdnText::Draw(150, 0, 0xfffffffff, "シーンメイン");

}
