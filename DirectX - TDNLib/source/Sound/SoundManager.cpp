#include	"TDNLIB.h"
#include	"SoundManager.h"

//**************************************************************************************************************
//
//		サウンド管理クラス(winmainとframeworkで使うだけ)
//
//**************************************************************************************************************
void SoundManager::Initialize()
{
	se = new SE_Manager;
	se->Initialize();
	bgm = new BGM_Manager;
	bgm->Initialize();
}

void SoundManager::Release()
{
	delete se;
	delete bgm;
}

void SoundManager::Update()
{
	se->Update();
	bgm->Update();
}


//**************************************************************************************************************
//
//		SE管理クラス
//
//**************************************************************************************************************
//*********************************************************************************************
//		パラメータの設定
//*********************************************************************************************
//	サウンドデータ(textで読み込むのも良いかもしれない)
SE_Manager::DATA all_dataS[] =
{
	{ "イエアア", "DATA/Sound/SE/yeaa.wav", 20, false },
	{ "END", nullptr }
};


//=============================================================================================
//		初	期	化
void SE_Manager::Initialize()
{
	play_manager = new tdnSoundSE;

	for (int i = 0;;i++)
	{
		if (strcmp(all_dataS[i].id, "END") == 0) break;	// 終端

		ID[all_dataS[i].id] = i;
		play_manager->Set(i, all_dataS[i].play_simultaneously, all_dataS[i].file_name, all_dataS[i].b3D);
	}

	// リスナーの初期設定
	play_manager->SetListenerAll(
		Vector3(tdnSystem::GetScreenSize().right * .5f, tdnSystem::GetScreenSize().bottom * .5f, 0),	// リスナー座標(画面の真ん中と仮定する)
		Vector3(1, 0, 0),	// 正面ベクトル
		Vector3(0, 1, 0),	// 上方ベクトル
		Vector3(0, 0, 0)	// 移動値
		);
}
//
//=============================================================================================



//=============================================================================================
//		解		放
SE_Manager::~SE_Manager()
{
	delete play_manager;
}
//
//=============================================================================================



//=============================================================================================
//		更		新
void SE_Manager::Update()
{
	play_manager->UpdateListener();
}
//
//=============================================================================================



//=============================================================================================
//		処		理
int SE_Manager::Play_in(int data_num, bool loop)
{
	if (data_num != TDNSOUND_PLAY_NONE)
	{
		return play_manager->Play(data_num, loop);
	}
	return TDNSOUND_PLAY_NONE;
}

int SE_Manager::Play_in(int data_num, const Vector2 &pos, const Vector2 &move, bool loop)
{
	if (data_num != TDNSOUND_PLAY_NONE)
	{
		return play_manager->Play(data_num, Vector3(pos.x, pos.y, 0), Vector3(move.x, move.y, 0), loop);
	}
	return TDNSOUND_PLAY_NONE;
}

int SE_Manager::Play(LPSTR _ID, bool loop)
{
	return Play_in(ID[_ID], loop);
}

int SE_Manager::Play(LPSTR _ID, const Vector2 &pos, const Vector2 &move, bool loop)
{
	return Play_in(ID[_ID], pos, move, loop);
}

void SE_Manager::Stop(LPSTR _ID, int no)
{
	play_manager->Stop(ID[_ID], no);
}

void SE_Manager::Stop_all()
{
	play_manager->AllStop();
}

void SE_Manager::SetListener(const Vector2 &pos, const Vector2 &move)
{
	play_manager->SetListenerPos(Vector3(pos.x, pos.y, 0));
	play_manager->SetListenerMove(Vector3(move.x, move.y, 0));
}
//
//=============================================================================================






//**************************************************************************************************************
//
//		BGM管理クラス
//
//**************************************************************************************************************
//*********************************************************************************************
//		パラメータの設定
//*********************************************************************************************
//	サウンドデータ(textで読み込むのも良いかもしれない)
BGM_Manager::DATA all_dataB[] =
{
	{ "EoE_A", "DATA/Sound/BGM/Collision_of_ElementsA.wav", false },
	{ "EoE_B", "DATA/Sound/BGM/Collision_of_ElementsB.wav", false },
	{ "END", nullptr }
};


//=============================================================================================
//		初	期	化
void BGM_Manager::Initialize()
{
	play_manager = new tdnSoundBGM;

	for (int i = 0;;i++)
	{
		if (strcmp(all_dataB[i].id, "END") == 0) break;	// 終端

		ID[all_dataB[i].id] = i;
		play_manager->Set(i, all_dataB[i].file_name, all_dataB[i].b3D);
	}

	// リスナーの初期設定
	play_manager->SetListenerAll(
		Vector3(tdnSystem::GetScreenSize().right * .5f, tdnSystem::GetScreenSize().bottom * .5f, 0),	// リスナー座標(画面の真ん中と仮定する)
		Vector3(1, 0, 0),	// 正面ベクトル
		Vector3(0, 1, 0),	// 上方ベクトル
		Vector3(0, 0, 0)	// 移動値
		);
}
//
//=============================================================================================



//=============================================================================================
//		解		放
BGM_Manager::~BGM_Manager()
{
	delete play_manager;
}
//
//=============================================================================================

//=============================================================================================
//		更		新
void BGM_Manager::Update()
{
	play_manager->Update();
	play_manager->UpdateListener();
}
//
//=============================================================================================



//=============================================================================================
//		処		理
void BGM_Manager::Play_in(int data_num, bool loop)
{
	if (data_num != TDNSOUND_PLAY_NONE)
	{
		play_manager->Play(data_num, loop);
	}
}

void BGM_Manager::Play_in(int data_num, const Vector3 &pos, const Vector3 &front, const Vector3 &move, bool loop)
{
	if (data_num != TDNSOUND_PLAY_NONE)
	{
		play_manager->Play(data_num, pos, front, move, loop);
	}
}

void BGM_Manager::Play(LPSTR _ID, bool loop)
{
	Play_in(ID[_ID], loop);
}

void BGM_Manager::Play(LPSTR _ID, const Vector3 &pos, const Vector3 &front, const Vector3 &move, bool loop)
{
	return Play_in(ID[_ID], pos, front, move, loop);
}

void BGM_Manager::Stop(LPSTR _ID)
{
	play_manager->Stop(ID[_ID]);
}

void BGM_Manager::Stop_all()
{
	play_manager->AllStop();
}

void BGM_Manager::Fade_in(LPSTR _ID, float fade_speed, bool loop)
{
	play_manager->FadeIn(ID[_ID], fade_speed, loop);
}

void BGM_Manager::Fade_out(LPSTR _ID, float fade_speed)
{
	play_manager->FadeOut(ID[_ID], fade_speed);
}

void BGM_Manager::Cross_fade(LPSTR inID, LPSTR outID, float fade_speed, bool loop)
{
	play_manager->CrossFade(ID[inID], ID[outID], fade_speed, tdnSoundBGM::CROSS_FADE_TYPE::END_OF_ETERNITY, loop);
}

void BGM_Manager::Cross_fade(LPSTR inID, LPSTR outID, float in_speed, float out_speed, bool loop)
{
	play_manager->CrossFade(ID[inID], ID[outID], in_speed, out_speed, tdnSoundBGM::CROSS_FADE_TYPE::END_OF_ETERNITY, loop);
}

void BGM_Manager::SetListener(const Vector2 &pos, const Vector2 &move)
{
	play_manager->SetListenerPos(Vector3(pos.x, pos.y, 0));
	play_manager->SetListenerMove(Vector3(move.x, move.y, 0));
}
//
//=============================================================================================



//=============================================================================================
//		実		体
SE_Manager *se;
BGM_Manager *bgm;
//
//=============================================================================================