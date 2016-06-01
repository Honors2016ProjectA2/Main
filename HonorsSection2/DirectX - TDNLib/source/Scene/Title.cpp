#include "TDNLIB.h"
#include "../system/Framework.h"
#include "Title.h"
#include "SceneMain.h"
#include "../Sheep/Sheep.h"
#include	"MousePointer.h"
#include	"../particle_2d/particle_2d.h"
#include "Effect\EffectManager.h"
#include	"../system/FadeCtrl.h"
#include "End.h"
#include "UI\ResultUIManager.h"

End* end;

bool Title::Initialize()
{
	//end = new End();
	//end->Init();
	//end->floorNum = 3;

	//RESULT_UIMNG;

	// パーティクル初期化
	Particle2dManager::Initialize("DATA/Effect/particle.png", 1000, 4, 4);

	FadeControl::Setting(FadeControl::MODE::WHITE_IN, 30.0f);
	tdnView::Init();

	m_pSheepMgr = new SheepManager();
	m_pointer = new MousePointer();

	// イヌ
	m_bDogFlag = false;
	m_dog.pic = new tdn2DObj("DATA/CHR/dog.png");
	m_dog.pos.x = 600.0f;
	m_dog.pos.y = 550.0f;
	m_dog.anim = 0;
	m_dog.animFlame = 0;

	// 小屋
	m_bKoyaFlag = false;
	m_koya.pic = new tdn2DObj("DATA/title/koyaflont.png");
	m_koya.picBack = new tdn2DObj("DATA/title/koyaBack.png");
	m_koya.pos.x = 1050.0f;
	m_koya.pos.y = 200.0f;

	// 背景
	m_BG = new tdn2DObj("Data/GameHaikei.png");
	m_titleLogo= new tdn2DObj("Data/title/titleLogo.png");
	return true;
}

Title::~Title()
{
	delete m_pSheepMgr;
	SAFE_DELETE(m_pointer);
	SAFE_DELETE(m_dog.pic);
	SAFE_DELETE(m_BG);
	SAFE_DELETE(m_titleLogo);
	SAFE_DELETE(m_koya.pic);
	SAFE_DELETE(m_koya.picBack);
	
	//SAFE_DELETE(end);

	Particle2dManager::Release();
	EffectMgr.Release();
	//RESULT_UIMNG.Release();

}

bool Title::Update()
{
	//if (KeyBoard(KB_E) == 3)
	//{
	//	end->Init();
	//}
	//static bool flag = 0;
	//if (end->Update())
	//{
	//	if (flag == 0)
	//	{
	//		RESULT_UIMNG.Action();
	//	}
	//	flag++;
	//}
	//RESULT_UIMNG.Update();

	// パーティクル
	Particle2dManager::Update();
	EffectMgr.Update();

	// フェード
	FadeControl::Update();

	tdnMouse::Update();

	// マウスポインタ―
	m_pointer->Update();

	// レーンを1番下に
	m_pSheepMgr->SetDownLane();
	m_pSheepMgr->Update();

	// いぬ更新
	DogUpdate();
	
	// 小屋
	KoyaVsSheep();

	if (KeyBoardTRG(KB_ENTER))
	{
		// 羊ループ
		for (auto &it : *m_pSheepMgr->Get_list())
		{
			// カーブを掛ける
			it->SetCurve(DIR::UP);
		}
	}


	// ゲーム画面へ
	if (m_bKoyaFlag == true)
	{
		if ( FadeControl::IsEndFade() )
		{
			MainFrame->ChangeScene(new sceneMain());
			return true;
		}
	
	}

	return true;
}

void Title::DogUpdate()
{
	// フラグがたったら
	if (m_bDogFlag == true)
	{
		m_dog.animFlame++;
		if (m_dog.animFlame > 12)
		{
			m_dog.animFlame = 0;
			m_dog.anim++;
			m_dog.anim = m_dog.anim % 2;
		}

		DogVsSheep();
	}
	else
	{
		DogVsMouse();
	}
}

// 当たり判定
void Title::DogVsMouse()
{
	// 距離計算 
	float len=Math::Length(m_dog.pos.x+60, m_dog.pos.y + 60, tdnMouse::GetPos().x, tdnMouse::GetPos().y);
	// 左クリック！！
	if (tdnMouse::GetLeft() == 3)
	{
		if (len < 60)
		{
			m_bDogFlag = true;
			EffectMgr.AddEffect((int)m_dog.pos.x + 60, (int)m_dog.pos.y + 60, EFFECT_TYPE::DOG_EFFECT);

		}
	}
}

void Title::DogVsSheep()
{
	for (auto &it : *m_pSheepMgr->Get_list())
	{
		Vector2 sheepPos;
		sheepPos = *it->Get_pos();

		// 距離計算 
		float len = Math::Length(m_dog.pos.x - 120 , m_dog.pos.y, sheepPos.x, sheepPos.y);
		// 距離で
		if (len < 60)
		{
			// 羊ループ

				// カーブを掛ける
			it->SetCurve(DIR::UP);

		}

	}
}


// 小屋Vs羊
void Title::KoyaVsSheep()
{

	for (auto &it : *m_pSheepMgr->Get_list())
	{
		Vector2 sheepPos;
		sheepPos = *it->Get_pos();

		// 距離計算 
		float len = Math::Length(m_koya.pos.x+120, m_koya.pos.y, sheepPos.x, sheepPos.y);
		// 距離で
		if (len < 160)
		{
			if (m_bKoyaFlag == true)continue;
			// 小屋フラグおｎ
			m_bKoyaFlag = true;
			FadeControl::Setting(FadeControl::MODE::WHITE_OUT, 255 / 12);
		}

	}
}

void Title::Render()
{
	tdnView::Activate();
	tdnView::Clear();
	
	m_BG->Render(0, 0);
	m_titleLogo->Render(0, 0);


	// 小屋後ろ
	m_koya.picBack->Render((int)m_koya.pos.x, (int)m_koya.pos.y);

	m_pSheepMgr->Render();
	// 小屋
	m_koya.pic->Render((int)m_koya.pos.x, (int)m_koya.pos.y);

	// イヌ
	m_dog.pic->Render((int)m_dog.pos.x, (int)m_dog.pos.y, 120, 120, m_dog.anim * 120, 0, 120, 120);
	


	// エフェクトマネージャー
	EffectMgr.Render();
	
	// パーティクル
	Particle2dManager::Render();

	//基本的には最後。説明時のみ説明書の後ろにするので別途
	m_pointer->Render();

	//end->Render();

	//RESULT_UIMNG.Render();
	//Fade
	FadeControl::Render();
}