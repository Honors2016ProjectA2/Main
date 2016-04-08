#pragma once
#include "TDNLIB.h"
#include "AI\Entity\BaseGameEntity.h"
#include "AI\NPC\BasePerson\BasePerson.h"


/*
�\�̔g��}�l�[�W���[.h

*/

class BasePerson;

class PersonManager:public BaseGameEntity
{
public:
	static PersonManager& GetInstance();
	static void Release();
	virtual ~PersonManager();

private:
	static PersonManager* pInstance;

	// Entity�̃J�E���g
	static int m_IDcount;

	// ����
	PersonManager();
	PersonManager(const PersonManager&);
	PersonManager& operator=(const PersonManager&);

	// �l�f�[�^
	std::vector<BasePerson*> m_PersonData;

	// ���킳�����ꂽ�l�̑��v
	int m_NumShedPerson;

	// �\�����̔��肵���u��
	bool m_bJudgeMoment;

	// �g��R���{��
	int m_combo;

	// ���S������
	int m_delay;

public:
	// ��ʐ��Y
	/*
		���ŏ���ɂh�c��U��
	*/
	void AddPerson(PERSON_TYPE type, Vector3 pos, bool isStay = false );

	//	���Z�b�g
	void Reset();

	// �X�V�E�`��
	void Update();
	void Render();

	// ���b�Z�[�W�X�V
	bool  HandleMessage(const Message& msg);

	// �X�e�[�g���Z�b�g&isShed=false
	void ResetState();
	
	// �X�^�[�g�̐l�̔g����o���I
	void StartGossip();
 
	// �g��vs�l
	void RippleVSPerson(RIPPLE_INFO* pRipData);

	// �A�N�Z�T
	BasePerson* GetPerson(int no);
	int GetPersonSize() { return (int)m_PersonData.size(); }

	// �N���A����: �S���ɉ\�𗬂�����
	bool isAllShed(){ return (m_NumShedPerson == m_PersonData.size()); }
	bool isJudgeMoment(){ return m_bJudgeMoment; }

	// �R���{��
	int GetRippleCombo(){ return m_combo; }
	int GetDelay(){ return m_delay; }

	// ���o�p�q����
	void ProductRipple();

	int GetMaxStayPerson();

};


// �C���X�^���X��
#define PersonMgr	(PersonManager::GetInstance())


