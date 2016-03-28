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


public:
	// ��ʐ��Y
	/*
		���ŏ���ɂh�c��U��
	*/
	void AddPerson(PERSON_TYPE type, Vector3 pos);

	//	���Z�b�g
	void Reset();

	// �X�V�E�`��
	void Update();
	void Render();

	// ���b�Z�[�W�X�V
	bool  HandleMessage(const Message& msg);

	// �g��vs�l
	void RippleVSPerson(int ripPersonNo);

	// �A�N�Z�T
	BasePerson* GetPerson(int no);
	int GetPersonSize() { return (int)m_PersonData.size(); }
};


// �C���X�^���X��
#define PersonMgr	(PersonManager::GetInstance())


