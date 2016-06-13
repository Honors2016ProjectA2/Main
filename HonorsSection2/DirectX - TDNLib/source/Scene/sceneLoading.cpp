#include	"TDNLIB.h"
#include	"system/Framework.h"
#include	"sceneLoading.h"

//=============================================================================================
//		�O	��	�[	�o	��	��	��
bool sceneLoading::isThread;


//=============================================================================================
//		��	��	��	��	�J	��
bool sceneLoading::Initialize(BaseScene *newScene)
{
	m_pImage = new tdn2DObj("DATA/UI/loading.png");
	m_AnimeCount = m_AnimeFrame = 0;

	//	�ʃX���b�h�쐬
	//���̃V�[���̃|�C���^�͌�Ŏg���̂�newScene�ɕۑ����Ă����B
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
//		�X	��	�b	�h	��	��
void sceneLoading::Thread_funk(void *arg)
{
	BaseScene* scene = (BaseScene*)arg;

	scene->Initialize();	// �������Ŗ{�̂�scene���K�v�Ȏ��Ԃ������鏉�������s��
	scene->m_bLoad = true;	// ���̎��_�Ń��[�h�t���O��ON�B�V�[����؂�ւ���ۂ̓ǂݍ��݂����Ȃ��悤�ɂ���

	//	�X���b�h�I������
	isThread = false;
	_endthread();
}


//
//=============================================================================================

//=============================================================================================
//		�X			�V
bool sceneLoading::Update()
{
	// �摜�A�j���[�V��������
	if (++m_AnimeFrame > m_AnimeSpeed)
	{
		m_AnimeFrame = 0;
		if (++m_AnimeCount > 5) m_AnimeFrame = 0;

	}

	//���[�h���I�������A�V�[�����`�F���W
	if (!isThread)
	{
		MainFrame->ChangeScene(m_newScene);
	}

	return true;
}
//
//=============================================================================================


//=============================================================================================
//		�`			��
void sceneLoading::Render()
{
	tdnView::Activate();
	tdnView::Clear();

	tdnPolygon::Rect(0, 0, tdnSystem::GetScreenSize().right, tdnSystem::GetScreenSize().bottom , RS::COPY, 0xffffffff);
	m_pImage->Render(800, 620, 512, 64, 0, m_AnimeCount * 64, 512, 64);
}
//
//=============================================================================================