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
	// 全部消して
	for (int i = 0; i < (int)m_RippleData.size(); i++)
	{
		SAFE_DELETE(m_RippleData[i]);
	}

	//　データを空に
	m_RippleData.clear();
}

// 更新
void GossipRippleManager::Update()
{

	if (m_RippleData.empty())return;// 空だったらリターン

									
	for (int i = 0; i < (int)m_RippleData.size(); i++)
	{
		m_RippleData[i]->Update();

		// (TODO)★ここで消す処理
		if (m_RippleData[i]->IsEnd()==true)
		{
		
		}
	}

}

// 描画
void GossipRippleManager::Render()
{
	if (m_RippleData.empty())return;// 空だったらリターン
									
	for (int i = 0; i < (int)m_RippleData.size(); i++)
	{
		m_RippleData[i]->Render();
	}

}

void GossipRippleManager::AddRipple(RIPPLE_TYPE type, Vector3 pos)
{
	GossipRipple* data;
	
	data = new GossipRipple(type, pos);
	// +Action (TODO) 今はここで波紋を広げる
	data->Action();

	// 要素追加
	m_RippleData.push_back(data);

}


