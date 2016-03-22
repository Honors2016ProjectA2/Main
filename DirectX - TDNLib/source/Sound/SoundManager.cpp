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

	if (KeyBoardTRG(KB_1))
	{
		play_manager->SetFX(DXA_FX::DXAFX_CHORUS);
	}
	else if (KeyBoardTRG(KB_2))
	{
		play_manager->SetFX(DXA_FX::DXAFX_COMPRESSOR);
	}
	else if (KeyBoardTRG(KB_3))
	{
		play_manager->SetFX(DXA_FX::DXAFX_DISTORTION);
	}
	else if (KeyBoardTRG(KB_4))
	{
		play_manager->SetFX(DXA_FX::DXAFX_ECHO);
	}
	else if (KeyBoardTRG(KB_5))
	{
		play_manager->SetFX(DXA_FX::DXAFX_ENVREVERB);
	}
	else if (KeyBoardTRG(KB_6))
	{
		play_manager->SetFX(DXA_FX::DXAFX_FLANGER);
	}
	else if (KeyBoardTRG(KB_7))
	{
		play_manager->SetFX(DXA_FX::DXAFX_GARGLE);
	}
	else if (KeyBoardTRG(KB_8))
	{
		play_manager->SetFX(DXA_FX::DXAFX_PARAMEQ);
	}
	else if (KeyBoardTRG(KB_9))
	{
		//play_manager->SetFX(DXA_FX::DXAFX_WAVESREVERB);
	}
	else if (KeyBoardTRG(KB_0))
	{
		play_manager->SetFX(DXA_FX::DXAFX_OFF);
	}
}
//
//=============================================================================================



//=============================================================================================
//		処		理
int SE_Manager::Play_in(int data_num, bool loop)
{
	if (data_num != -1)
	{
		return play_manager->Play(data_num, loop);
	}
	return -1;
}

int SE_Manager::Play_in(int data_num, float volume, bool loop)
{
	if (data_num != -1)
	{
		play_manager->SetVolume(data_num, volume);
		return play_manager->Play(data_num, loop);
	}
	return -1;
}

int SE_Manager::Play_in(int data_num, const Vector3 &pos, const Vector3 &front, const Vector3 &move, bool loop)
{
	if (data_num != -1)
	{
		return play_manager->Play(data_num, pos, front, move, loop);
	}
	return -1;
}

int SE_Manager::Play(LPSTR _ID, bool loop)
{
	return Play_in(ID[_ID], loop);
}

int SE_Manager::Play(LPSTR _ID, float volume, bool loop)
{
	if (volume < 0) volume = 0;
	else if (volume > 1.0f) volume = 1.0f;

	return Play_in(ID[_ID], volume, loop);
}

int SE_Manager::Play(LPSTR _ID, const Vector3 &pos, const Vector3 &front, const Vector3 &move, bool loop)
{
	return Play_in(ID[_ID], pos, front, move, loop);
}

void SE_Manager::Stop(LPSTR _ID, int no)
{
	play_manager->Stop(ID[_ID], no);
}

void SE_Manager::Stop_all()
{
	play_manager->AllStop();
}

void SE_Manager::Set_listener(const Vector3 &pos, const Vector3 &front, const Vector3 &up, const Vector3 &move)
{
	play_manager->SetListenerAll(pos, front, up, move);
}
//
//=============================================================================================




//=============================================================================================
//		ＩＤ一致データ検索
//int	SE_Manager::Find_data_no(LPSTR _ID)
//{
//	int	result = -1;
//	//	データ数分ループ
//	for (int i = 0; i < data_count; i++)
//	{
//		//	判定
//		if (strcmp(all_dataS[i].id, _ID) != 0) continue;
//
//		//	発見
//		result = i;
//		//	ループを抜ける
//		break;
//	}
//
//	return	result;
//}
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

	if (KeyBoardTRG(KB_1))
	{
		effect_no = (int)DXA_FX::DXAFX_CHORUS;
		play_manager->SetFX((DXA_FX)effect_no);
	}
	else if (KeyBoardTRG(KB_2))
	{
		effect_no = (int)DXA_FX::DXAFX_COMPRESSOR;
		play_manager->SetFX((DXA_FX)effect_no);
	}
	else if (KeyBoardTRG(KB_3))
	{
		effect_no = (int)DXA_FX::DXAFX_DISTORTION;
		play_manager->SetFX((DXA_FX)effect_no);
	}
	else if (KeyBoardTRG(KB_4))
	{
		effect_no = (int)DXA_FX::DXAFX_ECHO;
		play_manager->SetFX((DXA_FX)effect_no);
	}
	else if (KeyBoardTRG(KB_5))
	{
		effect_no = (int)DXA_FX::DXAFX_ENVREVERB;
		play_manager->SetFX((DXA_FX)effect_no);
	}
	else if (KeyBoardTRG(KB_6))
	{
		effect_no = (int)DXA_FX::DXAFX_FLANGER;
		play_manager->SetFX((DXA_FX)effect_no);
	}
	else if (KeyBoardTRG(KB_7))
	{
		effect_no = (int)DXA_FX::DXAFX_GARGLE;
		play_manager->SetFX((DXA_FX)effect_no);
	}
	else if (KeyBoardTRG(KB_8))
	{
		effect_no = (int)DXA_FX::DXAFX_PARAMEQ;
		play_manager->SetFX((DXA_FX)effect_no);
	}
	else if (KeyBoardTRG(KB_9))
	{
		effect_no = (int)DXA_FX::DXAFX_WAVESREVERB;
		play_manager->SetFX((DXA_FX)effect_no);
	}
	else if (KeyBoardTRG(KB_0))
	{
		effect_no = 0;
		play_manager->SetFX(DXA_FX::DXAFX_OFF);
	}
}
//
//=============================================================================================



//=============================================================================================
//		処		理
void BGM_Manager::Play_in(int data_num, bool loop)
{
	if (data_num != -1)
	{
		play_manager->Play(data_num, loop);
	}
}

void BGM_Manager::Play_in(int data_num, float volume, bool loop)
{
	if (data_num != -1)
	{
		play_manager->SetVolume(data_num, volume);
		play_manager->Play(data_num, loop);
	}
}

void BGM_Manager::Play_in(int data_num, const Vector3 &pos, const Vector3 &front, const Vector3 &move, bool loop)
{
	if (data_num != -1)
	{
		play_manager->Play(data_num, pos, front, move, loop);
	}
}

void BGM_Manager::Play(LPSTR _ID, bool loop)
{
	Play_in(ID[_ID], loop);
}

void BGM_Manager::Play(LPSTR _ID, float volume, bool loop)
{
	if (volume < 0) volume = 0;
	else if (volume > 1.0f) volume = 1.0f;

	Play_in(ID[_ID], volume, loop);
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

void BGM_Manager::Set_listener(const Vector3 &pos, const Vector3 &front, const Vector3 &up, const Vector3 &move)
{
	play_manager->SetListenerAll(pos, front, up, move);
}
//
//=============================================================================================




//=============================================================================================
//		ＩＤ一致データ検索
//int	BGN_Manager::Find_data_no(LPSTR _ID)
//{
//	int	result = -1;
//	//	データ数分ループ
//	for (int i = 0; i < data_count; i++)
//	{
//		//	判定
//		if (strcmp(all_dataB[i].id, _ID) != 0) continue;
//
//		//	発見
//		result = i;
//		//	ループを抜ける
//		break;
//	}
//
//	return	result;
//}
//
//=============================================================================================


//=============================================================================================
//		実		体
SE_Manager *se;
BGM_Manager *bgm;
//
//=============================================================================================