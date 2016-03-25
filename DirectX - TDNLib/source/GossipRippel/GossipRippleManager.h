#pragma once


/*
	�\�̔g��}�l�[�W���[.h

*/

class GossipRippleManager
{
public:
	static GossipRippleManager& GetInstance();
	static void Release();
	virtual ~GossipRippleManager();

private:
	static GossipRippleManager* pInstance;

	// ����
	GossipRippleManager();
	GossipRippleManager(const GossipRippleManager&);
	GossipRippleManager& operator=(const GossipRippleManager&);


};


// �C���X�^���X��
#define GossipRippleMgr	(GossipRippleManager::GetInstance())


