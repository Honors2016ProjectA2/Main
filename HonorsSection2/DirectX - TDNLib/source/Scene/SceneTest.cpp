#include	"TDNLIB.h"
#include	"../system/Framework.h"
#include	"../system/System.h"
#include	"SceneTest.h"
#include	"../Sound/SoundManager.h"
#include   "Effect\AnimationUV.h"
#include "Number\Number.h"

AnimationUV* animUV;

Vector3 cameraPos = Vector3(0, 10, -40);

Surface* backbuffer;
tdn2DObj* screen;

tdn2DObj* m_Effect;

tdn2DAnim* anim;
tdn2DObj* anim2;
//tdn2DAnim* d;
//tdn2DAnim* d2;


tdn2DAnim* app[4];

Number* num;

Number* num2;

Number* num3;

tdn2DObj* blind;
tdn2DObj* blindScreen;
//******************************************************************
//		初期化・解放
//******************************************************************
bool sceneTest::Initialize()
{
	tdnView::Init();

	BG = new tdn2DObj("Data/BG.png");

	player = new iex3DObj("Data/player/nazenaraba_toire.IEM");	
	
	stage = new iexMesh("Data/Stage/yama2.imo");
	stage->SetAngle(3.14f);
	stage->SetScale(0.4f);
	stage->SetPos(Vector3(0, 0, 20));
	stage->Update();

	tdnSystem::GetDevice()->GetRenderTarget(0, &backbuffer);
	screen = new tdn2DObj(1280, 720, FMT2D::RENDERTARGET);


	m_Effect = new tdn2DObj("Data/Effect/nodeEffect.png");

	animUV = new AnimationUV("DATA/UV/wave.imo", 0.00f, 0.01f, 60, false, 1, 45);

	anim = new tdn2DAnim("Data/114514.png");
	anim->OrderShake(12, 0, 20, 4);
	anim->OrderJump(4, 1.0f, 0.5f);
	anim->OrderShrink(12, 1.0f, 3.0f);

	//d = new tdn2DAnim("Data/youmu.png");
	//d->OrderRipple(12, 1, 0.02f);
	//d2 = new tdn2DAnim("Data/youmu.png");
	//d2->OrderRipple(12, 1, 0.02f);
	
	for (int i = 0; i < 4; i++)
	{
		app[i] = new tdn2DAnim("Data/select.png");
		app[i]->OrderMoveAppeared(12, 600, i * 128);

	}

	anim2= new tdn2DObj("Data/youmu.png");
	//anim->OrderRipple(12, 1, 0.02f);
	
	num = new Number();
	num2 = new Number();

	num3 = new Number("Data/Number/number3.png", 128);

	NumberEffect;

	blind = new tdn2DObj("DATA/UI/blind.png");
	shader2D->SetValue("BlindTex", blind);

	blindScreen = new tdn2DObj(512, 512, TDN2D::RENDERTARGET);
	m_blindSplit=4;
	m_blindAngle=0.0f;
	maskScreen = new tdn2DObj(512, 512, TDN2D::RENDERTARGET);

	return true;
}

sceneTest::~sceneTest()
{
	SAFE_DELETE(BG);
	SAFE_DELETE(player);
	SAFE_DELETE(stage);

	SAFE_DELETE(m_Effect);

	SAFE_DELETE(animUV);
	SAFE_DELETE(anim);

	SAFE_DELETE(anim2);

	SAFE_DELETE(screen);

	for (int i = 0; i < 4; i++)
	{
		SAFE_DELETE(app[i]);
	}
	//SoundManager::Release();

	SAFE_DELETE(num);
	SAFE_DELETE(num2);
	SAFE_DELETE(num3);

	NumberEffect.Release();


}


static int HOGE_NUM = 0;
static int HOGE_NUM2 = 0;
//******************************************************************
//		更新・描画
//******************************************************************
bool sceneTest::Update()
{

	// SoundManager::Update();
	if (KeyBoard(KB_F))cameraPos.z += 1;
	if (KeyBoard(KB_G))cameraPos.z -= 1;
	tdnView::Set(cameraPos, Vector3(0,10,0));
	shader->SetValue("VMatrix", matView);

	player->SetAngle(3.14f);
	player->SetPos(Vector3(10.0f, 0.0f, 0.0f));
	player->Update();
	player->Animation();
	
	if (KeyBoard(KB_B) == 3)
	{
		animUV->Action();
	}
	animUV->Update();

	if (KeyBoard(KB_Z) == 3)
	{
		anim->Action();
		//d->Action(12);
		//d2->Action(24);
	}
	if (KeyBoard(KB_X) == 3)
	{
		for (int i = 0; i < 4; i++)
		{
			app[i]->OrderMoveAppeared(12, 600,  128);
			app[i]->Action(i * 20);
		}
		
	}
	if (KeyBoard(KB_C) == 3)
	{
		for (int i = 0; i < 4; i++)
		{
			app[i]->OrderRipple(12, 1.0f, 0.1f);
			app[i]->Action();
		}

	}
	if (KeyBoard(KB_N) == 3)
	{
		num->Action();
		HOGE_NUM += 100;
	}
	if (KeyBoard(KB_M) == 3)
	{
		num->Action();
		HOGE_NUM += 1600;
	}
	if (KeyBoard(KB_K) == 3)
	{
		num3->Action();
		num->Action();
		HOGE_NUM += 240;
	}
	if (KeyBoard(KB_I) == 3)
	{
		num3->Action();
		num->Action();
		HOGE_NUM += 1;
	}
	if (KeyBoard(KB_L) == 3)
	{
		for (int i = 0; i < 5; i++)
		{
			int randX = (rand() % 100) + 250;
			int randY = (rand() % 500);
			int score = rand() % 1000;

			NumberEffect.AddNumber(randX, randY, score);
		}	

	}
	if (KeyBoard(KB_P) == 3) 
	{
		NumberEffect.AddNumber(500, 400, 114);
	}
	if (KeyBoard(KB_O) == 3)
	{
		num3->GetAnim()->OrderShrink(6, 1.0f, 2.0f);
		num3->Action();
		
	}

	if (HOGE_NUM2 < HOGE_NUM)
	{
		// 桁が3以上離れてると

		//if ()
		//{
		//	HOGE_NUM2 += 100;
		//}
		//else
		//{
			HOGE_NUM2 += 21;
		//}		

	}
	else
	{
		HOGE_NUM2 = HOGE_NUM;
	}

	anim->Update();
	
	//d->Update();
	//d2->Update();

	for (int i = 0; i < 4; i++)
	{
		app[i]->Update();
	}

	num->Update();
	num2->Update();
	num3->Update();

	NumberEffect.Update();

	return true;	
}

void sceneTest::Render()
{
	tdnView::Activate();


	BG->Render(0, 0, RS::COPY_NOZ);
	//BG->Render(0, 0, RS::ADD);

	// マスクせいせい
	MaskRender();

	screen->RenderTarget();
	tdnView::Clear(0xff005522);

	player->Render(shader, "copy");
	stage->Render();

	m_Effect->SetScale(10);
	m_Effect->Render3D(0,0,0);

	animUV->Render();
	
	anim2->Render(100, 90);
	anim->Render(500, 90);
	//d->Render(300, 90, RS::ADD);
	//d2->Render(600, 90, RS::ADD);

	for (int i = 0; i < 4; i++)
	{
		app[i]->Render(0, i * 128 , 256, 128, 0, i * 128, 256, 128);
	}


	tdnSystem::GetDevice()->SetRenderTarget(0, backbuffer);
	//screen->Render(0, 0,shader2D ,"blind");
	screen->Render(0, 0, shader2D, "blind");
	

	//blindScreen->Render(900, 100,256,256,0,0,256,256,shader2D, "blind");

	static float BlindRete = 1.0f;
	if (KeyBoard(KB_Q))
	{
		BlindRete -= 0.1f;
	}
	if (KeyBoard(KB_E))
	{
		BlindRete += 0.1f;
	}
	BlindRete = Math::Clamp(BlindRete, 0.0f, 1.0f);
	shader2D->SetValue("blindRate", BlindRete);
	tdnText::Draw(900, 100, 0xffffffff, "BlindRete->%.2f", BlindRete);

	maskScreen->Render(0, 0);

	// 数字描画
	num->Render(500, 100, HOGE_NUM2);

	num2->Render(500, 500, 114);

	num3->Render(1100, 20, HOGE_NUM);
	NumberEffect.Render();


	if (tdnInput::KeyGet(KEY_A) == 1) tdnText::Draw(10, 340, 0xffffff00, "Input : A");
	if (tdnInput::KeyGet(KEY_B) == 1) tdnText::Draw(10, 360, 0xffffff00, "Input : B");
	if (tdnInput::KeyGet(KEY_C) == 1) tdnText::Draw(10, 380, 0xffffff00, "Input : C");
	if (tdnInput::KeyGet(KEY_D) == 1) tdnText::Draw(10, 400, 0xffffff00, "Input : D");
	if (tdnInput::KeyGet(KEY_L1) == 1) tdnText::Draw(10, 420, 0xffffff00, "Input : L1");
	if (tdnInput::KeyGet(KEY_L2) == 1) tdnText::Draw(10, 440, 0xffffff00, "Input : L2");
	if (tdnInput::KeyGet(KEY_L3) == 1) tdnText::Draw(10, 460, 0xffffff00, "Input : L3");
	if (tdnInput::KeyGet(KEY_R1) == 1) tdnText::Draw(10, 480, 0xffffff00, "Input : R1");
	if (tdnInput::KeyGet(KEY_R2) == 1) tdnText::Draw(10, 500, 0xffffff00, "Input : R2");
	if (tdnInput::KeyGet(KEY_R3) == 1) tdnText::Draw(10, 520, 0xffffff00, "Input : R3");
	if (tdnInput::KeyGet(KEY_START) == 1) tdnText::Draw(10, 540, 0xffffff00, "Input : START");
	if (tdnInput::KeyGet(KEY_SELECT) == 1) tdnText::Draw(10, 560, 0xffffff00, "Input : SELECT");
	if (tdnInput::KeyGet(KEY_LEFT) == 1) tdnText::Draw(10, 580, 0xffffff00, "Input : LEFT");
	if (tdnInput::KeyGet(KEY_RIGHT) == 1) tdnText::Draw(10, 580, 0xffffff00, "Input : RIGHT");
	if (tdnInput::KeyGet(KEY_UP) == 1) tdnText::Draw(10, 600, 0xffffff00, "Input : UP");
	if (tdnInput::KeyGet(KEY_DOWN) == 1) tdnText::Draw(10, 600, 0xffffff00, "Input : DOWN");
}

void sceneTest::MaskRender()
{

	if (KeyBoard(KB_T)==3)
	{
		m_blindSplit++;
	}
	if (KeyBoard(KB_R) == 3)
	{
		m_blindSplit--;
	}
	if (KeyBoard(KB_Y) == 3)
	{
		m_blindAngle -= 0.1f;
	}
	if (KeyBoard(KB_U) == 3)
	{
		m_blindAngle += 0.1f;
	}

	maskScreen->RenderTarget();
	tdnView::Clear();

	// 保管しない
	tdnRenderState::Filter(false);

	// ブラインド
	blind->SetScale(1.5f);
	blind->SetAngle(m_blindAngle);
	blind->Render(0, 0, 512, 512, 0, 0, 256 * m_blindSplit, 256 * m_blindSplit);
	

	tdnRenderState::Filter(true);

	// ↑で描画した絵をマスク情報に	
	shader2D->SetValue("BlindTex", maskScreen);



}
