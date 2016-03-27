#include "GossipRipple.h"


GossipRipple::GossipRipple(RIPPLE_TYPE type, Vector3 pos)
{
	m_Type = type;	// �g��̐F�̃^�C�v

	obj = NULL;
	switch (m_Type)
	{
	case RIPPLE_TYPE::WHITE:
		obj = new iexMesh("Data/hasubo.IMO");
		break;
	case RIPPLE_TYPE::RED:
		obj = new iexMesh("Data/hasubo.IMO");
		break;
	case RIPPLE_TYPE::BLUE:
		obj = new iexMesh("Data/hasubo.IMO");
		break;
	default:
		MyAssert(0, "�˂���");
		break;
	}

	m_Pos = pos;
	m_Size = 1.0f;
	m_Flame = 0;

	isAction = false;
	isEnd = false;

}

GossipRipple::~GossipRipple()
{
	SAFE_DELETE(obj);
}

// ���s
void GossipRipple::Action()
{
	isAction = true;
	// ������
	isEnd = false;
	m_Flame = 0;
	m_Size = 1.0f;
}

// �X�V
void GossipRipple::Update()
{
	if (isAction == false)return;// �N������܂ōX�V���Ȃ��B 

	m_Flame++;// �J�E���g�J�n
	if (m_Flame >= MAX_FLAME)
	{
		m_Flame = 0;
		isEnd = true;
		isAction = false;	// ���s��~
	}

	//m_Size += 0.01f;
	obj->SetScale(m_Size);
	obj->SetPos(m_Pos);
	obj->Update();
}

// �`��
void GossipRipple::Render()
{
	if (isAction == false)return;// �N������܂ōX�V���Ȃ��B

	obj->Render();
}
