
#include "AI\NPC\PersonManager.h"

//　ひとの種類
#include "AI\NPC\BasePerson\BasePerson.h"



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
	BaseGameEntity(ENTITY_ID::PERSON_MNG)// ★人のマネージャー用のＩＤ番号を渡す
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

	data = new BasePerson(id, type);
	data->SetPos(pos);

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
	for (int i = 0; i < (int)m_PersonData.size(); i++)
	{
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
		//RIPPLE_INFO exInfo = (RIPPLE_INFO&)msg.ExtraInfo;
		//float exInfo = (float&)msg.ExtraInfo;

		// ここでＶＳ関数を起動　to be contted 

		return true;// [上手くメッセージが届いた!]
		break;
	}
	default:
		MyAssert(0, "そんな命令は受け付けてないです");
		break;
	}


	return false;// [上手くメッセージが届かなかった]
}

// 波紋 vs 人
// 当たり判定
void PersonManager::RippleVSPerson(int no)// ←波紋
{

	// 対象
	for (int b = 0; b < (int)m_PersonData.size(); b++)
	{
		if (no == b)continue;				// まず同じものは反応しない
		if (m_PersonData[b]->IsShed() == true)continue;// 噂を立てたやつは反応しない

		float ren =
			Math::Length(m_PersonData[no]->GetPos(), m_PersonData[b]->GetPos());
		if (ren <= 30)// 30m以内に人が存在すると
		{
			// 近くにいた人は波紋を飛ばす
			//m_PersonData[b]->ActionGossipRipple();

			// 近くにいた人に波紋のメッセージを送る
		}

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

