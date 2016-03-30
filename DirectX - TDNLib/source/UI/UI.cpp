#include "TDNLIB.h"

#include "UI.h"

// sceneMain�̃O���[�o���ϐ�����
extern int RippleCount;


/*************************/
//	�ÓI�����o�ϐ��̐錾
/*************************/
UIManager *UIManager::pInstance = nullptr;


/***********/
//	���̎擾
/***********/
UIManager &UIManager::GetInstance()
{
	if (!pInstance)
	{
		pInstance = new UIManager();
	}

	// TODO: return �X�e�[�g�����g�������ɑ}�����܂�
	return *pInstance;
}

/***********/
//	������
/***********/
void UIManager::Initialize()
{
	std::ifstream ifs("DATA/Text/UI/main.txt");
	assert(ifs);	// �t�@�C�������ĂȂ����t�@�C�����ԈႦ�Ă��I

	int count(0);
	while (!ifs.eof())
	{
		UIData *data = new UIData;

		char skip[64];
		ifs >> skip;
		ifs >> skip;
		int ID;
		ifs >> ID;

		assert(ID == count);	// �e�L�X�g��ID��0���珇�Ԃɕ��ׂĂ��������I�I
		count++;

		// �摜�̃t�@�C���p�X
		ifs >> skip;
		char filename[128];
		ifs >> filename;
		data->lpImage = new tdn2DObj(filename);

		// �摜���W	�����̒l�͖������Ď�ł��Ŕz�u���Ă��ǂ�
		ifs >> skip;
		ifs >> data->pos.x;
		ifs >> data->pos.y;

		m_Datas.push_back(data);
	}
}

/***********/
//	���
/***********/
UIManager::~UIManager()
{
	// ���X�g�̊J��
	for (auto it : m_Datas)
	{
		delete it;
	}
}

void UIManager::Release()
{
	// �ÓI���̂̉�����̃f�X�g���N�^���Ăяo��
	SAFE_DELETE(pInstance);
}


/***********/
//	�X�V
/***********/
void UIManager::Update()
{

}

/***********/
//	�`��
/***********/
void UIManager::Render()
{
	// �c��
	ID_Render(UI_ID::NOKORI);

	// �c��̐���
	m_Datas[UI_ID::NOKORI_NUMBER]->lpImage->Render(
		(int)m_Datas[UI_ID::NOKORI_NUMBER]->pos.x,
		(int)m_Datas[UI_ID::NOKORI_NUMBER]->pos.y, 
		256, 128, 256*RippleCount, 0, 256, 128);
}








// ���ڂ�
void UIManager::ID_Render(UI_ID id)
{
	m_Datas[id]->lpImage->Render((int)m_Datas[id]->pos.x, (int)m_Datas[id]->pos.y);
}