#include	"TDNLIB.h"
#include	"../system/Framework.h"
#include	"../system/System.h"
#include	"SceneMain.h"
#include	"../Sound/SoundManager.h"
#include "AI\NPC\PersonManager.h"
#include "GossipRipple\GossipRipple.h"
#include "GossipRipple\GossipRippleManager.h"
#include "../Part/Part.h"
#include "../Stage/PersonLoader.h"

#include "Animation\AnimationUV.h"


//GossipRipple* rip;

iexMesh* school;
int RippleCount;
PartManager *part_mgr;


//******************************************************************
//		初期化・解放
//******************************************************************
bool sceneMain::Initialize()
{
	tdnView::Init();
	tdnMouse::Initialize(TRUE);
	tdnView::Set(Vector3(.0f,75.0f,-60.0f), Vector3(.0f, .0f, .0f));

	//rip = new GossipRipple(RIPPLE_TYPE::RED, Vector3(-20, 0, 20));

	GossipRippleMgr;

	RippleCount = 3;	// 仮で3
	part_mgr = new PartManager;


	PersonLoader::LoadPerson(0);
	//PersonMgr;
	//PersonMgr.AddPerson(PERSON_TYPE::RED,Vector3(30, 10, 30));
	//PersonMgr.AddPerson(PERSON_TYPE::RED, Vector3(10, 10, 30));
	//PersonMgr.AddPerson(PERSON_TYPE::RED, Vector3(-10, 10, 30));
	//PersonMgr.AddPerson(PERSON_TYPE::BLUE,Vector3(-30, 10, -20));
	//PersonMgr.AddPerson(PERSON_TYPE::RED, Vector3(-30, 10, 30));

	school = new iexMesh("Data/Stage/school.imo");
	school->SetScale(0.8f);
	school->SetAngle(PI);
	school->Update();

	return true;
}

sceneMain::~sceneMain()
{
	//delete rip;
	GossipRippleMgr.Release();

	PersonMgr.Reset();
	PersonMgr.Release();
	
	delete school;
	delete part_mgr;
	
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
	//if (KeyBoard('X') == 1)
	if (tdnMouse::GetLeft() == 3 && RippleCount > 0)
	{
		FOR(PersonMgr.GetPersonSize())
		{
			if ((Math::WorldToScreen(PersonMgr.GetPerson(i)->GetPos()) - tdnMouse::GetPos()).Length() < 50)
			{
				RippleCount--;
				PersonMgr.GetPerson(i)->ActionGossipRipple();
			}
		}
		//PersonMgr.GetPerson(0)->ActionGossipRipple();
	}

	part_mgr->Update();
	if (RippleCount == 0)
	{
		part_mgr->ChangePart(PartManager::PART::GAME_OVER);
	}
	

	//rip->Update();
	GossipRippleMgr.Update();


	PersonMgr.Update();

	
	return true;	
}

void sceneMain::Render()
{
	tdnView::Activate();
	tdnView::Clear(0xff005522);

	
	school->Render();

	//rip->Render();
	GossipRippleMgr.Render();

	PersonMgr.Render();

	tdnText::Draw(150, 0, 0xffffffff, "シーンメイン");

	tdnText::Draw(320, 30, 0xffffffff, "残り回数 : %d", RippleCount);

	part_mgr->Render();
}
