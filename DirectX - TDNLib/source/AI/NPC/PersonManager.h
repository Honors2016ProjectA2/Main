#pragma once


/*
�\�̔g��}�l�[�W���[.h

*/

class BasePerson;

class PersonManager
{
public:
	static PersonManager& GetInstance();
	static void Release();
	virtual ~PersonManager();

private:
	static PersonManager* pInstance;

	// ����
	PersonManager();
	PersonManager(const PersonManager&);
	PersonManager& operator=(const PersonManager&);


};


// �C���X�^���X��
#define PersonMgr	(PersonManager::GetInstance())


