#include "GossipRipple.h"


GossipRipple::GossipRipple(RIPPLE_TYPE type, Vector3 pos)
{
	m_Type = type;	// �g��̐F�̃^�C�v

	m_uvObj = NULL;
	m_Pos = pos;
	m_Size = 0.1f;

	switch (m_Type)// �^�C�v�ɂ����UV�G�t�F�N�g��ς���
	{
	case RIPPLE_TYPE::WHITE:
		m_uvObj = new AnimationUV("DATA/UV/wave.imo", 0.00f, 0.01f, 60, false, 1, 45);
		break;
	case RIPPLE_TYPE::RED:
		m_uvObj = new AnimationUV("DATA/UV/Red/wave.imo", 0.00f, 0.01f, 60, false, 1, 45);
		break;
	case RIPPLE_TYPE::BLUE:
		m_uvObj = new AnimationUV("DATA/UV/Blue/wave.imo", 0.00f, 0.01f, 60, false, 1, 45);
		
		m_Size = 0.25f;// (��)�T�C�Y�ł���
		break;
	default:
		MyAssert(0, "�˂���");
		break;
	}


	//m_Flame = 0;

	//isAction = false;
	//isEnd = false;

}

GossipRipple::~GossipRipple()
{
	SAFE_DELETE(m_uvObj);
}

// ���s
void GossipRipple::Action()
{
	// �V����
	m_uvObj->Action();

	//isAction = true;
	// ������
	//isEnd = false;
	//m_Flame = 0;
	//m_Size = 1.0f;
}

// �X�V
void GossipRipple::Update()
{
	//if (isAction == false)return;// �N������܂ōX�V���Ȃ��B 

	//m_Flame++;// �J�E���g�J�n
	//if (m_Flame >= 40)
	//{
	//	m_Flame = 0;
	//	isEnd = true;
	//	isAction = false;	// ���s��~
	//}

	//m_Size += 0.01f;
	//m_uvObj->GetObj()->SetScale(m_Size);
	//obj->SetPos(m_Pos);
	//obj->Update();
	m_uvObj->Update(m_Pos, VECTOR_ZERO, m_Size);
}

// �`��
void GossipRipple::Render()
{
	//if (isAction == false)return;// �N������܂ōX�V���Ȃ��B

	m_uvObj->Render();
}
