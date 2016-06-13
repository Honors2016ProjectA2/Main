#include	"TDNLIB.h"
#include	"system/Framework.h"
#include	"sceneLoading.h"

//=============================================================================================
//		グ	ロ	ー	バ	ル	領	域
bool sceneLoading::isThread;


//=============================================================================================
//		初	期	化	と	開	放
bool sceneLoading::Initialize(BaseScene *newScene)
{
	m_pImage = new tdn2DObj("DATA/UI/loading.png");
	m_AnimeCount = m_AnimeFrame = 0;

	//	別スレッド作成
	//次のシーンのポインタは後で使うのでnewSceneに保存しておく。
	m_newScene = newScene;

	isThread = true;
	_beginthread(Thread_funk, 0, (void*)m_newScene);

	return true;
}

sceneLoading::~sceneLoading()
{
	delete m_pImage;
}
//=============================================================================================


//=============================================================================================
//		ス	レ	ッ	ド	処	理
void sceneLoading::Thread_funk(void *arg)
{
	BaseScene* scene = (BaseScene*)arg;

	scene->Initialize();	// ★ここで本体のsceneが必要な時間をかかる初期化を行う
	scene->m_bLoad = true;	// この時点でロードフラグをON。シーンを切り替える際の読み込みをしないようにする

	//	スレッド終了処理
	isThread = false;
	_endthread();
}


//
//=============================================================================================

//=============================================================================================
//		更			新
bool sceneLoading::Update()
{
	// 画像アニメーション処理
	if (++m_AnimeFrame > m_AnimeSpeed)
	{
		m_AnimeFrame = 0;
		if (++m_AnimeCount > 5) m_AnimeFrame = 0;

	}

	//ロードが終わったら、シーンをチェンジ
	if (!isThread)
	{
		MainFrame->ChangeScene(m_newScene);
	}

	return true;
}
//
//=============================================================================================


//=============================================================================================
//		描			画
void sceneLoading::Render()
{
	tdnView::Activate();
	tdnView::Clear();

	tdnPolygon::Rect(0, 0, tdnSystem::GetScreenSize().right, tdnSystem::GetScreenSize().bottom , RS::COPY, 0xffffffff);
	m_pImage->Render(800, 620, 512, 64, 0, m_AnimeCount * 64, 512, 64);
}
//
//=============================================================================================