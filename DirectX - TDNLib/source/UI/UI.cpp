#include "TDNLIB.h"

#include "UI.h"

// sceneMain�̃O���[�o���ϐ�����
extern int RippleCount;



/*************************/
//	�����o���̃Z�b�g
/*************************/
void UIManager::PushHukidashi(const Vector3 &pos, HUKIDASHI_TYPE type)
{
	Vector2 srcXY;
	if (tdnRandom::Get(0, 1))
	{
		srcXY.x = 0;
		srcXY.y = 1;
	}
	else
	{
		srcXY.x = (float)tdnRandom::Get(0, 3);
		srcXY.y = 0;
	}
	srcXY.x *= 256;
	srcXY.y *= 128;

	tdn2DObj *Image;
	switch (type)
	{
	case HUKIDASHI_TYPE::ORIGIN:
		Image = m_Datas[UI_ID::HUKIDASHI_MINE]->lpImage;
		srcXY = Vector2(0, 0);
		break;
	case HUKIDASHI_TYPE::SUCCESS:
		Image = m_Datas[UI_ID::HUKIDASHI_CORRECT]->lpImage;
		break;
	case HUKIDASHI_TYPE::FAILED:
		Image = m_Datas[UI_ID::HUKIDASHI_INCORRECT]->lpImage;
		break;
	}
	Hukidashi *data = new Hukidashi(Image, srcXY, Hukidashi::DEFAULT_APP_TIME, pos);
	m_HukidashiList.push_back(data);
}

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
	for (auto it : m_HukidashiList)
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
	// �����o�����X�g�̍X�V
	for (auto it = m_HukidashiList.begin(); it != m_HukidashiList.end();)
	{
		(*it)->Update();
		if ((*it)->EraseOK())
		{ 
			delete (*it);
			it = m_HukidashiList.erase(it);
		}
		else it++;
	}
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

	// �����o�����X�g�̕`��
	for (auto it : m_HukidashiList) it->Render();
}




void Hukidashi::Render()
{
	// ��
	m_pImage->SetARGB((BYTE)(255 * ((float)m_AppTime / DEFAULT_APP_TIME)), (BYTE)255, (BYTE)255, (BYTE)255);

	//m_pImage->Render3D(m_pos, 256, 128, (int)m_SrcXY.x, (int)m_SrcXY.y, 256, 128);
	m_pImage->Render((int)Math::WorldToScreen(m_pos).x - 20, (int)Math::WorldToScreen(m_pos).y - 160, 256, 128, (int)m_SrcXY.x, (int)m_SrcXY.y, 256, 128);
}



// ���ڂ�
void UIManager::ID_Render(UI_ID id)
{
	m_Datas[id]->lpImage->Render((int)m_Datas[id]->pos.x, (int)m_Datas[id]->pos.y);
}