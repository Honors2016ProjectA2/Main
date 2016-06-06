
#include	"TDNLIB.h"
#include	"SoundManager.h"


//����������������������������������������������������������������������������������������������������������������
//
//								�T�E���h�Ăяo��ID�ꗗ														�@����
//
//����������������������������������������������������������������������������������������������������������������

//==============================================================================================
//				S	E
//==============================================================================================
const SE_Manager::DATA all_dataS[] =
{
	{ "CURSUR",			"DATA/Sound/SE/se1.wav",		3, false },
	{ "CLICK",			"DATA/Sound/SE/se4.wav",		3, false },
	{ "�ӂƂ�",			"DATA/Sound/SE/be_fat.wav",		3, false },
	{ "!"		,		"DATA/Sound/SE/wolf.wav",		3, false },
	{ "�p�N���O",		"DATA/Sound/SE/pakumogu.wav",	2, false },
	{ "���A��HIT",		"DATA/Sound/SE/hit.wav",		2, false },
	{ "���U���g�X�R�A",	"DATA/Sound/SE/score.wav",		1, false },
	{ "���U���g�_��",	"DATA/Sound/SE/score.wav",		1, false },
	{ "DAMAGE",			"DATA/Sound/SE/eat.wav",		20, false },
	{ "RANK_S",			"DATA/Sound/SE/se20.wav",		1, false },
	{ "RANK_A",			"DATA/Sound/SE/se21.wav",		1, false },
	{ "RANK_B",			"DATA/Sound/SE/se20.wav",		1, false },
	{ "RANK_C",			"DATA/Sound/SE/se21.wav",		1, false },
	{ "�r���[���ς��",	"DATA/Sound/SE/arrert.wav",		3, false },
	{ "�q���萶����",	"DATA/Sound/SE/born.wav",		3, false },
	{ "�������r����",	"DATA/Sound/SE/debuosu.wav",	6, false },
	{ "�q������",		"DATA/Sound/SE/grew.wav",		3, false },
	{ "��",				"DATA/Sound/SE/inu.wav",		4, false },
	{ "����߂��r����",	"DATA/Sound/SE/kiramekuhitsujisan.wav", 3, false },
	{ "�r����",			"DATA/Sound/SE/sheep_run.wav",	3, false },
	{ "�������rIN",		"DATA/Sound/SE/fat_in.wav",		3, false },
	{ "�h�A",			"DATA/Sound/SE/door.wav",		3, false },
	{ "BAD",			"DATA/Sound/SE/bad.wav",		1, false },
	{ "GREAT",			"DATA/Sound/SE/great.wav",		1, false },
	{ "PERFECT",		"DATA/Sound/SE/perfect.wav",	1, false },
	{ "�Ă���",			"DATA/Sound/SE/yaketa.wav",		1, false },
	{ "���ł�",			"DATA/Sound/SE/imadesu.wav",	1, false },
	{ "�ߖ�",			"DATA/Sound/SE/himei.wav",		1, false },
	{ "�r�͂�",			"DATA/Sound/SE/sheep_cry.wav",	6, false },
	{ "�h��",			"DATA/Sound/SE/se9.wav",		1, false },
	{ "�h�h��",			"DATA/Sound/SE/se22.wav",		1, false },
};

//==============================================================================================
//				B	G	M
//==============================================================================================
BGM_Manager::DATA all_dataB[] =
{
	//{ "TITLE",	"DATA/Sound/BGM/�Ö��q��.wav", false },
	{ "GO",		"DATA/Sound/BGM/se10.wav", false },
	{ "MAIN",	"DATA/Sound/BGM/main.wav", false },
	{ "RESULT", "DATA/Sound/BGM/ResultBGM.wav", false },
	{ "�z�C�b�X��", "DATA/Sound/SE/whistle.wav", false },
};


//**************************************************************************************************************
//
//		�T�E���h�Ǘ��N���X(winmain��framework�Ŏg������)
//
//**************************************************************************************************************
//=============================================================================================
//		��	��	��
void SoundManager::Initialize()
{
	se = new SE_Manager;
	se->Initialize();
	bgm = new BGM_Manager;
	bgm->Initialize();
}

//=============================================================================================
//		��		��
void SoundManager::Release()
{
	SAFE_DELETE(se);
	SAFE_DELETE(bgm);
}

//=============================================================================================
//		�X�V
void SoundManager::Update()
{
	se->Update();
	bgm->Update();
}


///********************************************************************************************
//		���X�i�[���ݒ�I
void SoundManager::SetListenerPos(const Vector2 &pos)
{
	se->SetListener(pos);
}


//**************************************************************************************************************
//
//		SE�Ǘ��N���X
//
//**************************************************************************************************************


//=============================================================================================
//		��	��	��
void SE_Manager::Initialize()
{
	play_manager = new tdnSoundSE;

	for (int i = 0; i < _countof(all_dataS); i++)
	{
		//if (strcmp(all_dataS[i].id, "END") == 0) break;	// �I�[

		ID[all_dataS[i].id] = i;
		play_manager->Set(i, all_dataS[i].play_simultaneously, all_dataS[i].file_name, all_dataS[i].b3D);
	}

	// ���X�i�[�̏����ݒ�
	m_ListenerPos = Vector2(tdnSystem::GetScreenSize().right * .5f, tdnSystem::GetScreenSize().bottom * .5f);
	//play_manager->SetListenerAll(
	//	Vector3(tdnSystem::GetScreenSize().right * .5f, tdnSystem::GetScreenSize().bottom * .5f, 0),	// ���X�i�[���W(��ʂ̐^�񒆂Ɖ��肷��)
	//	Vector3(1, 0, 0),	// ���ʃx�N�g��
	//	Vector3(0, 1, 0),	// ����x�N�g��
	//	Vector3(0, 0, 0)	// �ړ��l
	//	);
}
//
//=============================================================================================



//=============================================================================================
//		��		��
SE_Manager::~SE_Manager()
{
	delete play_manager;
}
//
//=============================================================================================



//=============================================================================================
//		�X		�V
void SE_Manager::Update()
{
	play_manager->UpdateListener();
}
//
//=============================================================================================



//=============================================================================================
//		��		��
int SE_Manager::Play_in(int data_num, bool loop)
{
	if (data_num != TDNSOUND_PLAY_NONE)
	{
		play_manager->SetPan(data_num, DSBPAN_CENTER);
		play_manager->SetVolume(data_num, DSBVOLUME_MAX);
		play_manager->Play(data_num, loop);
	}
	return TDNSOUND_PLAY_NONE;
}

int SE_Manager::Play_in(int data_num, const Vector2 &pos, const Vector2 &move, bool loop)
{
	if (data_num != TDNSOUND_PLAY_NONE)
	{
		// �X�e���I�蓮�Őݒ肵�Ă݂�
		static const int DSBPAN_WIDTH = 10000;
		const int pan = (int)((DSBPAN_WIDTH / 8)*((pos.x - m_ListenerPos.x) / (tdnSystem::GetScreenSize().right / 2)));
		const int vol = DSBVOLUME_MAX + (int)((DSBVOLUME_MIN / 32) * (sqrtf((m_ListenerPos.x - pos.x)*(m_ListenerPos.x - pos.x)) / m_ListenerPos.x) + (DSBVOLUME_MIN / 16) * (sqrtf((m_ListenerPos.y - pos.y)*(m_ListenerPos.y - pos.y)) / m_ListenerPos.y));
		play_manager->SetPan(data_num, pan);
		play_manager->SetVolume(data_num, vol);

		return play_manager->Play(data_num, loop);
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

void SE_Manager::SetTone(LPSTR _ID, int tone)
{
	play_manager->SetSpeed(ID[_ID], 1.0f + (float)tone*.048f);
}

void SE_Manager::SetPos(LPSTR _ID, int no, const Vector2 &pos)
{
	// �X�e���I�蓮�Őݒ肵�Ă݂�
	static const int DSBPAN_WIDTH = 10000;
	const int pan = (int)((DSBPAN_WIDTH / 8)*((pos.x - m_ListenerPos.x) / (tdnSystem::GetScreenSize().right / 2)));
	const int vol = DSBVOLUME_MAX + (int)((DSBVOLUME_MIN / 32) * (sqrtf((m_ListenerPos.x - pos.x)*(m_ListenerPos.x - pos.x)) / m_ListenerPos.x) + (DSBVOLUME_MIN / 16) * (sqrtf((m_ListenerPos.y - pos.y)*(m_ListenerPos.y - pos.y)) / m_ListenerPos.y));
	play_manager->SetPan(no, pan);
	play_manager->SetVolume(no, vol);
}

void SE_Manager::SetListener(const Vector2 &pos)
{
	m_ListenerPos = pos;
	//play_manager->SetListenerPos(Vector3(pos.x, pos.y, 0));
	//play_manager->SetListenerMove(Vector3(move.x, move.y, 0));
}
//
//=============================================================================================






//**************************************************************************************************************
//
//		BGM�Ǘ��N���X
//
//**************************************************************************************************************


//=============================================================================================
//		��	��	��
void BGM_Manager::Initialize()
{
	play_manager = new tdnSoundBGM;

	for (int i = 0; i < _countof(all_dataB); i++)
	{
		//if (strcmp(all_dataB[i].id, "END") == 0) break;	// �I�[

		ID[all_dataB[i].id] = i;
		play_manager->Set(i, all_dataB[i].file_name, all_dataB[i].b3D);
	}

	// ���X�i�[�̏����ݒ�
	play_manager->SetListenerAll(
		Vector3(tdnSystem::GetScreenSize().right * .5f, tdnSystem::GetScreenSize().bottom * .5f, 0),	// ���X�i�[���W(��ʂ̐^�񒆂Ɖ��肷��)
		Vector3(1, 0, 0),	// ���ʃx�N�g��
		Vector3(0, 1, 0),	// ����x�N�g��
		Vector3(0, 0, 0)	// �ړ��l
		);
}
//
//=============================================================================================



//=============================================================================================
//		��		��
BGM_Manager::~BGM_Manager()
{
	delete play_manager;
}
//
//=============================================================================================

//=============================================================================================
//		�X		�V
void BGM_Manager::Update()
{
	play_manager->Update();
	play_manager->UpdateListener();
}
//
//=============================================================================================



//=============================================================================================
//		��		��
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
//		��		��
SE_Manager *se;
BGM_Manager *bgm;
//
//=============================================================================================