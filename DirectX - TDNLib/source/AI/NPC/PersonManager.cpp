
#include "AI\NPC\PersonManager.h"

//　ひとの種類
#include "AI\NPC\BasePerson\BasePerson.h"
#include "../../UI/UI.h"
#include "../../Sound/SoundManager.h"

#include "WaitPerson\WaitPerson.h"
#include "StartPerson\StartPerson.h"
#include "GoalPerson\GoalPerson.h"
#include "StopPerson\StopPerson.h"
#include "StrongPerson\StrongPerson.h"
#include "GameOverPerson\GameOverPerson.h"

// 宣言
PersonManager* PersonManager::pInstance = nullptr;

int PersonManager::m_IDcount = 0;

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
PersonManager::PersonManager():
	BaseGameEntity(ENTITY_ID::PERSON_MNG),// ★人のマネージャー用のＩＤ番号を渡す
	m_NumShedPerson(0),
	m_bJudgeMoment(false),
	m_combo(0),
	m_ComboAction(false),
	m_posUp(0)
{
	m_ComboTimer = COMBO_TIMER;

}


// 解放
PersonManager::~PersonManager()
{
	// 全部消して
	for (auto it : m_PersonData)
	{
		SAFE_DELETE(it);
	}

	//　データを空に
	m_PersonData.clear();
}

// キャラ追加
ENTITY_ID PersonManager::AddPerson(PERSON_TYPE type,Vector3 pos, bool isStay)
{
	m_posUp += 0.1f;
	pos.y += m_posUp;

	// 人
	BasePerson* data;

	ENTITY_ID id = (ENTITY_ID)(GAME_ID::id_person_first + m_IDcount);

	// ここでキャラクターのデータを変える
	switch (type)
	{
	case PERSON_TYPE::WAIT:
		data = new WaitPerson(id);
		data->SetPos(pos);

		break;
	case PERSON_TYPE::START:
		data = new StartPerson(id);
		data->SetPos(pos);

		// ★追加 スタートの人間だったらカメラ演出に必要なので座標を送ってあげる！(ここでイントロカメラのスイッチがONになる)
		//if (type == PERSON_TYPE::START)
		{
			// 構造体作成
			INTRO_CAMERA_INFO ex;
			ex.start_person_pos = pos;	// 噂スタート人の座標

			// カメラマネージャーに情報送信
			MsgMgr->Dispatch(
				MSG_NO_DELAY,
				ENTITY_ID::PERSON_MNG,
				ENTITY_ID::CAMERA_MNG,
				MESSAGE_TYPE::START_INTRO_CAMERA,	// イントロカメラ稼働！
				(void*)&ex	// [追記情報]自分のタイプを送る
				);
		}

		break;
	case PERSON_TYPE::GOAL:
		data = new GoalPerson(id);

		break;
	case PERSON_TYPE::STOP:
		data = new StopPerson(id);

		break;
	case PERSON_TYPE::STRONG:
		data = new StrongPerson(id);

		break;
	case PERSON_TYPE::GAMEOVER:
		data = new GameOverPerson(id);

		break;
	default:
		MyAssert(0, "そんな奴はいない");
		break;
	}

	// ここで設定を変更
	data->SetPos(pos);
	data->SetIsStay(isStay);

	// 配列に加える
	m_PersonData.push_back(data);

	++m_IDcount;// 次の人の番号へ

	// 設置した人のIDを返す
	return id;
}

void PersonManager::Reset()
{
	m_IDcount = 0;
	m_posUp = 0.0f;
}

void PersonManager::Update()
{
	if (m_PersonData.empty())return;// 空だったらリターン

	// 人たち更新

	// 噂の流れが続いているか止まっているかの判定
	if (m_ComboAction)
	{
		if (m_ComboTimer > 0) m_ComboTimer--;
	}
	//{
	//	if (m_delay > 0)
	//	{
	//		if (--m_delay == 0)
	//		{
	//			bool bShedNow(false);
	//			for (auto it : m_PersonData)
	//			{
	//				if (it->IsShedNow())
	//				{
	//					bShedNow = true;
	//				}
	//			}

	//			if (bShedNow)
	//			{
	//				m_delay = 10;
	//			}
	//			else
	//			{
	//				m_bJudgeMoment = true;
	//				m_combo = 0;
	//			}
	//		}

	//	}
	//	else
	//	{
	//		m_bJudgeMoment = false;
	//	}
	//}

	m_NumShedPerson = 0;

	for (auto it : m_PersonData)
	{
		if (it->IsShed()) m_NumShedPerson++;
		it->Update();
	}
}

void PersonManager::Render()
{
	if (m_PersonData.empty())return;// 空だったらリターン


								   // 人たち描画
	for (auto it : m_PersonData)
	{

		it->RangeRender();

		it->Render();


	}
	// 人たちのＵＩ描画
	for (auto it : m_PersonData)
	{

		it->UIRender();
	}
}

// メッセージ
bool  PersonManager::HandleMessage(const Message& msg)
{
	// まあこのMGRにステートマシンはさすがにいいかな・・・
	// ということでここで直にメッセージに対する処理を

	switch (msg.Msg)
	{
	case MESSAGE_TYPE::RIPPLE_VS_PERSON:
	{
		// オリジナル構造体のポインタ―型で受け取るぜ！

		RIPPLE_INFO* exInfo = (RIPPLE_INFO*)msg.ExtraInfo;
		//float* exInfo = (float*)msg.ExtraInfo;

		// ここでＶＳ関数を起動　to be contted 
		RippleVSPerson(exInfo);

		return true;// [上手くメッセージが届いた!]
		break;
	}
	default:
		MyAssert(0, "そんな命令は受け付けてないです");
		break;
	}


	return false;// [上手くメッセージが届かなかった]
}

// リセット
void PersonManager::ResetState()
{

	// 全員のステート初期化
	for (auto it : m_PersonData)
	{
		it->SetIsShed(false);	// 噂流したフラグ=Falseに
		it->ResetState();
	}
}
// スタートの人の波紋を出す！
void PersonManager::StartGossip()
{
	// 全員のステート初期化
	for (auto it : m_PersonData)
	{
		// 全員に送信
		MsgMgr->Dispatch(
			MSG_NO_DELAY,
			ENTITY_ID::PERSON_MNG,
			it->GetID(),
			START_GOSSIP,
			MSG_NO_EXINFO
			);
	}
}

/*	(現在)一人でもＯＫ・波紋は一回しか反応しない　*/
// 波紋 vs 人
// 当たり判定
void PersonManager::RippleVSPerson(RIPPLE_INFO* pRipData)// ←波紋
{
	// コンボ継続！
	m_ComboTimer = COMBO_TIMER;
	m_ComboAction = true;

	// 対象
	int count(0);
	//int count2(0);

	// 何人当たっているか
	//for (int b = 0; b < (int)m_PersonData.size(); b++)
	//{
	//	if (m_PersonData[b]->IsShed() == true)continue;// 噂を立てたやつは反応しない
	//	if (m_PersonData[b]->GetPersonType() == pRipData->type) continue;// 同じタイプはバイバイ

	//	float ren = Math::Length(pRipData->pos, m_PersonData[b]->GetPos());
	//	if (ren <= pRipData->size)// 30m以内に人が存在すると
	//	{
	//		count2++;// (仮)
	//	}
	//}	

	// ★止める人がいたら波紋を出さない
	for (auto it : m_PersonData)
	{
		if (it->IsShed() == true)continue;// 噂を立てたやつは反応しない

		float ren = Math::Length(pRipData->pos, it->GetPos());
		if (ren <= pRipData->size)// 30m以内に人が存在すると
		{
			if (it->GetPersonType() == PERSON_TYPE::STOP)
			{
				return;// 止める人がいたらコンティニュー
			}

		}
	}

	// 二人以上ならこなみ
	//if (count2 >= 2)
	//{

	for (auto it : m_PersonData)
		{

			if (it->IsShed() == true)continue;// 噂を立てたやつは反応しない
			//if (m_PersonData[b]->GetPersonType() == pRipData->type) continue;// 同じタイプはバイバイ
		

			float ren =
				Math::Length(pRipData->pos, it->GetPos());
			if (ren <= pRipData->size)// 30m以内に人が存在すると
			{
				// 二人以上ならこなみ
				//if (count2 >= 2)
				{
					// 近くにいた人は波紋を飛ばす
					it->ActionGossipRipple();

					// 成功時の吹き出し
					//UIMgr.PushHukidashi(m_PersonData[b]->GetPos(), HUKIDASHI_TYPE::SUCCESS);

					// コンボ数を増やす(2人カウントしたら、2トーン上げしまうのでその対処)
					if (count == 0) se->SetTone("波紋出す", ++m_combo);

					count++;
				}
				//else
				//{
				//	// 失敗時の吹き出し
				//	UIMgr.PushHukidashi(m_PersonData[b]->GetPos(), HUKIDASHI_TYPE::FAILED);
				//}
			}

		}

	//}

	if (count == 0)
	{
		// 判定してますよー！
		//m_bJudgeMoment = true;
		//m_delay = 10;
	}
}

BasePerson * PersonManager::GetPerson(int no)
{
	if (m_PersonData.empty())
	{
		MyAssert(0, "データが空");
	}

	// エラー処理
	if (no >= (int)m_PersonData.size())
	{
		MyAssert(0, "指定した場所にデータがない");
	}

	int i(0);
	m_it = m_PersonData.begin();
	while (i != no)
	{
		m_it++;
		i++;
	}

	return (*m_it);

}

void PersonManager::ProductRipple()
{
	// 全員に演出用のメッセージ送る
	for (auto it : m_PersonData)
	{
		// 全員に送信
		MsgMgr->Dispatch(
			MSG_NO_DELAY,
			ENTITY_ID::PERSON_MNG,
			it->GetID(),
			PRODUCT_RIPPLE,
			MSG_NO_EXINFO
			);
	}

}

// 最大数取得
int PersonManager::GetMaxStayPerson()
{
	// ふぉｒ分でキャラクター全部見て
	// フラグの立ってるやつをカウント
	int ret = 0;
	for (auto it : m_PersonData)
	{
		ret += it->IsStay();

	}
	return ret;
}

// 引数にidをもらってそのidのパーソンを消す処理
int PersonManager::ErasePerson(ENTITY_ID id)
{
	// 削除した際に回復する設置ポイント
	int recover(0);

	for (auto it = m_PersonData.begin(); it != m_PersonData.end();)
	{
		if ((*it)->GetID() == id)
		{
			// コストに応じて回復量を
			if ((*it)->GetPersonType() == PERSON_TYPE::STRONG) recover = 2;
			else recover = 1;

			delete (*it);
			it = m_PersonData.erase(it);
			break;
		}
		it++;
	}
	return recover;
}

