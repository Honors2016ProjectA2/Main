#include "JudgeManager.h"




// 宣言
JudgeManager* JudgeManager::pInstance = nullptr;

/***********/
//	実体取得
/***********/
JudgeManager & JudgeManager::GetInstance()
{
	if (!pInstance)
	{
		pInstance = new JudgeManager();
	}

	// TODO: return ステートメントをここに挿入します
	return *pInstance;
}

/***********/
//	解放
/***********/
void JudgeManager::Release()
{
	if (pInstance)
	{
		delete pInstance;
		pInstance = nullptr;
	}
}


// 初期化
JudgeManager::JudgeManager() :
	BaseGameEntity(ENTITY_ID::JUDGE_MNG)// ★ジャッジマネージャー用のＩＤ番号を渡す
{


}


// 解放
JudgeManager::~JudgeManager()
{

}

void JudgeManager::Update()
{


}

bool JudgeManager::HandleMessage(const Message & msg)
{
	// まあこのMGRにステートマシンはさすがにいいかな・・・
	// ということでここで直にメッセージに対する処理を

	switch (msg.Msg)
	{
	case MESSAGE_TYPE::GOAL_GOSSIP:
	{
		int a = 0;
		a++;
		/*ここに来たらゲームクリア！あとはクリア処理頼みます*/
		// このクラスの略はJudgeMgrです

		return true;// [上手くメッセージが届いた!]
		break;
	}
	default:
		MyAssert(0, "そんな命令は受け付けてないです");
		break;
	}


	return false;// [上手くメッセージが届かなかった]
}
