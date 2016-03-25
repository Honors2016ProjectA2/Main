#include "AI\NPC\PersonManager.h"

//　ひとの種類
#include "AI\NPC\BasePerson\BasePerson.h"



// 宣言
PersonManager* PersonManager::pInstance = nullptr;


/***********/
//	実体取得
/***********/
PersonManager & PersonManager::GetInstance()
{
	if (!pInstance)
	{
		pInstance = new PersonManager();
	}

	// TODO: return ステートメントをここに挿入します
	return *pInstance;
}

/***********/
//	解放
/***********/
void PersonManager::Release()
{
	if (pInstance)
	{
		delete pInstance;
		pInstance = nullptr;
	}
}


// 初期化
PersonManager::PersonManager()
{

}

// 解放
PersonManager::~PersonManager()
{

}
