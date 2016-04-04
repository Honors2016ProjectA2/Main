
#include "AI\NPC\PersonManager.h"

//　ひとの種類
#include "AI\NPC\BasePerson\BasePerson.h"
#include "../../UI/UI.h"
#include "../../Sound/SoundManager.h"

#include "WaitPerson\WaitPerson.h"


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
	m_delay(0)
{
	

}


// 解放
PersonManager::~PersonManager()
{
	// 全部消して
	for (int i = 0; i < (int)m_PersonData.size(); i++)
	{
		SAFE_DELETE(m_PersonData[i]);
	}

	//　データを空に
	m_PersonData.clear();
}

// キャラ追加
void PersonManager::AddPerson(PERSON_TYPE type,Vector3 pos)
{
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
	default:
		MyAssert(0, "そんな奴はいない");
		break;
	}



	// 配列に加える
	m_PersonData.push_back(data);

	++m_IDcount;// 次の人の番号へ
}

void PersonManager::Reset()
{
	m_IDcount = 0;

}

void PersonManager::Update()
{
	if (m_PersonData.empty())return;// 空だったらリターン

	// 人たち更新

	// ステージ3のすぐクリアになってしまうバグを直すためのごり押しを超えた何か
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

	for (int i = 0; i < (int)m_PersonData.size(); i++)
	{
		if (m_PersonData[i]->IsShed()) m_NumShedPerson++;
		m_PersonData[i]->Update();
	}
}

void PersonManager::Render()
{
	if (m_PersonData.empty())return;// 空だったらリターン


								   // 人たち描画
	for (int i = 0; i < (int)m_PersonData.size(); i++)
	{
		m_PersonData[i]->Render();
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
	for (int i = 0; i < (int)m_PersonData.size(); i++)
	{
		m_PersonData[i]->SetIsShed(false);	// 噂流したフラグ=Falseに
		m_PersonData[i]->ResetState();
	}
}

/*	(現在)一人でもＯＫ・波紋は一回しか反応しない　*/
// 波紋 vs 人
// 当たり判定
void PersonManager::RippleVSPerson(RIPPLE_INFO* pRipData)// ←波紋
{
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

	// 二人以上ならこなみ
	//if (count2 >= 2)
	//{

		for (int b = 0; b < (int)m_PersonData.size(); b++)
		{

			if (m_PersonData[b]->IsShed() == true)continue;// 噂を立てたやつは反応しない
			//if (m_PersonData[b]->GetPersonType() == pRipData->type) continue;// 同じタイプはバイバイ

			float ren =
				Math::Length(pRipData->pos, m_PersonData[b]->GetPos());
			if (ren <= pRipData->size)// 30m以内に人が存在すると
			{
				// 二人以上ならこなみ
				//if (count2 >= 2)
				{
					// 近くにいた人は波紋を飛ばす
					m_PersonData[b]->ActionGossipRipple();

					// 成功時の吹き出し
					UIMgr.PushHukidashi(m_PersonData[b]->GetPos(), HUKIDASHI_TYPE::SUCCESS);

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
		m_delay = 10;
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

	return  m_PersonData[no];

}

