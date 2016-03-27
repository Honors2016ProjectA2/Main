#pragma once
#include "TDNLIB.h"
#include "GossipRipple.h"


/*
	�\�̔g��}�l�[�W���[.h

*/

class GossipRippleManager
{
public:
	static GossipRippleManager& GetInstance();
	static void Release();
	virtual ~GossipRippleManager();

	// �`��E�X�V
	void Update();
	void Render();

	// �g��ǉ�
	void AddRipple(RIPPLE_TYPE type, Vector3 pos);

private:
	static GossipRippleManager* pInstance;

	// ����
	GossipRippleManager();
	GossipRippleManager(const GossipRippleManager&);
	GossipRippleManager& operator=(const GossipRippleManager&);

	
	// �g��f�[�^
	std::vector<GossipRipple*> m_RippleData;



};


// �C���X�^���X��
#define GossipRippleMgr	(GossipRippleManager::GetInstance())


