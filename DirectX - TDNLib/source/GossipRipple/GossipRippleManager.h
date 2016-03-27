#pragma once
#include "TDNLIB.h"
#include "GossipRipple.h"


/*
	噂の波紋マネージャー.h

*/

class GossipRippleManager
{
public:
	static GossipRippleManager& GetInstance();
	static void Release();
	virtual ~GossipRippleManager();

	// 描画・更新
	void Update();
	void Render();

	// 波紋追加
	void AddRipple(RIPPLE_TYPE type, Vector3 pos);

private:
	static GossipRippleManager* pInstance;

	// 封印
	GossipRippleManager();
	GossipRippleManager(const GossipRippleManager&);
	GossipRippleManager& operator=(const GossipRippleManager&);

	
	// 波紋データ
	std::vector<GossipRipple*> m_RippleData;



};


// インスタンス化
#define GossipRippleMgr	(GossipRippleManager::GetInstance())


