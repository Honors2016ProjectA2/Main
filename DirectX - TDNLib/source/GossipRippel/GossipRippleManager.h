#pragma once


/*
	噂の波紋マネージャー.h

*/

class GossipRippleManager
{
public:
	static GossipRippleManager& GetInstance();
	static void Release();
	virtual ~GossipRippleManager();

private:
	static GossipRippleManager* pInstance;

	// 封印
	GossipRippleManager();
	GossipRippleManager(const GossipRippleManager&);
	GossipRippleManager& operator=(const GossipRippleManager&);


};


// インスタンス化
#define GossipRippleMgr	(GossipRippleManager::GetInstance())


