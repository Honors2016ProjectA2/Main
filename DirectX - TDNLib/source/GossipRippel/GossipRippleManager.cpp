#include "GossipRippleManager.h"


// 宣言
GossipRippleManager* GossipRippleManager::pInstance = nullptr;


/***********/
//	実体取得
/***********/
GossipRippleManager & GossipRippleManager::GetInstance()
{
	if (!pInstance)
	{
		pInstance = new GossipRippleManager();
	}

	// TODO: return ステートメントをここに挿入します
	return *pInstance;
}

/***********/
//	解放
/***********/
void GossipRippleManager::Release()
{
	if (pInstance)
	{
		delete pInstance;
		pInstance = nullptr;
	}
}


// 初期化
GossipRippleManager::GossipRippleManager()
{

}

// 解放
GossipRippleManager::~GossipRippleManager()
{

}


