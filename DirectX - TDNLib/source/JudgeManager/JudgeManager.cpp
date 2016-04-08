#include "JudgeManager.h"
#include "../AI/NPC/PersonManager.h"

//**************************************************************************************************
//
//		★ステートごとのメッセージ受信処理
//
//**************************************************************************************************

//===============================================================================
//		グローバルステート
bool Judge::GlobalState::OnMessage(JudgeManager *pJudge, const Message &msg)
{
	// 出ていけ！
	return false;
}

//===============================================================================
//		全員に流す
void Judge::AllShed::Enter(JudgeManager *pJudge)
{
	m_ShedCount = 0;

	// 全員なので、リストのサイズ
	// ★TODO:うわさを流してはいけない人間はカウントしないようにしたい
	m_GoalCount = PersonMgr.GetMaxStayPerson();
}
bool Judge::AllShed::OnMessage(JudgeManager *pJudge, const Message &msg)
{
	switch (msg.Msg)
	{
	case MESSAGE_TYPE::SHED_GOSSIP:
		// 全員流した！
		if (++m_ShedCount >= m_GoalCount)
		{
			pJudge->SetJudgeMode(JudgeManager::JUDGEMODE::GAME_CLEAR);
		}
		break;

	case MESSAGE_TYPE::GAMEOVER:
		// 流してはいけないヤツに流したとか
		pJudge->SetJudgeMode(JudgeManager::JUDGEMODE::GAME_OVER);
		break;

	case MESSAGE_TYPE::GOAL_GOSSIP:
	{
		MyAssert(0, "クリア条件は全員に流すと設定されていますが、ゴールとなる人間がいます");
		break;
	}
	default:
		MyAssert(0, "そんな命令は受け付けてないです");
		break;
	}


	return false;// [上手くメッセージが届かなかった]
}

//===============================================================================
//		特定人物に流す
bool Judge::GoalPerson::OnMessage(JudgeManager *pJudge, const Message &msg)
{
	switch (msg.Msg)
	{
	case MESSAGE_TYPE::SHED_GOSSIP:
		break;

	case MESSAGE_TYPE::GAMEOVER:
		// 流してはいけないヤツに流したとか
		pJudge->SetJudgeMode(JudgeManager::JUDGEMODE::GAME_OVER);
		break;

	case MESSAGE_TYPE::GOAL_GOSSIP:
	{
		/*ここに来たらゲームクリア！あとはクリア処理頼みます*/
		// このクラスの略はJudgeMgrです

		pJudge->SetJudgeMode(JudgeManager::JUDGEMODE::GAME_CLEAR);

		return true;// [上手くメッセージが届いた!]
		break;
	}
	default:
		MyAssert(0, "そんな命令は受け付けてないです");
		break;
	}
	
	
	return false;// [上手くメッセージが届かなかった]
}



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
	BaseGameEntity(ENTITY_ID::JUDGE_MNG),// ★ジャッジマネージャー用のＩＤ番号を渡す
	m_ClearFlag(CLEAR_FLAG::GOAL_PERSON)
{
	// 判定状態初期化
	m_JudgeMode = JUDGEMODE::NONE;

	// ★ステートマシンの初期化
	m_pStateMachine = new StateMachine<JudgeManager>(this);
	m_pStateMachine->SetGlobalState(Judge::GlobalState::GetInstance());// グローバル
}


// 解放
JudgeManager::~JudgeManager()
{
	SAFE_DELETE(m_pStateMachine);
}

void JudgeManager::Update()
{
	m_pStateMachine->Update();
}


//**************************************************************************************************
//		メッセージ受信
//**************************************************************************************************
bool JudgeManager::HandleMessage(const Message & msg)
{
	// まあこのMGRにステートマシンはさすがにいいかな・・・
	// ということでここで直にメッセージに対する処理を

	//switch (msg.Msg)
	//{
	//case MESSAGE_TYPE::GOAL_GOSSIP:
	//{
	//	int a = 0;
	//	a++;
	//	/*ここに来たらゲームクリア！あとはクリア処理頼みます*/
	//	// このクラスの略はJudgeMgrです
	//
	//	return true;// [上手くメッセージが届いた!]
	//	break;
	//}
	//default:
	//	MyAssert(0, "そんな命令は受け付けてないです");
	//	break;
	//}
	//
	//
	//return false;// [上手くメッセージが届かなかった]

	// 一応、複数のクリア条件にも対応するようにステートマシンで分けてみます。↑の処理はステートの中で書いています
	return m_pStateMachine->HandleMessage(msg);
}

//**************************************************************************************************
//		クリア条件の設定
//**************************************************************************************************
void JudgeManager::SetClearFlag(CLEAR_FLAG flag)
{
	SetClearFlag((BYTE)flag);	// 下の関数を呼び出す

	// 条件に応じて、ステートを変更
	//switch (flag)
	//{
	//case CLEAR_FLAG::ALL_SHED:
	//	m_pStateMachine->SetCurrentState(Judge::AllShed::GetInstance());
	//	break;
	//
	//case CLEAR_FLAG::GOAL_PERSON:
	//	m_pStateMachine->SetCurrentState(Judge::GoalPerson::GetInstance());
	//	break;
	//default:
	//	MyAssert(0, "例外: 意図しないクリアフラグが設定されました");
	//	break;
	//}
}

void JudgeManager::SetClearFlag(BYTE flag)
{
	// 条件に応じて、ステートを変更
	if (flag & (BYTE)CLEAR_FLAG::ALL_SHED)
	{
		m_ClearFlag = CLEAR_FLAG::ALL_SHED;
		m_pStateMachine->SetCurrentState(Judge::AllShed::GetInstance());
	}
	else if (flag & (BYTE)CLEAR_FLAG::GOAL_PERSON)
	{
		m_ClearFlag = CLEAR_FLAG::GOAL_PERSON;
		m_pStateMachine->SetCurrentState(Judge::GoalPerson::GetInstance());
	}
	else MyAssert(0, "例外: 意図しないクリアフラグが設定されました");

	// 特定人物に流すなフラグの設定
	m_bDontShed = !(!(flag & (BYTE)CLEAR_FLAG::DONT_SHED_PERSON));
}